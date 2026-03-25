"""
test_hft_pipeline_top.py — Cocotb testbench for hft_pipeline_top

Drives realistic S&P 500 market data through the full 7-stage HFT pipeline,
monitors all outputs, and feeds a live trading dashboard GUI.

Usage:
    make                          # default: 20 components, GUI on
    make HFT_NUM_COMPONENTS=500   # full S&P 500
    make HFT_GUI_ENABLE=0         # headless (no GUI)
"""

import os
import struct
import random
import threading
import queue
import time
import math
import logging
from collections import defaultdict, OrderedDict
from dataclasses import dataclass, field
from typing import Dict, List, Optional, Tuple

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge, Timer, ClockCycles, FallingEdge

# ---------------------------------------------------------------------------
# Configuration from environment
# ---------------------------------------------------------------------------
NUM_COMPONENTS  = int(os.environ.get("HFT_NUM_COMPONENTS", "20"))
GUI_ENABLE      = os.environ.get("HFT_GUI_ENABLE", "1") == "1"
IDX_SYMBOL      = NUM_COMPONENTS  # index instrument right after components
TOB_SYMBOLS     = NUM_COMPONENTS + 2
TOB_LEVELS      = 3
XDP_PORT        = 11101
FRAC_BITS       = 20
Q_ONE           = 1 << FRAC_BITS  # 1.0 in Q12.20

logger = logging.getLogger("hft_tb")
logger.setLevel(logging.INFO)

# ---------------------------------------------------------------------------
# Inter-thread queues  (cocotb ↔ GUI)
# ---------------------------------------------------------------------------
input_q   = queue.Queue(maxsize=5000)
book_q    = queue.Queue(maxsize=5000)
trade_q   = queue.Queue(maxsize=1000)
status_q  = queue.Queue(maxsize=500)
control_q = queue.Queue(maxsize=100)
# control_q commands: "pause", "play", "step", "quit"

# ===========================================================================
#  S&P 500 STOCK DATA
# ===========================================================================
_TOP_STOCKS = [
    ("AAPL",  19200), ("MSFT",  42500), ("AMZN",  18700), ("NVDA",  88000),
    ("GOOGL", 17500), ("META",  50500), ("BRK.B", 41000), ("TSLA",  25500),
    ("UNH",   52800), ("JNJ",   15800), ("V",     27500), ("XOM",   11700),
    ("JPM",   19800), ("PG",    16500), ("MA",    45200), ("HD",    36800),
    ("CVX",   16200), ("MRK",   12500), ("ABBV",  17400), ("LLY",   78000),
    ("PEP",   18200), ("KO",    6400),  ("AVGO",  17000), ("COST",  74000),
    ("MCD",   29500), ("WMT",   17800), ("TMO",   55000), ("CSCO",  5200),
    ("ACN",   33500), ("ABT",   11200), ("DHR",   25600), ("TXN",   17900),
    ("NEE",   7900),  ("PM",    10500), ("UNP",   24800), ("RTX",   10200),
    ("BMY",   5200),  ("INTC",  3100),  ("QCOM",  17500), ("AMGN",  28000),
    ("HON",   20800), ("COP",   11600), ("LOW",   24500), ("INTU",  62000),
    ("SPGI",  44500), ("BA",    19000), ("AMAT",  16000), ("GE",    16800),
    ("CAT",   29500), ("IBM",   19000), ("NOW",   70000), ("ADP",   25500),
    ("GS",    47500), ("BLK",   78000), ("SYK",   34500), ("MDLZ",  7600),
    ("DE",    39000), ("LMT",   46000), ("ISRG",  40000), ("ADI",   22000),
    ("GILD",  8600),  ("BKNG",  38000), ("SBUX",  10000), ("VRTX",  44000),
    ("MMC",   20000), ("CI",    35000), ("ZTS",   18500), ("AXP",   22500),
    ("PYPL",  7200),  ("PLD",   13000), ("REGN",  80000), ("CME",   21000),
    ("SLB",   5400),  ("SO",    8500),  ("DUK",   10600), ("AON",   34000),
    ("TMUS",  18000), ("WM",    20000), ("CL",    9100),  ("MO",    5100),
    ("APD",   29000), ("GD",    28500), ("ICE",   14000), ("FDX",   28000),
    ("PNC",   17500), ("NSC",   25000), ("FCX",   4500),  ("LRCX",  78000),
    ("MET",   7800),  ("MCK",   58000), ("USB",   4800),  ("KLAC",  68000),
    ("DG",    14000), ("PSA",   31000), ("EW",    7800),  ("PSX",   14000),
    ("ORLY",  95000), ("GM",    4500),  ("AEP",   9800),  ("EMR",   11000),
    ("SPY",   52500),  # Index instrument — last
]

def build_stock_list(n_components: int) -> List[dict]:
    """Build stock list: real tickers for top stocks, generated for the rest."""
    stocks = []
    rng = random.Random(42)

    for i in range(n_components):
        if i < len(_TOP_STOCKS) - 1:  # -1 to skip SPY at the end
            ticker, base = _TOP_STOCKS[i]
        else:
            ticker = f"SYM{i:03d}"
            base = rng.randint(2000, 60000)  # $20-$600 in cents
        stocks.append({"ticker": ticker, "idx": i, "base_price": base})

    # Index instrument
    spy_price = _TOP_STOCKS[-1][1]
    stocks.append({"ticker": "SPY", "idx": IDX_SYMBOL, "base_price": spy_price})
    return stocks


STOCKS = build_stock_list(NUM_COMPONENTS)
TICKER_MAP = {s["idx"]: s["ticker"] for s in STOCKS}

# ===========================================================================
#  PACKET BUILDER — constructs raw Ethernet/IP/UDP/XDP byte arrays
# ===========================================================================
MSG_ADD  = 0
MSG_MOD  = 1
MSG_DEL  = 2
MSG_EXEC = 3
MSG_REPL = 4

XDP_WIRE_ADD  = 100
XDP_WIRE_MOD  = 101
XDP_WIRE_DEL  = 102
XDP_WIRE_EXEC = 103
XDP_WIRE_REPL = 104

XDP_ADD_SZ  = 39
XDP_MOD_SZ  = 35
XDP_DEL_SZ  = 25
XDP_EXEC_SZ = 42


def _pack_le16(v): return struct.pack("<H", v & 0xFFFF)
def _pack_le32(v): return struct.pack("<I", v & 0xFFFFFFFF)
def _pack_le64(v): return struct.pack("<Q", v & 0xFFFFFFFFFFFFFFFF)
def _pack_be16(v): return struct.pack(">H", v & 0xFFFF)


def build_net_header(udp_payload_len: int, dst_port: int = XDP_PORT) -> bytearray:
    """42-byte Ethernet + IPv4 + UDP header."""
    ip_total = 20 + 8 + udp_payload_len
    udp_total = 8 + udp_payload_len
    pkt = bytearray(42)
    # Dst MAC
    pkt[0:6] = bytes([0x01, 0x00, 0x5e, 0x00, 0x00, 0x01])
    # Src MAC
    pkt[6:12] = bytes([0xde, 0xad, 0xbe, 0xef, 0xca, 0xfe])
    # EtherType = IPv4
    pkt[12:14] = b'\x08\x00'
    # IPv4 header
    pkt[14] = 0x45; pkt[15] = 0x00
    pkt[16:18] = _pack_be16(ip_total)
    pkt[22] = 0x40; pkt[23] = 0x11  # TTL=64, proto=UDP
    pkt[26:30] = bytes([0xef, 0x01, 0x01, 0x01])  # src IP
    pkt[30:34] = bytes([0xef, 0x01, 0x01, 0x01])  # dst IP
    # UDP
    pkt[34:36] = _pack_be16(0xCAFE)  # src port
    pkt[36:38] = _pack_be16(dst_port)
    pkt[38:40] = _pack_be16(udp_total)
    return pkt


def build_xdp_pkt_header(pkt_size: int, num_msgs: int, seq_num: int) -> bytearray:
    """16-byte XDP packet header at bytes 42-57."""
    hdr = bytearray(16)
    hdr[0:2] = _pack_le16(pkt_size)
    hdr[2]   = 11  # DeliveryFlag
    hdr[3]   = num_msgs & 0xFF
    hdr[4:8] = _pack_le32(seq_num)
    hdr[8:12]  = _pack_le32(0x5F3759DF)
    hdr[12:16] = _pack_le32(0x00001234)
    return hdr


def build_msg_common(msg_size: int, msg_type_wire: int,
                     sym: int, ssn: int, oid: int) -> bytearray:
    """Common XDP message header: MsgSize(2) MsgType(2) SourceTimeNS(4)
       SymbolIndex(4) SymbolSeqNum(4) OrderID(8) = 24 bytes."""
    buf = bytearray(24)
    buf[0:2]  = _pack_le16(msg_size)
    buf[2:4]  = _pack_le16(msg_type_wire)
    buf[4:8]  = _pack_le32(0xAAAABBBB)   # SourceTimeNS
    buf[8:12] = _pack_le32(sym)
    buf[12:16] = _pack_le32(ssn)
    buf[16:24] = _pack_le64(oid)
    return buf


def build_add_packet(sym, ssn, oid, price, qty, side_buy, seq) -> bytearray:
    udp_payload = 16 + XDP_ADD_SZ
    pkt = build_net_header(udp_payload)
    pkt += build_xdp_pkt_header(16 + XDP_ADD_SZ, 1, seq)
    msg = build_msg_common(XDP_ADD_SZ, XDP_WIRE_ADD, sym, ssn, oid)
    msg += _pack_le32(price)
    msg += _pack_le32(qty)
    msg += bytes([0x42 if side_buy else 0x53])  # 'B' or 'S'
    msg += bytearray(XDP_ADD_SZ - len(msg))
    pkt += msg
    return pkt


def build_mod_packet(sym, ssn, oid, price, qty, seq) -> bytearray:
    udp_payload = 16 + XDP_MOD_SZ
    pkt = build_net_header(udp_payload)
    pkt += build_xdp_pkt_header(16 + XDP_MOD_SZ, 1, seq)
    msg = build_msg_common(XDP_MOD_SZ, XDP_WIRE_MOD, sym, ssn, oid)
    msg += _pack_le32(price)
    msg += _pack_le32(qty)
    msg += bytearray(XDP_MOD_SZ - len(msg))
    pkt += msg
    return pkt


def build_del_packet(sym, ssn, oid, seq) -> bytearray:
    udp_payload = 16 + XDP_DEL_SZ
    pkt = build_net_header(udp_payload)
    pkt += build_xdp_pkt_header(16 + XDP_DEL_SZ, 1, seq)
    msg = build_msg_common(XDP_DEL_SZ, XDP_WIRE_DEL, sym, ssn, oid)
    msg += bytearray(XDP_DEL_SZ - len(msg))
    pkt += msg
    return pkt


def build_exec_packet(sym, ssn, oid, price, qty, seq) -> bytearray:
    udp_payload = 16 + XDP_EXEC_SZ
    pkt = build_net_header(udp_payload)
    pkt += build_xdp_pkt_header(16 + XDP_EXEC_SZ, 1, seq)
    msg = build_msg_common(XDP_EXEC_SZ, XDP_WIRE_EXEC, sym, ssn, oid)
    msg += _pack_le32(0xDEAD0001)  # TradeID
    msg += _pack_le32(price)
    msg += _pack_le32(qty)
    msg += bytes([0x59])  # Printable = 'Y'
    msg += bytearray(XDP_EXEC_SZ - len(msg))
    pkt += msg
    return pkt


# ===========================================================================
#  PYTHON-SIDE ORDER BOOK TRACKER (shadow of DUT state)
# ===========================================================================
@dataclass
class Order:
    oid: int
    symbol: int
    price: int
    qty: int
    side: bool  # True = buy

@dataclass
class BookLevel:
    price: int
    qty: int

@dataclass
class OrderBook:
    bids: List[BookLevel] = field(default_factory=list)  # sorted desc by price
    asks: List[BookLevel] = field(default_factory=list)  # sorted asc by price

    def add(self, price, qty, is_bid):
        levels = self.bids if is_bid else self.asks
        for lv in levels:
            if lv.price == price:
                lv.qty += qty
                return
        levels.append(BookLevel(price, qty))
        if is_bid:
            levels.sort(key=lambda l: -l.price)
        else:
            levels.sort(key=lambda l: l.price)
        while len(levels) > TOB_LEVELS:
            levels.pop()

    def remove(self, price, qty, is_bid):
        levels = self.bids if is_bid else self.asks
        for i, lv in enumerate(levels):
            if lv.price == price:
                lv.qty -= qty
                if lv.qty <= 0:
                    levels.pop(i)
                return

    def snapshot(self) -> dict:
        return {
            "bids": [(l.price, l.qty) for l in self.bids[:TOB_LEVELS]],
            "asks": [(l.price, l.qty) for l in self.asks[:TOB_LEVELS]],
        }


class BookTracker:
    def __init__(self):
        self.orders: Dict[int, Order] = {}
        self.books: Dict[int, OrderBook] = defaultdict(OrderBook)
        self.history: List[Tuple[int, Dict]] = []  # (cycle, {sym: snapshot})
        self._snap_interval = 50

    def apply_add(self, oid, sym, price, qty, is_bid):
        self.orders[oid] = Order(oid, sym, price, qty, is_bid)
        self.books[sym].add(price, qty, is_bid)

    def apply_mod(self, oid, new_price, new_qty):
        if oid in self.orders:
            o = self.orders[oid]
            self.books[o.symbol].remove(o.price, o.qty, o.side)
            o.price = new_price
            o.qty = new_qty
            self.books[o.symbol].add(new_price, new_qty, o.side)

    def apply_del(self, oid):
        if oid in self.orders:
            o = self.orders[oid]
            self.books[o.symbol].remove(o.price, o.qty, o.side)
            del self.orders[oid]

    def apply_exec(self, oid, exec_qty):
        if oid in self.orders:
            o = self.orders[oid]
            self.books[o.symbol].remove(o.price, exec_qty, o.side)
            o.qty -= exec_qty
            if o.qty <= 0:
                del self.orders[oid]

    def take_snapshot(self, cycle):
        snap = {}
        for sym, book in self.books.items():
            snap[sym] = book.snapshot()
        self.history.append((cycle, snap))

    def get_book(self, sym) -> dict:
        if sym in self.books:
            return self.books[sym].snapshot()
        return {"bids": [], "asks": []}

    def all_symbols(self) -> list:
        return sorted(self.books.keys())


tracker = BookTracker()

# ===========================================================================
#  MARKET DATA GENERATOR — realistic order flow for S&P 500
# ===========================================================================
class MarketDataGen:
    """Generates a stream of realistic market events."""

    def __init__(self, stocks, rng_seed=12345):
        self.stocks = stocks
        self.rng = random.Random(rng_seed)
        self.next_oid = 0x1000
        self.seq = 1
        self.ssn_per_sym = defaultdict(int)
        self.active_oids: Dict[int, dict] = {}  # oid → {sym, price, qty, side}
        self._prices = {}
        for s in stocks:
            self._prices[s["idx"]] = s["base_price"]

    def _new_oid(self):
        self.next_oid += 1
        return self.next_oid

    def _next_seq(self):
        self.seq += 1
        return self.seq

    def _ssn(self, sym):
        self.ssn_per_sym[sym] += 1
        return self.ssn_per_sym[sym]

    def gen_initial_book(self) -> List[dict]:
        """Build initial 2-sided book for every stock: 3 bid + 3 ask levels."""
        events = []
        for s in self.stocks:
            sym = s["idx"]
            base = s["base_price"]
            spread = max(base // 200, 10)  # ~0.5% spread, min 10 cents

            for i in range(TOB_LEVELS):
                bid_p = base - spread // 2 - i * spread
                ask_p = base + spread // 2 + i * spread
                bid_q = self.rng.randint(50, 500)
                ask_q = self.rng.randint(50, 500)

                bid_oid = self._new_oid()
                ask_oid = self._new_oid()

                events.append({
                    "type": MSG_ADD, "sym": sym, "oid": bid_oid,
                    "price": bid_p, "qty": bid_q, "side": True,
                    "ssn": self._ssn(sym), "seq": self._next_seq(),
                })
                self.active_oids[bid_oid] = {
                    "sym": sym, "price": bid_p, "qty": bid_q, "side": True
                }

                events.append({
                    "type": MSG_ADD, "sym": sym, "oid": ask_oid,
                    "price": ask_p, "qty": ask_q, "side": False,
                    "ssn": self._ssn(sym), "seq": self._next_seq(),
                })
                self.active_oids[ask_oid] = {
                    "sym": sym, "price": ask_p, "qty": ask_q, "side": False
                }
        return events

    def gen_update_batch(self, batch_size=10) -> List[dict]:
        """Generate a batch of random market updates."""
        events = []
        for _ in range(batch_size):
            ev = self._gen_single_update()
            if ev:
                events.append(ev)
        return events

    def gen_arb_trigger(self, component_idx=0, delta=2000) -> List[dict]:
        """Move a component's bid up sharply to trigger an arbitrage signal."""
        sym = component_idx
        events = []
        candidates = [
            oid for oid, info in self.active_oids.items()
            if info["sym"] == sym and info["side"]
        ]
        if candidates:
            oid = candidates[0]
            old = self.active_oids[oid]
            new_price = old["price"] + delta
            events.append({
                "type": MSG_MOD, "sym": sym, "oid": oid,
                "price": new_price, "qty": old["qty"], "side": old["side"],
                "ssn": self._ssn(sym), "seq": self._next_seq(),
            })
            self.active_oids[oid]["price"] = new_price
        return events

    def _gen_single_update(self) -> Optional[dict]:
        if not self.active_oids:
            return None

        action = self.rng.choices(
            ["add", "mod", "del", "exec"],
            weights=[40, 30, 15, 15]
        )[0]

        if action == "add":
            s = self.rng.choice(self.stocks)
            sym = s["idx"]
            is_bid = self.rng.random() < 0.5
            base = self._prices[sym]
            spread = max(base // 200, 10)
            if is_bid:
                price = base - self.rng.randint(spread // 2, spread * 3)
            else:
                price = base + self.rng.randint(spread // 2, spread * 3)
            qty = self.rng.choice([50, 100, 200, 300, 500])
            oid = self._new_oid()
            self.active_oids[oid] = {
                "sym": sym, "price": price, "qty": qty, "side": is_bid
            }
            return {
                "type": MSG_ADD, "sym": sym, "oid": oid,
                "price": price, "qty": qty, "side": is_bid,
                "ssn": self._ssn(sym), "seq": self._next_seq(),
            }

        elif action == "mod":
            oid = self.rng.choice(list(self.active_oids.keys()))
            old = self.active_oids[oid]
            delta = self.rng.randint(-50, 50)
            new_price = max(100, old["price"] + delta)
            new_qty = self.rng.choice([50, 100, 200, 300])
            self.active_oids[oid]["price"] = new_price
            self.active_oids[oid]["qty"] = new_qty
            return {
                "type": MSG_MOD, "sym": old["sym"], "oid": oid,
                "price": new_price, "qty": new_qty, "side": old["side"],
                "ssn": self._ssn(old["sym"]), "seq": self._next_seq(),
            }

        elif action == "del":
            oid = self.rng.choice(list(self.active_oids.keys()))
            old = self.active_oids.pop(oid)
            return {
                "type": MSG_DEL, "sym": old["sym"], "oid": oid,
                "price": old["price"], "qty": old["qty"], "side": old["side"],
                "ssn": self._ssn(old["sym"]), "seq": self._next_seq(),
            }

        elif action == "exec":
            oid = self.rng.choice(list(self.active_oids.keys()))
            old = self.active_oids[oid]
            exec_qty = min(old["qty"], self.rng.choice([25, 50, 100]))
            old["qty"] -= exec_qty
            if old["qty"] <= 0:
                self.active_oids.pop(oid)
            return {
                "type": MSG_EXEC, "sym": old["sym"], "oid": oid,
                "price": old["price"], "qty": exec_qty, "side": old["side"],
                "ssn": self._ssn(old["sym"]), "seq": self._next_seq(),
            }
        return None


# ===========================================================================
#  AXI-STREAM DRIVER — send raw packets beat by beat
# ===========================================================================
async def axis_send_packet(dut, pkt_bytes: bytearray):
    """Drive one packet onto rx_axis as 64-bit AXI-Stream beats."""
    total_bytes = len(pkt_bytes)
    beats = (total_bytes + 7) // 8

    for b in range(beats):
        offset = b * 8
        remaining = min(8, total_bytes - offset)

        data = 0
        keep = 0
        for i in range(remaining):
            data |= pkt_bytes[offset + i] << (i * 8)
            keep |= 1 << i

        dut.rx_axis_tdata.value = data
        dut.rx_axis_tkeep.value = keep
        dut.rx_axis_tvalid.value = 1
        dut.rx_axis_tlast.value = 1 if b == beats - 1 else 0

        await RisingEdge(dut.clk)
        while dut.rx_axis_tready.value == 0:
            await RisingEdge(dut.clk)

    dut.rx_axis_tvalid.value = 0
    dut.rx_axis_tlast.value = 0


def event_to_packet(ev: dict) -> bytearray:
    """Convert a market event dict to a raw XDP packet."""
    t = ev["type"]
    if t == MSG_ADD:
        return build_add_packet(
            ev["sym"], ev["ssn"], ev["oid"],
            ev["price"], ev["qty"], ev["side"], ev["seq"]
        )
    elif t == MSG_MOD:
        return build_mod_packet(
            ev["sym"], ev["ssn"], ev["oid"],
            ev["price"], ev["qty"], ev["seq"]
        )
    elif t == MSG_DEL:
        return build_del_packet(
            ev["sym"], ev["ssn"], ev["oid"], ev["seq"]
        )
    elif t == MSG_EXEC:
        return build_exec_packet(
            ev["sym"], ev["ssn"], ev["oid"],
            ev["price"], ev["qty"], ev["seq"]
        )
    raise ValueError(f"Unknown event type {t}")


MSG_NAMES = {MSG_ADD: "ADD", MSG_MOD: "MOD", MSG_DEL: "DEL",
             MSG_EXEC: "EXEC", MSG_REPL: "REPL"}


def push_input_event(ev: dict, cycle: int):
    """Push a market event to the GUI input queue."""
    ticker = TICKER_MAP.get(ev["sym"], f"?{ev['sym']}")
    try:
        input_q.put_nowait({
            "cycle": cycle,
            "type": MSG_NAMES.get(ev["type"], "?"),
            "ticker": ticker,
            "sym": ev["sym"],
            "price": ev["price"],
            "qty": ev["qty"],
            "side": "BUY" if ev.get("side") else "SELL",
        })
    except queue.Full:
        pass


# ===========================================================================
#  OUTPUT MONITORS — read packed-struct ports from DUT
# ===========================================================================
#
# tob_out_t packed layout (MSB→LSB):
#   valid(1) | symbol_idx(9) | best_bid.price(32) | best_bid.qty(32)
#            | best_ask.price(32) | best_ask.qty(32)
# Total = 138 bits
#
# trade_signal_t packed layout (MSB→LSB):
#   valid(1) | direction(1) | spread(64) | computed_index(64) | actual_price(32)
# Total = 162 bits

def read_tob(dut) -> Optional[dict]:
    """Read tob_snapshot packed struct."""
    try:
        raw = int(dut.tob_snapshot.value)
    except Exception:
        return None
    valid     = (raw >> 137) & 1
    sym_idx   = (raw >> 128) & 0x1FF
    bid_price = (raw >> 96) & 0xFFFFFFFF
    bid_qty   = (raw >> 64) & 0xFFFFFFFF
    ask_price = (raw >> 32) & 0xFFFFFFFF
    ask_qty   = raw & 0xFFFFFFFF
    if not valid:
        return None
    return {
        "sym": sym_idx,
        "bid_price": bid_price, "bid_qty": bid_qty,
        "ask_price": ask_price, "ask_qty": ask_qty,
    }


def read_trade(dut) -> Optional[dict]:
    """Read trade_signal packed struct."""
    try:
        raw = int(dut.trade_signal.value)
    except Exception:
        return None
    valid     = (raw >> 161) & 1
    direction = (raw >> 160) & 1
    spread_u  = (raw >> 96) & 0xFFFFFFFFFFFFFFFF
    comp_idx  = (raw >> 32) & 0xFFFFFFFFFFFFFFFF
    actual_p  = raw & 0xFFFFFFFF

    if not valid:
        return None

    # spread is signed Q44.20
    if spread_u >= (1 << 63):
        spread_s = spread_u - (1 << 64)
    else:
        spread_s = spread_u

    return {
        "direction": "SELL" if direction else "BUY",
        "spread": spread_s,
        "spread_dollars": spread_s / Q_ONE,
        "computed_index": comp_idx / Q_ONE,
        "actual_price": actual_p,
    }


async def output_monitor(dut, stop_event: threading.Event):
    """Background cocotb coroutine that polls DUT outputs every cycle."""
    cycle = 0
    trade_count = 0
    while not stop_event.is_set():
        await RisingEdge(dut.clk)
        cycle += 1

        # Read trade signal
        trade = read_trade(dut)
        if trade:
            trade_count += 1
            trade["number"] = trade_count
            trade["cycle"] = cycle
            try:
                trade_q.put_nowait(trade)
            except queue.Full:
                pass

        # Periodic book snapshot for GUI history slider
        if cycle % 50 == 0:
            tracker.take_snapshot(cycle)
            snap_all = {}
            for sym in tracker.all_symbols():
                snap_all[sym] = tracker.get_book(sym)
            try:
                book_q.put_nowait({"cycle": cycle, "books": snap_all})
            except queue.Full:
                pass

        # Status update
        if cycle % 20 == 0:
            try:
                oc = int(dut.order_count.value)
            except Exception:
                oc = 0
            try:
                np_raw = int(dut.net_position.value)
                if np_raw >= (1 << 31):
                    np_val = np_raw - (1 << 32)
                else:
                    np_val = np_raw
            except Exception:
                np_val = 0
            try:
                ci = int(dut.computed_index.value) / Q_ONE
            except Exception:
                ci = 0.0
            try:
                status_q.put_nowait({
                    "cycle": cycle,
                    "order_count": oc,
                    "net_position": np_val,
                    "computed_index": ci,
                    "trades": trade_count,
                })
            except queue.Full:
                pass


# ===========================================================================
#  MAIN COCOTB TEST
# ===========================================================================
@cocotb.test()
async def test_hft_pipeline(dut):
    """Full integration test: S&P 500 data → pipeline → GUI dashboard."""

    stop_event = threading.Event()

    # ------------------------------------------------------------------
    # 1. Start clock (250 MHz = 4 ns)
    # ------------------------------------------------------------------
    cocotb.start_soon(Clock(dut.clk, 4, units="ns").start())

    # ------------------------------------------------------------------
    # 2. Start GUI in background thread
    # ------------------------------------------------------------------
    gui_thread = None
    if GUI_ENABLE:
        try:
            from hft_dashboard import HFTDashboard
            dashboard = HFTDashboard(
                input_q, book_q, trade_q, status_q, control_q,
                STOCKS, TICKER_MAP, tracker
            )
            gui_thread = threading.Thread(target=dashboard.run, daemon=True)
            gui_thread.start()
            logger.info("GUI dashboard launched")
        except Exception as e:
            logger.warning(f"GUI unavailable: {e}")

    # ------------------------------------------------------------------
    # 3. Reset
    # ------------------------------------------------------------------
    dut.rst_n.value = 0
    dut.rx_axis_tdata.value = 0
    dut.rx_axis_tkeep.value = 0
    dut.rx_axis_tvalid.value = 0
    dut.rx_axis_tlast.value = 0
    dut.tx_enable.value = 1
    dut.wt_wr_en.value = 0
    dut.wt_wr_addr.value = 0
    dut.wt_wr_data.value = 0
    dut.threshold.value = 0
    dut.tx_axis_tready.value = 1

    await ClockCycles(dut.clk, 10)
    dut.rst_n.value = 1
    logger.info("Reset released")

    # ------------------------------------------------------------------
    # 4. Wait for hash table init
    # ------------------------------------------------------------------
    for _ in range(20000):
        await RisingEdge(dut.clk)
        try:
            if int(dut.ol_ready.value) == 1:
                break
        except Exception:
            pass
    logger.info("Hash table ready")

    # ------------------------------------------------------------------
    # 5. Load index weights — equal weight for all components
    # ------------------------------------------------------------------
    weight_per = Q_ONE // NUM_COMPONENTS  # 1/N in Q12.20
    for i in range(NUM_COMPONENTS):
        dut.wt_wr_en.value = 1
        dut.wt_wr_addr.value = i
        dut.wt_wr_data.value = weight_per
        await RisingEdge(dut.clk)
    dut.wt_wr_en.value = 0
    logger.info(f"Loaded {NUM_COMPONENTS} weights (each={weight_per} Q12.20)")

    # ------------------------------------------------------------------
    # 6. Set spread threshold — 100 cents ($1.00) in Q44.20
    # ------------------------------------------------------------------
    threshold_cents = 100
    threshold_q = threshold_cents * Q_ONE
    dut.threshold.value = threshold_q
    await RisingEdge(dut.clk)
    logger.info(f"Threshold set to {threshold_cents} cents ({threshold_q} Q44.20)")

    # ------------------------------------------------------------------
    # 7. Start output monitor
    # ------------------------------------------------------------------
    cocotb.start_soon(output_monitor(dut, stop_event))

    # ------------------------------------------------------------------
    # 8. Initialize order books — send ADD orders for all stocks
    # ------------------------------------------------------------------
    logger.info(f"Building initial order books for {len(STOCKS)} instruments...")
    mkt = MarketDataGen(STOCKS)
    init_events = mkt.gen_initial_book()

    cycle_count = 0
    for i, ev in enumerate(init_events):
        # Check for pause
        while True:
            try:
                cmd = control_q.get_nowait()
                if cmd == "pause":
                    while True:
                        await ClockCycles(dut.clk, 10)
                        try:
                            cmd2 = control_q.get_nowait()
                            if cmd2 in ("play", "step"):
                                break
                            if cmd2 == "quit":
                                stop_event.set()
                                return
                        except queue.Empty:
                            pass
                elif cmd == "quit":
                    stop_event.set()
                    return
            except queue.Empty:
                break

        pkt = event_to_packet(ev)
        await axis_send_packet(dut, pkt)
        await ClockCycles(dut.clk, 30)  # drain time
        cycle_count += 30

        # Update Python-side tracker
        if ev["type"] == MSG_ADD:
            tracker.apply_add(ev["oid"], ev["sym"], ev["price"],
                              ev["qty"], ev["side"])
        push_input_event(ev, cycle_count)

        # Push book update periodically
        if i % 20 == 0:
            snap_all = {}
            for sym in tracker.all_symbols():
                snap_all[sym] = tracker.get_book(sym)
            try:
                book_q.put_nowait({"cycle": cycle_count, "books": snap_all})
            except queue.Full:
                pass

        if (i + 1) % 100 == 0:
            logger.info(f"  init: {i+1}/{len(init_events)} orders sent")

    logger.info(f"Initial book built: {len(init_events)} orders")
    tracker.take_snapshot(cycle_count)

    # ------------------------------------------------------------------
    # 9. Continuous market updates + arb triggers
    # ------------------------------------------------------------------
    logger.info("Starting continuous market simulation...")
    num_rounds = 50
    paused = False

    for rnd in range(num_rounds):
        # Check controls
        try:
            cmd = control_q.get_nowait()
            if cmd == "pause":
                paused = True
            elif cmd == "play":
                paused = False
            elif cmd == "quit":
                break
        except queue.Empty:
            pass

        while paused:
            await ClockCycles(dut.clk, 10)
            try:
                cmd = control_q.get_nowait()
                if cmd in ("play", "step"):
                    paused = cmd != "step"
                    break
                elif cmd == "quit":
                    stop_event.set()
                    return
            except queue.Empty:
                pass

        # Normal market updates
        updates = mkt.gen_update_batch(batch_size=8)
        for ev in updates:
            pkt = event_to_packet(ev)
            await axis_send_packet(dut, pkt)
            await ClockCycles(dut.clk, 25)
            cycle_count += 25

            # Track
            if ev["type"] == MSG_ADD:
                tracker.apply_add(ev["oid"], ev["sym"], ev["price"],
                                  ev["qty"], ev["side"])
            elif ev["type"] == MSG_MOD:
                tracker.apply_mod(ev["oid"], ev["price"], ev["qty"])
            elif ev["type"] == MSG_DEL:
                tracker.apply_del(ev["oid"])
            elif ev["type"] == MSG_EXEC:
                tracker.apply_exec(ev["oid"], ev["qty"])

            push_input_event(ev, cycle_count)

        # Every 10 rounds, trigger an arb opportunity
        if rnd % 10 == 9 and rnd > 0:
            comp = rnd // 10 % NUM_COMPONENTS
            logger.info(f"  Triggering arb on component {comp} "
                        f"({TICKER_MAP.get(comp, '?')})")
            arb_events = mkt.gen_arb_trigger(component_idx=comp, delta=3000)
            for ev in arb_events:
                pkt = event_to_packet(ev)
                await axis_send_packet(dut, pkt)
                await ClockCycles(dut.clk, 40)
                cycle_count += 40
                tracker.apply_mod(ev["oid"], ev["price"], ev["qty"])
                push_input_event(ev, cycle_count)

        # Push book snapshots
        snap_all = {}
        for sym in tracker.all_symbols():
            snap_all[sym] = tracker.get_book(sym)
        try:
            book_q.put_nowait({"cycle": cycle_count, "books": snap_all})
        except queue.Full:
            pass

        await ClockCycles(dut.clk, 20)
        cycle_count += 20

        if (rnd + 1) % 10 == 0:
            logger.info(f"  round {rnd+1}/{num_rounds}")

    # ------------------------------------------------------------------
    # 10. Final status
    # ------------------------------------------------------------------
    await ClockCycles(dut.clk, 100)
    stop_event.set()

    try:
        oc = int(dut.order_count.value)
    except Exception:
        oc = 0
    try:
        np_raw = int(dut.net_position.value)
        np_val = np_raw - (1 << 32) if np_raw >= (1 << 31) else np_raw
    except Exception:
        np_val = 0

    logger.info("=" * 60)
    logger.info(f"  Simulation complete")
    logger.info(f"  Cycles:       {cycle_count}")
    logger.info(f"  Order count:  {oc}")
    logger.info(f"  Net position: {np_val}")
    logger.info(f"  Snapshots:    {len(tracker.history)}")
    logger.info("=" * 60)

    # Keep GUI alive a bit
    if gui_thread and gui_thread.is_alive():
        logger.info("GUI still open — close window to finish")
        for _ in range(500):
            await ClockCycles(dut.clk, 100)
            if not gui_thread.is_alive():
                break
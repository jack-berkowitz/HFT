"""
test_hft_pipeline_top.py — Cocotb testbench for hft_pipeline_top

RTL parameters (IDX_SYM, N_COMPONENTS, etc.) are overridden at compile time
via Verilator -G flags in the Makefile so they match this Python test.

Fetches live stock prices from Yahoo Finance at startup.
"""

import os, struct, random, threading, queue, time, logging
from collections import defaultdict
from dataclasses import dataclass, field
from typing import Dict, List, Optional

import cocotb
from cocotb.clock import Clock
from cocotb.triggers import RisingEdge, ClockCycles

NUM_COMPONENTS = int(os.environ.get("HFT_NUM_COMPONENTS", "20"))
GUI_ENABLE     = os.environ.get("HFT_GUI_ENABLE", "1") == "1"
IDX_SYMBOL     = NUM_COMPONENTS  # Must match Makefile -GIDX_SYM
TOB_LEVELS     = 3
XDP_PORT       = 11101
FRAC_BITS      = 20
Q_ONE          = 1 << FRAC_BITS
INTER_PKT_DELAY = float(os.environ.get("HFT_DELAY", "0.4"))

logger = logging.getLogger("hft_tb")
logger.setLevel(logging.INFO)

input_q   = queue.Queue(maxsize=5000)
book_q    = queue.Queue(maxsize=5000)
trade_q   = queue.Queue(maxsize=1000)
status_q  = queue.Queue(maxsize=500)
control_q = queue.Queue(maxsize=100)

# ===========================================================================
#  STOCK DATA — live Yahoo Finance prices with fallback
# ===========================================================================

# Full S&P 500 tickers (top ~100 by weight, rest auto-generated)
_TICKERS = [
    "AAPL","MSFT","AMZN","NVDA","GOOGL","META","BRK-B","TSLA",
    "UNH","JNJ","V","XOM","JPM","PG","MA","HD",
    "CVX","MRK","ABBV","LLY","PEP","KO","AVGO","COST",
    "MCD","WMT","TMO","CSCO","ACN","ABT","DHR","TXN",
    "NEE","PM","UNP","RTX","BMY","INTC","QCOM","AMGN",
    "HON","COP","LOW","INTU","SPGI","BA","AMAT","GE",
    "CAT","IBM","NOW","ADP","GS","BLK","SYK","MDLZ",
    "DE","LMT","ISRG","ADI","GILD","BKNG","SBUX","VRTX",
    "MMC","CI","ZTS","AXP","PYPL","PLD","REGN","CME",
    "SLB","SO","DUK","AON","TMUS","WM","CL","MO",
    "APD","GD","ICE","FDX","PNC","NSC","FCX","LRCX",
    "MET","MCK","USB","KLAC","DG","PSA","EW","PSX",
    "ORLY","GM","AEP","EMR","SHW","SNPS","CDNS","CTAS",
    "MAR","MSI","ECL","NXPI","CMG","ADSK","HCA","DXCM",
    "LULU","MNST","IDXX","AJG","KDP","MCHP","ROP","A",
    "PAYX","HSY","PCAR","AFL","TDG","BKR","MSCI","TFC",
    "AIG","COF","STZ","WMB","SPG","O","KMB","HLT",
    "CARR","GWW","SYY","FAST","OKE","CTSH","ALL","ED",
    "AZO","CPRT","TEL","TRV","BDX","MTD","BIIB","VRSK",
    "PRU","CBRE","RMD","DOW","DD","IQV","AME","FIS",
    "FTNT","AWK","KEYS","WEC","DTE","TRGP","EXR","XEL",
    "PPL","ODFL","DLTR","WBA","GPC","NUE","VICI","LEN",
    "EFX","HPQ","CF","IFF","LYB","PKI","STE","HOLX",
    "ETR","VMC","ALGN","MLM","BAX","FTV","CDW","MOH",
    "NTRS","SWKS","WAT","PHM","EPAM","AVB","CNC","DGX",
    "CAH","TYL","BR","CINF","ATO","TSCO","CLX","POOL",
    "NDAQ","KEY","MKC","BRO","JBHT","IEX","FE","NVR",
    "ESS","ARE","CPT","UDR","REG","KIM","HST","AIZ",
    "TPR","RL","HWM","BEN","LKQ","GL","TECH","GNRC",
    "LNT","EVRG","CMS","CNP","NI","AES","PNW","PEAK",
    "BXP","VTR","OGN","DVA","BBWI","CTLT","SEE","AAL",
    "ALK","WYNN","MHK","NWL","CZR","NWSA","DISH","FRT",
    "BWA","HII","WHR","PNR","FFIV","LW","CE","TAP",
    "IVZ","ZION","ALLE","HAS","XRAY","RHI","BIO",
]

# Fallback prices (cents) for top stocks if yfinance unavailable
_FALLBACK = {
    "AAPL":19200,"MSFT":42500,"AMZN":18700,"NVDA":88000,"GOOGL":17500,
    "META":50500,"BRK-B":41000,"TSLA":25500,"UNH":52800,"JNJ":15800,
    "V":27500,"XOM":11700,"JPM":19800,"PG":16500,"MA":45200,"HD":36800,
    "CVX":16200,"MRK":12500,"ABBV":17400,"LLY":78000,"PEP":18200,"KO":6400,
    "AVGO":17000,"COST":74000,"MCD":29500,"WMT":17800,"TMO":55000,"CSCO":5200,
    "ACN":33500,"ABT":11200,"DHR":25600,"TXN":17900,"NEE":7900,"PM":10500,
    "UNP":24800,"RTX":10200,"BMY":5200,"INTC":3100,"QCOM":17500,"AMGN":28000,
    "HON":20800,"COP":11600,"LOW":24500,"INTU":62000,"SPGI":44500,"BA":19000,
    "AMAT":16000,"GE":16800,"CAT":29500,"IBM":19000,"NOW":70000,"ADP":25500,
    "GS":47500,"BLK":78000,"SYK":34500,"MDLZ":7600,"DE":39000,"LMT":46000,
    "ISRG":40000,"ADI":22000,"GILD":8600,"BKNG":38000,"SBUX":10000,"VRTX":44000,
    "MMC":20000,"CI":35000,"ZTS":18500,"AXP":22500,"PYPL":7200,"PLD":13000,
    "REGN":80000,"CME":21000,"SLB":5400,"SO":8500,"DUK":10600,"AON":34000,
    "TMUS":18000,"WM":20000,"CL":9100,"MO":5100,"APD":29000,"GD":28500,
    "ICE":14000,"FDX":28000,"PNC":17500,"NSC":25000,"FCX":4500,"LRCX":78000,
    "MET":7800,"MCK":58000,"USB":4800,"KLAC":68000,"DG":14000,"PSA":31000,
    "EW":7800,"PSX":14000,"ORLY":95000,"GM":4500,"AEP":9800,"EMR":11000,
}

def _fetch_live_prices(tickers):
    try:
        import yfinance as yf
        logger.info(f"Fetching live prices for {len(tickers)+1} stocks...")
        all_t = list(tickers) + ["SPY"]
        data = yf.download(all_t, period="1d", interval="1d", progress=False, threads=True)
        prices = {}
        if 'Close' in data.columns or hasattr(data, 'columns'):
            close = data['Close']
            if hasattr(close, 'columns'):
                for t in all_t:
                    if t in close.columns:
                        v = close[t].dropna()
                        if len(v) > 0: prices[t] = int(float(v.iloc[-1]) * 100)
            else:
                v = close.dropna()
                if len(v) > 0: prices[all_t[0]] = int(float(v.iloc[-1]) * 100)
        logger.info(f"  Got {len(prices)} live prices")
        return prices
    except ImportError:
        logger.warning("yfinance not installed — using fallback prices")
        return {}
    except Exception as e:
        logger.warning(f"Yahoo Finance failed: {e}")
        return {}

def build_stock_list(n):
    live = _fetch_live_prices(_TICKERS[:n])
    stocks = []; rng = random.Random(42)
    for i in range(n):
        t = _TICKERS[i] if i < len(_TICKERS) else f"SYM{i:03d}"
        base = live.get(t, _FALLBACK.get(t, rng.randint(2000, 60000)))
        stocks.append({"ticker": t.replace("-", "."), "idx": i, "base_price": base})
    spy_p = live.get("SPY", 52500)
    stocks.append({"ticker": "SPY", "idx": IDX_SYMBOL, "base_price": spy_p})
    for s in stocks[:5]:
        src = "LIVE" if s["ticker"].replace(".", "-") in live else "fallback"
        logger.info(f"  {s['ticker']:>6} = ${s['base_price']/100:.2f} ({src})")
    logger.info(f"  {'SPY':>6} = ${spy_p/100:.2f} (idx sym={IDX_SYMBOL})")
    return stocks

STOCKS = build_stock_list(NUM_COMPONENTS)
TICKER_MAP = {s["idx"]: s["ticker"] for s in STOCKS}

# ===========================================================================
#  PACKET BUILDER
# ===========================================================================
MSG_ADD=0; MSG_MOD=1; MSG_DEL=2; MSG_EXEC=3
def _le16(v): return struct.pack("<H",v&0xFFFF)
def _le32(v): return struct.pack("<I",v&0xFFFFFFFF)
def _le64(v): return struct.pack("<Q",v&0xFFFFFFFFFFFFFFFF)
def _be16(v): return struct.pack(">H",v&0xFFFF)

def build_net_hdr(udp_pl, port=XDP_PORT):
    p=bytearray(42)
    p[0:6]=bytes([1,0,0x5e,0,0,1]);p[6:12]=bytes([0xde,0xad,0xbe,0xef,0xca,0xfe])
    p[12:14]=b'\x08\x00';p[14]=0x45;p[16:18]=_be16(20+8+udp_pl)
    p[22]=0x40;p[23]=0x11;p[26:30]=bytes([0xef,1,1,1]);p[30:34]=bytes([0xef,1,1,1])
    p[34:36]=_be16(0xCAFE);p[36:38]=_be16(port);p[38:40]=_be16(8+udp_pl)
    return p
def build_xdp_hdr(sz,nm,seq):
    h=bytearray(16);h[0:2]=_le16(sz);h[2]=11;h[3]=nm&0xFF
    h[4:8]=_le32(seq);h[8:12]=_le32(0x5F3759DF);h[12:16]=_le32(0x1234);return h
def build_msg_cmn(msz,wtype,sym,ssn,oid):
    b=bytearray(24);b[0:2]=_le16(msz);b[2:4]=_le16(wtype)
    b[4:8]=_le32(0xAAAABBBB);b[8:12]=_le32(sym);b[12:16]=_le32(ssn);b[16:24]=_le64(oid);return b

def build_add(sym,ssn,oid,price,qty,buy,seq):
    SZ=39;p=build_net_hdr(16+SZ);p+=build_xdp_hdr(16+SZ,1,seq)
    m=build_msg_cmn(SZ,100,sym,ssn,oid);m+=_le32(price)+_le32(qty)+bytes([0x42 if buy else 0x53])
    m+=bytearray(SZ-len(m));p+=m;return p
def build_mod(sym,ssn,oid,price,qty,seq):
    SZ=35;p=build_net_hdr(16+SZ);p+=build_xdp_hdr(16+SZ,1,seq)
    m=build_msg_cmn(SZ,101,sym,ssn,oid);m+=_le32(price)+_le32(qty)
    m+=bytearray(SZ-len(m));p+=m;return p
def build_del(sym,ssn,oid,seq):
    SZ=25;p=build_net_hdr(16+SZ);p+=build_xdp_hdr(16+SZ,1,seq)
    m=build_msg_cmn(SZ,102,sym,ssn,oid);m+=bytearray(SZ-len(m));p+=m;return p
def build_exec(sym,ssn,oid,price,qty,seq):
    SZ=42;p=build_net_hdr(16+SZ);p+=build_xdp_hdr(16+SZ,1,seq)
    m=build_msg_cmn(SZ,103,sym,ssn,oid);m+=_le32(0xDEAD0001)+_le32(price)+_le32(qty)+bytes([0x59])
    m+=bytearray(SZ-len(m));p+=m;return p

# ===========================================================================
#  BOOK TRACKER
# ===========================================================================
@dataclass
class BL:
    price:int; qty:int

class OrderBook:
    def __init__(self): self.bids=[]; self.asks=[]
    def add(self,price,qty,bid):
        ls=self.bids if bid else self.asks
        for l in ls:
            if l.price==price: l.qty+=qty; return
        ls.append(BL(price,qty))
        if bid: ls.sort(key=lambda x:-x.price)
        else: ls.sort(key=lambda x:x.price)
        while len(ls)>TOB_LEVELS: ls.pop()
    def remove(self,price,qty,bid):
        ls=self.bids if bid else self.asks
        for i,l in enumerate(ls):
            if l.price==price: l.qty-=qty; (ls.pop(i) if l.qty<=0 else None); return
    def snap(self):
        return {"bids":[(l.price,l.qty) for l in self.bids],"asks":[(l.price,l.qty) for l in self.asks]}

class Tracker:
    def __init__(self): self.orders={}; self.books=defaultdict(OrderBook)
    def add(self,oid,sym,p,q,bid):
        self.orders[oid]={"s":sym,"p":p,"q":q,"b":bid}; self.books[sym].add(p,q,bid)
    def mod(self,oid,np,nq):
        if oid not in self.orders: return
        o=self.orders[oid]; self.books[o["s"]].remove(o["p"],o["q"],o["b"])
        o["p"]=np; o["q"]=nq; self.books[o["s"]].add(np,nq,o["b"])
    def delete(self,oid):
        if oid not in self.orders: return
        o=self.orders.pop(oid); self.books[o["s"]].remove(o["p"],o["q"],o["b"])
    def exec(self,oid,eq):
        if oid not in self.orders: return
        o=self.orders[oid]; self.books[o["s"]].remove(o["p"],eq,o["b"])
        o["q"]-=eq
        if o["q"]<=0: del self.orders[oid]
    def get(self,sym): return self.books[sym].snap() if sym in self.books else {"bids":[],"asks":[]}
    def all(self): return {s:self.books[s].snap() for s in self.books}

tracker = Tracker()

# ===========================================================================
#  MARKET DATA GEN
# ===========================================================================
class MktGen:
    def __init__(self,stocks,seed=12345):
        self.stocks=stocks;self.rng=random.Random(seed)
        self._oid=0x1000;self._seq=1;self._ssn=defaultdict(int);self.active={}
        self._prices={s["idx"]:s["base_price"] for s in stocks}
    def oid(self): self._oid+=1; return self._oid
    def seq(self): self._seq+=1; return self._seq
    def ssn(self,s): self._ssn[s]+=1; return self._ssn[s]

    def init_book(self):
        evs=[]
        for s in self.stocks:
            sym=s["idx"];base=s["base_price"];sp=max(base//200,10)
            for i in range(TOB_LEVELS):
                for buy in [True,False]:
                    o=self.oid();p=base+(-sp//2-i*sp if buy else sp//2+i*sp)
                    q=self.rng.randint(50,500)
                    evs.append({"type":MSG_ADD,"sym":sym,"oid":o,"price":p,"qty":q,
                                "side":buy,"ssn":self.ssn(sym),"seq":self.seq()})
                    self.active[o]={"sym":sym,"price":p,"qty":q,"side":buy}
        return evs

    def gen_one(self):
        if not self.active: return None
        act=self.rng.choices(["add","mod","del","exec"],weights=[40,30,15,15])[0]
        if act=="add":
            s=self.rng.choice(self.stocks);sym=s["idx"];buy=self.rng.random()<0.5
            base=self._prices[sym];sp=max(base//200,10)
            p=base+(self.rng.randint(-sp*3,-sp//2) if buy else self.rng.randint(sp//2,sp*3))
            q=self.rng.choice([50,100,200,300,500]);o=self.oid()
            self.active[o]={"sym":sym,"price":p,"qty":q,"side":buy}
            return {"type":MSG_ADD,"sym":sym,"oid":o,"price":p,"qty":q,"side":buy,
                    "ssn":self.ssn(sym),"seq":self.seq()}
        elif act=="mod":
            o=self.rng.choice(list(self.active.keys()));old=self.active[o]
            np=max(100,old["price"]+self.rng.randint(-50,50));nq=self.rng.choice([50,100,200,300])
            self.active[o]["price"]=np;self.active[o]["qty"]=nq
            return {"type":MSG_MOD,"sym":old["sym"],"oid":o,"price":np,"qty":nq,
                    "side":old["side"],"ssn":self.ssn(old["sym"]),"seq":self.seq()}
        elif act=="del":
            o=self.rng.choice(list(self.active.keys()));old=self.active.pop(o)
            return {"type":MSG_DEL,"sym":old["sym"],"oid":o,"price":old["price"],"qty":old["qty"],
                    "side":old["side"],"ssn":self.ssn(old["sym"]),"seq":self.seq()}
        elif act=="exec":
            o=self.rng.choice(list(self.active.keys()));old=self.active[o]
            eq=min(old["qty"],self.rng.choice([25,50,100]));old["qty"]-=eq
            if old["qty"]<=0: self.active.pop(o)
            return {"type":MSG_EXEC,"sym":old["sym"],"oid":o,"price":old["price"],"qty":eq,
                    "side":old["side"],"ssn":self.ssn(old["sym"]),"seq":self.seq()}

    def arb_trigger(self,comp=0,delta=3000):
        cands=[o for o,i in self.active.items() if i["sym"]==comp and i["side"]]
        if not cands: return []
        o=cands[0];old=self.active[o];np=old["price"]+delta
        self.active[o]["price"]=np
        return [{"type":MSG_MOD,"sym":comp,"oid":o,"price":np,"qty":old["qty"],
                 "side":old["side"],"ssn":self.ssn(comp),"seq":self.seq()}]

# ===========================================================================
#  AXI-STREAM + HELPERS
# ===========================================================================
async def axis_send(dut,pkt):
    n=len(pkt);beats=(n+7)//8
    for b in range(beats):
        off=b*8;rem=min(8,n-off);data=0;keep=0
        for i in range(rem): data|=pkt[off+i]<<(i*8); keep|=1<<i
        dut.rx_axis_tdata.value=data;dut.rx_axis_tkeep.value=keep
        dut.rx_axis_tvalid.value=1;dut.rx_axis_tlast.value=(1 if b==beats-1 else 0)
        await RisingEdge(dut.clk)
        while dut.rx_axis_tready.value==0: await RisingEdge(dut.clk)
    dut.rx_axis_tvalid.value=0;dut.rx_axis_tlast.value=0

def ev_to_pkt(ev):
    t=ev["type"]
    if t==MSG_ADD:  return build_add(ev["sym"],ev["ssn"],ev["oid"],ev["price"],ev["qty"],ev["side"],ev["seq"])
    if t==MSG_MOD:  return build_mod(ev["sym"],ev["ssn"],ev["oid"],ev["price"],ev["qty"],ev["seq"])
    if t==MSG_DEL:  return build_del(ev["sym"],ev["ssn"],ev["oid"],ev["seq"])
    if t==MSG_EXEC: return build_exec(ev["sym"],ev["ssn"],ev["oid"],ev["price"],ev["qty"],ev["seq"])

MSG_NAMES={MSG_ADD:"ADD",MSG_MOD:"MOD",MSG_DEL:"DEL",MSG_EXEC:"EXEC"}

def push_input(ev,cycle):
    try: input_q.put_nowait({"cycle":cycle,"type":MSG_NAMES.get(ev["type"],"?"),
        "ticker":TICKER_MAP.get(ev["sym"],"?"),"sym":ev["sym"],
        "price":ev["price"],"qty":ev["qty"],"side":"BUY" if ev.get("side") else "SELL"})
    except: pass

def push_books(cycle,active_sym=None):
    try: book_q.put_nowait({"cycle":cycle,"books":tracker.all(),"active_sym":active_sym})
    except: pass

# ===========================================================================
#  TX MONITOR
# ===========================================================================
async def tx_monitor(dut,stop_event):
    tc=0;cyc=0;pkt=bytearray()
    while not stop_event.is_set():
        await RisingEdge(dut.clk);cyc+=1
        try: tv=int(dut.tx_axis_tvalid.value);tl=int(dut.tx_axis_tlast.value)
        except: continue
        if tv:
            try: td=int(dut.tx_axis_tdata.value);tk=int(dut.tx_axis_tkeep.value)
            except: continue
            for i in range(8):
                if tk&(1<<i): pkt.append((td>>(i*8))&0xFF)
            if tl:
                tc+=1;d="?";ap=0;sd=0.0
                if len(pkt)>=74:
                    d="BUY" if pkt[44]==0x42 else "SELL"
                    ap=struct.unpack_from("<I",pkt,50)[0]
                    sd=struct.unpack_from("<q",pkt,66)[0]/Q_ONE
                logger.info(f"  >>> TX TRADE #{tc}: {d} price=${ap/100:.2f} spread=${sd:.2f}")
                try: trade_q.put_nowait({"number":tc,"direction":d,"spread_dollars":sd,
                    "computed_index":0.0,"actual_price":ap,"cycle":cyc})
                except: pass
                pkt=bytearray()
        if cyc%500==0:
            try: oc=int(dut.order_count.value)
            except: oc=0
            try: nr=int(dut.net_position.value);nv=nr-(1<<32) if nr>=(1<<31) else nr
            except: nv=0
            try: ci=int(dut.computed_index.value)/Q_ONE
            except: ci=0.0
            try: status_q.put_nowait({"cycle":cyc,"order_count":oc,"net_position":nv,
                                      "computed_index":ci,"trades":tc})
            except: pass

# ===========================================================================
async def check_pause(dut):
    try:
        cmd=control_q.get_nowait()
        if cmd=="quit": return True
        if cmd=="pause":
            logger.info("  PAUSED")
            while True:
                await ClockCycles(dut.clk,100)
                try:
                    c=control_q.get_nowait()
                    if c in ("play","step"): break
                    if c=="quit": return True
                except: pass
    except: pass
    return False

async def send_event(dut,ev,cycle):
    pkt=ev_to_pkt(ev)
    await axis_send(dut,pkt)
    await ClockCycles(dut.clk,80);cycle[0]+=80
    t=ev["type"]
    if t==MSG_ADD: tracker.add(ev["oid"],ev["sym"],ev["price"],ev["qty"],ev["side"])
    elif t==MSG_MOD: tracker.mod(ev["oid"],ev["price"],ev["qty"])
    elif t==MSG_DEL: tracker.delete(ev["oid"])
    elif t==MSG_EXEC: tracker.exec(ev["oid"],ev["qty"])
    push_input(ev,cycle[0]);push_books(cycle[0],active_sym=ev["sym"])
    if INTER_PKT_DELAY>0:
        for _ in range(int(INTER_PKT_DELAY/0.01)):
            await ClockCycles(dut.clk,50);cycle[0]+=50;time.sleep(0.01)

# ===========================================================================
#  MAIN TEST
# ===========================================================================
@cocotb.test()
async def test_hft_pipeline(dut):
    stop_event=threading.Event()
    cocotb.start_soon(Clock(dut.clk,4,units="ns").start())

    # GUI
    gui_thread=None
    if GUI_ENABLE:
        try:
            from hft_dashboard import HFTDashboard
            dashboard=HFTDashboard(input_q,book_q,trade_q,status_q,control_q,STOCKS,TICKER_MAP,tracker)
            gui_thread=threading.Thread(target=dashboard.run,daemon=True)
            gui_thread.start();logger.info("Dashboard launched");time.sleep(1.5)
        except Exception as e: logger.warning(f"GUI unavailable: {e}")

    # Reset
    dut.rst_n.value=0;dut.rx_axis_tdata.value=0;dut.rx_axis_tkeep.value=0
    dut.rx_axis_tvalid.value=0;dut.rx_axis_tlast.value=0;dut.tx_enable.value=1
    dut.wt_wr_en.value=0;dut.wt_wr_addr.value=0;dut.wt_wr_data.value=0
    dut.threshold.value=0;dut.tx_axis_tready.value=1
    await ClockCycles(dut.clk,10);dut.rst_n.value=1
    logger.info("Reset released")

    for _ in range(20000):
        await RisingEdge(dut.clk)
        try:
            if int(dut.ol_ready.value)==1: break
        except: pass
    logger.info("Hash table ready")

    # Price-calibrated weights: w = SPY_base * Q_ONE / SUM(component_base_prices)
    # Ensures SUM(price_i * w_i) == SPY_price * Q_ONE at startup -> spread starts near 0.
    # Arb pushes (positive delta) create positive spread -> SELL.
    # Price drops create negative spread -> BUY. Both directions fire.
    spy_stock = next(s for s in STOCKS if s["idx"] == IDX_SYMBOL)
    spy_base  = spy_stock["base_price"]
    comp_stocks = [s for s in STOCKS if s["idx"] < NUM_COMPONENTS]
    sum_base  = sum(s["base_price"] for s in comp_stocks)
    w = (spy_base * Q_ONE) // sum_base
    logger.info(f"Price-calibrated weights: SPY={spy_base}c sum_comp={sum_base}c w={w} (Q_ONE={Q_ONE})")
    for s in comp_stocks:
        dut.wt_wr_en.value=1; dut.wt_wr_addr.value=s["idx"]; dut.wt_wr_data.value=w
        await RisingEdge(dut.clk)
    dut.wt_wr_en.value=0

    # Threshold — 25 cents
    thresh=25*Q_ONE
    dut.threshold.value=thresh
    await RisingEdge(dut.clk)
    logger.info(f"Threshold=25c ({thresh} Q44.20)")
    logger.info(f"IDX_SYMBOL={IDX_SYMBOL} (must match RTL IDX_SYM via Makefile -G)")

    cocotb.start_soon(tx_monitor(dut,stop_event))

    # Build initial books
    logger.info(f"Building books for {len(STOCKS)} instruments...")
    mkt=MktGen(STOCKS);init=mkt.init_book();cycle=[0]
    for i,ev in enumerate(init):
        await axis_send(dut,ev_to_pkt(ev))
        await ClockCycles(dut.clk,60);cycle[0]+=60
        tracker.add(ev["oid"],ev["sym"],ev["price"],ev["qty"],ev["side"])
        if i%20==0: push_books(cycle[0],ev["sym"]); push_input(ev,cycle[0])
        if (i+1)%50==0: logger.info(f"  init: {i+1}/{len(init)}")
    push_books(cycle[0]);await ClockCycles(dut.clk,200);cycle[0]+=200
    logger.info(f"Book init done: {len(init)} orders")

    # Probe after init
    try:
        oc=int(dut.order_count.value);ci=int(dut.computed_index.value)/Q_ONE
        logger.info(f"  POST-INIT: order_count={oc}, computed_index={ci:.2f} cents (${ci/100:.2f})")
    except: pass

    # Continuous simulation
    logger.info(f"Live simulation running (delay={INTER_PKT_DELAY}s)...")
    rnd=0; arb_every=3

    # SPY-bid arb: mean-reverting random walk drives natural BUY/SELL signals.
    # Every arb_every rounds, nudge SPY best bid by (revert_toward_center + noise).
    # Reversion prevents runaway drift; noise makes direction genuinely random.
    _spy_base    = next(s for s in STOCKS if s["idx"]==IDX_SYMBOL)["base_price"]
    _spy_sp      = max(_spy_base//200, 10)
    _spy_bid_ctr = _spy_base - _spy_sp//2          # init_book level-0 bid center
    _spy_bid_max = _spy_bid_ctr + _spy_sp//2 - 20  # stay 20c below ask
    _spy_bid_cur = _spy_bid_ctr                    # tracks current best-bid price
    _spy_noise   = max(120, _spy_sp//2)            # noise amplitude (>threshold/2)

    while True:
        if await check_pause(dut): break
        if gui_thread and not gui_thread.is_alive(): logger.info("Browser closed"); break

        ev=mkt.gen_one()
        if not ev: continue
        await send_event(dut,ev,cycle)
        rnd+=1

        if rnd%arb_every==0:
            spy_bids=[(o,i["price"]) for o,i in mkt.active.items()
                      if i["sym"]==IDX_SYMBOL and i["side"]]
            if spy_bids:
                spy_oid=max(spy_bids,key=lambda x:x[1])[0]
                # Mean-reverting random walk: 25% pull back to center + uniform noise
                revert  = int(0.25*(_spy_bid_ctr - _spy_bid_cur))
                noise   = random.randint(-_spy_noise, _spy_noise)
                spy_delta = revert + noise
                _spy_bid_cur = max(100, min(_spy_bid_max, _spy_bid_cur + spy_delta))
                mkt.active[spy_oid]["price"] = _spy_bid_cur
                spy_ev={"type":MSG_MOD,"sym":IDX_SYMBOL,"oid":spy_oid,
                        "price":_spy_bid_cur,"qty":mkt.active[spy_oid]["qty"],"side":True,
                        "ssn":mkt.ssn(IDX_SYMBOL),"seq":mkt.seq()}
                logger.info(f"  #{rnd}: SPY bid {spy_delta:+d}c (cur={_spy_bid_cur})")
                await send_event(dut,spy_ev,cycle)
            try:
                oc=int(dut.order_count.value);ci=int(dut.computed_index.value)/Q_ONE
                logger.info(f"    order_count={oc}, idx={ci:.2f}c (${ci/100:.2f})")
            except: pass

        if rnd%25==0: logger.info(f"  #{rnd} cycle={cycle[0]}")

    stop_event.set();await ClockCycles(dut.clk,200)
    try: oc=int(dut.order_count.value)
    except: oc=0
    logger.info("="*50)
    logger.info(f"  Done. Events={rnd}, DUT orders={oc}")
    logger.info("="*50)
"""
hft_dashboard.py — Dark-themed HFT Pipeline Trading Dashboard

Professional trading terminal UI showing:
  1. Live market feed (scrolling input messages)
  2. S&P 500 index order book
  3. Slideable order book viewer for any symbol
  4. Trade signal output blotter
  5. Pause / Play / Step controls
"""

import tkinter as tk
from tkinter import ttk
import queue
import time
import math
from typing import Dict, List, Optional

# ═══════════════════════════════════════════════════════════════════════
# COLOUR PALETTE — dark trading terminal aesthetic
# ═══════════════════════════════════════════════════════════════════════
C = {
    "bg_dark":     "#0a0e17",
    "bg_panel":    "#111827",
    "bg_card":     "#1a2332",
    "bg_input":    "#0f1a2e",
    "border":      "#2a3a52",
    "border_lite": "#1e2d42",
    "text":        "#e2e8f0",
    "text_dim":    "#64748b",
    "text_muted":  "#475569",
    "bid_green":   "#22c55e",
    "bid_bg":      "#052e16",
    "bid_bar":     "#15803d",
    "ask_red":     "#ef4444",
    "ask_bg":      "#2a0a0a",
    "ask_bar":     "#991b1b",
    "accent":      "#3b82f6",
    "accent_dim":  "#1e3a5f",
    "warn":        "#f59e0b",
    "header":      "#0f172a",
    "sell_badge":  "#7f1d1d",
    "buy_badge":   "#14532d",
}

FONT_MONO   = ("JetBrains Mono", "Consolas", "Menlo", "monospace")
FONT_SANS   = ("Segoe UI", "SF Pro Display", "Helvetica Neue", "sans-serif")
FONT_HEADER = ("Segoe UI Semibold", "SF Pro Display", "Helvetica Neue")


def _mono(size=11):
    return (FONT_MONO[0], size)

def _sans(size=11, bold=False):
    w = "bold" if bold else "normal"
    return (FONT_SANS[0], size, w)


class HFTDashboard:
    """Main dashboard window — call run() from a background thread."""

    def __init__(self, input_q, book_q, trade_q, status_q, control_q,
                 stocks, ticker_map, tracker):
        self.input_q   = input_q
        self.book_q    = book_q
        self.trade_q   = trade_q
        self.status_q  = status_q
        self.control_q = control_q
        self.stocks    = stocks
        self.ticker_map = ticker_map
        self.tracker   = tracker

        self.paused = False
        self.current_books = {}
        self.history_snaps = []
        self.selected_sym = 0
        self.idx_sym = max(s["idx"] for s in stocks)

        self.root = tk.Tk()
        self.root.title("HFT Pipeline — Trading Dashboard")
        self.root.configure(bg=C["bg_dark"])
        self.root.geometry("1400x900")
        self.root.minsize(1100, 700)
        self.root.protocol("WM_DELETE_WINDOW", self._on_close)

        self._setup_styles()
        self._build_ui()

    # ══════════════════════════════════════════════════════════════════
    # THEME & STYLES
    # ══════════════════════════════════════════════════════════════════
    def _setup_styles(self):
        style = ttk.Style(self.root)
        style.theme_use("clam")
        style.configure(".", background=C["bg_dark"], foreground=C["text"],
                         borderwidth=0, font=_sans(11))
        style.configure("TFrame", background=C["bg_dark"])
        style.configure("Card.TFrame", background=C["bg_panel"])
        style.configure("TLabel", background=C["bg_dark"],
                         foreground=C["text"], font=_sans(11))
        style.configure("Header.TLabel", background=C["header"],
                         foreground=C["text"], font=_sans(13, bold=True))
        style.configure("Dim.TLabel", foreground=C["text_dim"], font=_sans(10))
        style.configure("Stat.TLabel", foreground=C["accent"],
                         font=_mono(13), background=C["header"])
        style.configure("Title.TLabel", foreground=C["text"],
                         font=_sans(12, bold=True), background=C["bg_panel"])
        style.configure("TButton", background=C["bg_card"],
                         foreground=C["text"], font=_sans(11, bold=True),
                         padding=(12, 6))
        style.map("TButton",
                   background=[("active", C["accent_dim"])],
                   foreground=[("active", C["text"])])
        style.configure("Play.TButton", foreground=C["bid_green"])
        style.configure("Pause.TButton", foreground=C["warn"])
        style.configure("TCombobox", fieldbackground=C["bg_card"],
                         background=C["bg_card"], foreground=C["text"],
                         selectbackground=C["accent_dim"],
                         font=_mono(11))
        style.configure("Horizontal.TScale", background=C["bg_dark"],
                         troughcolor=C["bg_card"])

    # ══════════════════════════════════════════════════════════════════
    # BUILD UI
    # ══════════════════════════════════════════════════════════════════
    def _build_ui(self):
        # ── Header bar ──
        hdr = tk.Frame(self.root, bg=C["header"], height=56)
        hdr.pack(fill="x", side="top")
        hdr.pack_propagate(False)

        tk.Label(hdr, text="  \u26A1  HFT PIPELINE SIMULATOR",
                 font=(FONT_HEADER[0], 15, "bold"),
                 bg=C["header"], fg=C["accent"]).pack(side="left", padx=8)

        # Status labels
        self._lbl_cycle = tk.Label(hdr, text="CYCLE 0", font=_mono(11),
                                    bg=C["header"], fg=C["text_dim"])
        self._lbl_cycle.pack(side="left", padx=20)
        self._lbl_orders = tk.Label(hdr, text="ORDERS 0", font=_mono(11),
                                     bg=C["header"], fg=C["text_dim"])
        self._lbl_orders.pack(side="left", padx=10)
        self._lbl_trades = tk.Label(hdr, text="TRADES 0", font=_mono(11),
                                     bg=C["header"], fg=C["warn"])
        self._lbl_trades.pack(side="left", padx=10)
        self._lbl_pos = tk.Label(hdr, text="POS 0", font=_mono(11),
                                  bg=C["header"], fg=C["text_dim"])
        self._lbl_pos.pack(side="left", padx=10)
        self._lbl_index = tk.Label(hdr, text="IDX $0.00", font=_mono(11),
                                    bg=C["header"], fg=C["bid_green"])
        self._lbl_index.pack(side="left", padx=10)

        # Controls
        ctrl = tk.Frame(hdr, bg=C["header"])
        ctrl.pack(side="right", padx=12)
        self._btn_play = ttk.Button(ctrl, text="\u25B6  Play",
                                     style="Play.TButton",
                                     command=self._on_play)
        self._btn_play.pack(side="left", padx=4)
        self._btn_pause = ttk.Button(ctrl, text="\u23F8  Pause",
                                      style="Pause.TButton",
                                      command=self._on_pause)
        self._btn_pause.pack(side="left", padx=4)
        self._btn_step = ttk.Button(ctrl, text="\u23ED  Step",
                                     command=self._on_step)
        self._btn_step.pack(side="left", padx=4)

        # ── Main body: 2×2 grid ──
        body = tk.Frame(self.root, bg=C["bg_dark"])
        body.pack(fill="both", expand=True, padx=8, pady=8)
        body.columnconfigure(0, weight=2)
        body.columnconfigure(1, weight=3)
        body.rowconfigure(0, weight=1)
        body.rowconfigure(1, weight=1)

        # Panel 1: Live Inputs (top-left)
        self._build_input_panel(body, 0, 0)
        # Panel 2: Order Book Viewer (top-right)
        self._build_book_panel(body, 0, 1)
        # Panel 3: Trade Output (bottom-left)
        self._build_trade_panel(body, 1, 0)
        # Panel 4: S&P 500 Index Book (bottom-right)
        self._build_index_panel(body, 1, 1)

    # ── Panel builders ──

    def _panel_frame(self, parent, row, col, title, icon=""):
        outer = tk.Frame(parent, bg=C["bg_panel"], highlightbackground=C["border"],
                          highlightthickness=1)
        outer.grid(row=row, column=col, sticky="nsew", padx=4, pady=4)
        outer.columnconfigure(0, weight=1)
        outer.rowconfigure(1, weight=1)

        hdr = tk.Frame(outer, bg=C["bg_panel"], height=32)
        hdr.grid(row=0, column=0, sticky="ew", padx=8, pady=(8, 4))
        tk.Label(hdr, text=f"{icon}  {title}", font=_sans(12, bold=True),
                 bg=C["bg_panel"], fg=C["accent"]).pack(side="left")
        return outer, hdr

    def _build_input_panel(self, parent, row, col):
        frame, hdr = self._panel_frame(parent, row, col,
                                         "LIVE MARKET FEED", "\U0001F4CA")
        # Scrollable text
        txt = tk.Text(frame, bg=C["bg_input"], fg=C["text"], font=_mono(10),
                       wrap="none", bd=0, highlightthickness=0,
                       insertbackground=C["text"], padx=8, pady=4,
                       cursor="arrow", state="disabled")
        sb = tk.Scrollbar(frame, command=txt.yview, bg=C["bg_card"],
                           troughcolor=C["bg_dark"], width=8)
        txt.configure(yscrollcommand=sb.set)
        txt.grid(row=1, column=0, sticky="nsew", padx=8, pady=(0, 8))
        sb.grid(row=1, column=1, sticky="ns", pady=(0, 8))
        frame.columnconfigure(0, weight=1)

        txt.tag_configure("buy", foreground=C["bid_green"])
        txt.tag_configure("sell", foreground=C["ask_red"])
        txt.tag_configure("time", foreground=C["text_muted"])
        txt.tag_configure("mod", foreground=C["warn"])
        txt.tag_configure("exec", foreground="#a78bfa")
        txt.tag_configure("del", foreground=C["text_dim"])
        self._input_txt = txt

    def _build_book_panel(self, parent, row, col):
        frame, hdr = self._panel_frame(parent, row, col,
                                         "ORDER BOOK VIEWER", "\U0001F4D6")

        # Symbol selector
        sel_frame = tk.Frame(hdr, bg=C["bg_panel"])
        sel_frame.pack(side="right")
        tk.Label(sel_frame, text="Symbol:", font=_sans(10),
                 bg=C["bg_panel"], fg=C["text_dim"]).pack(side="left", padx=4)

        sym_options = [f"{s['ticker']} ({s['idx']})" for s in self.stocks]
        self._sym_var = tk.StringVar(value=sym_options[0] if sym_options else "")
        combo = ttk.Combobox(sel_frame, textvariable=self._sym_var,
                              values=sym_options, state="readonly", width=18)
        combo.pack(side="left", padx=4)
        combo.bind("<<ComboboxSelected>>", self._on_sym_change)

        # History slider
        sl_frame = tk.Frame(frame, bg=C["bg_panel"])
        sl_frame.grid(row=2, column=0, columnspan=2, sticky="ew", padx=8, pady=(0, 4))
        tk.Label(sl_frame, text="History:", font=_sans(9),
                 bg=C["bg_panel"], fg=C["text_muted"]).pack(side="left")
        self._hist_slider = tk.Scale(sl_frame, from_=0, to=0,
                                      orient="horizontal", bg=C["bg_panel"],
                                      fg=C["text_dim"], troughcolor=C["bg_card"],
                                      highlightthickness=0, bd=0,
                                      sliderrelief="flat", font=_mono(8),
                                      command=self._on_hist_slide)
        self._hist_slider.pack(side="left", fill="x", expand=True, padx=8)
        self._lbl_snap = tk.Label(sl_frame, text="LIVE", font=_mono(9),
                                   bg=C["bg_panel"], fg=C["warn"])
        self._lbl_snap.pack(side="right")

        # Book canvas
        self._book_canvas = tk.Canvas(frame, bg=C["bg_input"], bd=0,
                                       highlightthickness=0)
        self._book_canvas.grid(row=1, column=0, columnspan=2,
                                sticky="nsew", padx=8, pady=4)
        frame.columnconfigure(0, weight=1)

    def _build_trade_panel(self, parent, row, col):
        frame, hdr = self._panel_frame(parent, row, col,
                                         "TRADE SIGNALS", "\U0001F3AF")
        txt = tk.Text(frame, bg=C["bg_input"], fg=C["text"], font=_mono(10),
                       wrap="none", bd=0, highlightthickness=0,
                       padx=8, pady=4, cursor="arrow", state="disabled")
        sb = tk.Scrollbar(frame, command=txt.yview, bg=C["bg_card"],
                           troughcolor=C["bg_dark"], width=8)
        txt.configure(yscrollcommand=sb.set)
        txt.grid(row=1, column=0, sticky="nsew", padx=8, pady=(0, 8))
        sb.grid(row=1, column=1, sticky="ns", pady=(0, 8))
        frame.columnconfigure(0, weight=1)

        txt.tag_configure("sell", foreground=C["ask_red"])
        txt.tag_configure("buy", foreground=C["bid_green"])
        txt.tag_configure("hdr", foreground=C["text_muted"])
        txt.tag_configure("spread", foreground=C["warn"])

        # Insert column header
        txt.configure(state="normal")
        txt.insert("end",
            f" {'#':>4}  {'DIR':^5}  {'SPREAD':>12}  {'COMPUTED':>12}  "
            f"{'ACTUAL':>10}  {'CYCLE':>8}\n", "hdr")
        txt.insert("end", " " + "\u2500" * 68 + "\n", "hdr")
        txt.configure(state="disabled")
        self._trade_txt = txt

    def _build_index_panel(self, parent, row, col):
        frame, hdr = self._panel_frame(parent, row, col,
                                         "S&P 500 INDEX BOOK", "\U0001F4C8")

        # Index stats
        stats = tk.Frame(frame, bg=C["bg_panel"])
        stats.grid(row=1, column=0, sticky="ew", padx=12, pady=4)

        self._lbl_comp = tk.Label(stats, text="Computed: --",
                                   font=_mono(12), bg=C["bg_panel"],
                                   fg=C["bid_green"])
        self._lbl_comp.pack(side="left", padx=(0, 20))
        self._lbl_actual = tk.Label(stats, text="Actual: --",
                                     font=_mono(12), bg=C["bg_panel"],
                                     fg=C["text"])
        self._lbl_actual.pack(side="left", padx=(0, 20))
        self._lbl_spread = tk.Label(stats, text="Spread: --",
                                     font=_mono(12), bg=C["bg_panel"],
                                     fg=C["warn"])
        self._lbl_spread.pack(side="left")

        # Index book canvas
        self._idx_canvas = tk.Canvas(frame, bg=C["bg_input"], bd=0,
                                      highlightthickness=0)
        self._idx_canvas.grid(row=2, column=0, sticky="nsew",
                               padx=8, pady=(4, 8))
        frame.columnconfigure(0, weight=1)
        frame.rowconfigure(2, weight=1)

    # ══════════════════════════════════════════════════════════════════
    # ORDER BOOK RENDERING — price ladder with quantity bars
    # ══════════════════════════════════════════════════════════════════
    def _draw_book(self, canvas, book_data, title=""):
        canvas.delete("all")
        w = canvas.winfo_width()
        h = canvas.winfo_height()
        if w < 20 or h < 20:
            return

        bids = book_data.get("bids", [])
        asks = book_data.get("asks", [])

        mid_x = w // 2
        row_h = 32
        pad_y = 10
        col_w = mid_x - 20

        # Find max qty for bar scaling
        all_qty = [q for _, q in bids] + [q for _, q in asks]
        max_qty = max(all_qty) if all_qty else 1

        # Column headers
        canvas.create_text(mid_x - col_w // 2, pad_y,
                            text="QTY        BID", font=_mono(9),
                            fill=C["text_muted"], anchor="n")
        canvas.create_text(mid_x + col_w // 2, pad_y,
                            text="ASK        QTY", font=_mono(9),
                            fill=C["text_muted"], anchor="n")

        # Divider line
        canvas.create_line(mid_x, pad_y + 4, mid_x, h - 4,
                            fill=C["border"], width=1, dash=(2, 4))

        y_start = pad_y + 22
        n_rows = max(len(bids), len(asks), 1)

        for i in range(n_rows):
            y = y_start + i * row_h
            if y + row_h > h:
                break

            # Bid side (left)
            if i < len(bids):
                bp, bq = bids[i]
                bar_w = max(4, int((bq / max_qty) * (col_w - 90)))
                # Bar (grows left from center)
                canvas.create_rectangle(
                    mid_x - 8 - bar_w, y + 4,
                    mid_x - 8, y + row_h - 4,
                    fill=C["bid_bg"], outline=""
                )
                canvas.create_rectangle(
                    mid_x - 8 - bar_w, y + 4,
                    mid_x - 8, y + row_h - 4,
                    fill="", outline=C["bid_bar"], width=1
                )
                # Price
                canvas.create_text(mid_x - 14, y + row_h // 2,
                                    text=f"${bp/100:.2f}",
                                    font=_mono(11), fill=C["bid_green"],
                                    anchor="e")
                # Qty
                canvas.create_text(mid_x - 8 - bar_w - 6, y + row_h // 2,
                                    text=str(bq), font=_mono(10),
                                    fill=C["text_dim"], anchor="e")

            # Ask side (right)
            if i < len(asks):
                ap, aq = asks[i]
                bar_w = max(4, int((aq / max_qty) * (col_w - 90)))
                canvas.create_rectangle(
                    mid_x + 8, y + 4,
                    mid_x + 8 + bar_w, y + row_h - 4,
                    fill=C["ask_bg"], outline=""
                )
                canvas.create_rectangle(
                    mid_x + 8, y + 4,
                    mid_x + 8 + bar_w, y + row_h - 4,
                    fill="", outline=C["ask_bar"], width=1
                )
                canvas.create_text(mid_x + 14, y + row_h // 2,
                                    text=f"${ap/100:.2f}",
                                    font=_mono(11), fill=C["ask_red"],
                                    anchor="w")
                canvas.create_text(mid_x + 8 + bar_w + 6, y + row_h // 2,
                                    text=str(aq), font=_mono(10),
                                    fill=C["text_dim"], anchor="w")

        # Mid-price label
        if bids and asks:
            mid = (bids[0][0] + asks[0][0]) / 2
            canvas.create_text(mid_x, h - 8,
                                text=f"mid ${mid/100:.2f}",
                                font=_mono(9), fill=C["text_muted"],
                                anchor="s")

    # ══════════════════════════════════════════════════════════════════
    # CALLBACKS
    # ══════════════════════════════════════════════════════════════════
    def _on_play(self):
        self.paused = False
        self.control_q.put("play")

    def _on_pause(self):
        self.paused = True
        self.control_q.put("pause")

    def _on_step(self):
        self.control_q.put("step")

    def _on_close(self):
        self.control_q.put("quit")
        self.root.destroy()

    def _on_sym_change(self, event=None):
        val = self._sym_var.get()
        try:
            idx = int(val.split("(")[-1].rstrip(")"))
            self.selected_sym = idx
        except (ValueError, IndexError):
            pass
        self._refresh_book_display()

    def _on_hist_slide(self, val):
        idx = int(float(val))
        if idx < len(self.history_snaps):
            cyc, snap = self.history_snaps[idx]
            self._lbl_snap.config(text=f"CYC {cyc}")
            if self.selected_sym in snap:
                self._draw_book(self._book_canvas, snap[self.selected_sym])
        elif self.history_snaps:
            self._lbl_snap.config(text="LIVE")
            self._refresh_book_display()

    def _refresh_book_display(self):
        book = self.current_books.get(self.selected_sym,
                                       {"bids": [], "asks": []})
        self._draw_book(self._book_canvas, book)

    # ══════════════════════════════════════════════════════════════════
    # QUEUE POLLING — called every 40ms
    # ══════════════════════════════════════════════════════════════════
    def _poll(self):
        # Process input events
        count = 0
        while count < 40:
            try:
                ev = self.input_q.get_nowait()
                count += 1
                self._append_input(ev)
            except queue.Empty:
                break

        # Process book updates
        try:
            while True:
                bk = self.book_q.get_nowait()
                self.current_books = bk.get("books", {})
                cyc = bk.get("cycle", 0)
                self.history_snaps.append((cyc, dict(self.current_books)))
                self._hist_slider.config(to=max(0, len(self.history_snaps) - 1))
        except queue.Empty:
            pass

        # Process trade signals
        try:
            while True:
                tr = self.trade_q.get_nowait()
                self._append_trade(tr)
        except queue.Empty:
            pass

        # Process status
        try:
            while True:
                st = self.status_q.get_nowait()
                self._update_status(st)
        except queue.Empty:
            pass

        # Refresh book display
        self._refresh_book_display()
        self._refresh_index_display()

        self.root.after(40, self._poll)

    def _append_input(self, ev):
        txt = self._input_txt
        txt.configure(state="normal")

        cyc = ev.get("cycle", 0)
        mtype = ev.get("type", "?")
        ticker = ev.get("ticker", "?")
        price = ev.get("price", 0)
        qty = ev.get("qty", 0)
        side = ev.get("side", "?")

        tag_map = {"ADD": "buy" if side == "BUY" else "sell",
                   "MOD": "mod", "DEL": "del", "EXEC": "exec"}
        tag = tag_map.get(mtype, "time")

        line = (f" {cyc:>7}  {mtype:<5} {ticker:<8} "
                f"{'B' if side == 'BUY' else 'S'} "
                f"${price/100:>9.2f} x {qty:<5}\n")
        txt.insert("end", line, tag)
        txt.see("end")

        # Keep buffer bounded
        lines = int(txt.index("end-1c").split(".")[0])
        if lines > 500:
            txt.delete("1.0", f"{lines - 400}.0")

        txt.configure(state="disabled")

    def _append_trade(self, tr):
        txt = self._trade_txt
        txt.configure(state="normal")

        num  = tr.get("number", 0)
        dire = tr.get("direction", "?")
        sprd = tr.get("spread_dollars", 0)
        comp = tr.get("computed_index", 0)
        act  = tr.get("actual_price", 0)
        cyc  = tr.get("cycle", 0)
        tag  = "sell" if dire == "SELL" else "buy"

        line = (f" {num:>4}  {dire:^5}  ${sprd:>+10.2f}  "
                f"${comp:>10.2f}  ${act/100:>8.2f}  {cyc:>8}\n")
        txt.insert("end", line, tag)
        txt.see("end")
        txt.configure(state="disabled")

    def _update_status(self, st):
        cyc = st.get("cycle", 0)
        oc  = st.get("order_count", 0)
        np  = st.get("net_position", 0)
        ci  = st.get("computed_index", 0)
        tc  = st.get("trades", 0)

        self._lbl_cycle.config(text=f"CYCLE {cyc:,}")
        self._lbl_orders.config(text=f"ORDERS {oc}")
        self._lbl_trades.config(text=f"TRADES {tc}")
        pos_color = C["ask_red"] if np < 0 else C["bid_green"] if np > 0 else C["text_dim"]
        self._lbl_pos.config(text=f"POS {np:+d}", fg=pos_color)
        self._lbl_index.config(text=f"IDX ${ci:,.2f}")

    def _refresh_index_display(self):
        idx_book = self.current_books.get(self.idx_sym,
                                           {"bids": [], "asks": []})
        self._draw_book(self._idx_canvas, idx_book)

        # Update index stats from latest status
        bids = idx_book.get("bids", [])
        asks = idx_book.get("asks", [])
        if bids:
            self._lbl_actual.config(text=f"Best bid: ${bids[0][0]/100:.2f}")
        if asks:
            self._lbl_spread.config(text=f"Best ask: ${asks[0][0]/100:.2f}")

    # ══════════════════════════════════════════════════════════════════
    # RUN — starts the tkinter main loop (call from thread)
    # ══════════════════════════════════════════════════════════════════
    def run(self):
        # Delay first poll slightly so widgets are mapped
        self.root.after(200, self._poll)
        try:
            self.root.mainloop()
        except Exception:
            pass


# ═══════════════════════════════════════════════════════════════════════
# STANDALONE MODE — run the GUI with fake data for UI testing
# ═══════════════════════════════════════════════════════════════════════
if __name__ == "__main__":
    import threading, random, time as _time

    inp_q = queue.Queue(5000)
    bk_q  = queue.Queue(5000)
    tr_q  = queue.Queue(1000)
    st_q  = queue.Queue(500)
    ctl_q = queue.Queue(100)

    # Fake stocks
    fake_stocks = [
        {"ticker": "AAPL", "idx": 0, "base_price": 19200},
        {"ticker": "MSFT", "idx": 1, "base_price": 42500},
        {"ticker": "AMZN", "idx": 2, "base_price": 18700},
        {"ticker": "NVDA", "idx": 3, "base_price": 88000},
        {"ticker": "GOOGL","idx": 4, "base_price": 17500},
        {"ticker": "SPY",  "idx": 5, "base_price": 52500},
    ]
    ticker_map = {s["idx"]: s["ticker"] for s in fake_stocks}

    class FakeTracker:
        def all_symbols(self): return list(range(6))
        def get_book(self, sym): return {"bids": [], "asks": []}
        history = []

    def fake_data_pump():
        rng = random.Random(99)
        cyc = 0
        trade_n = 0
        while True:
            _time.sleep(0.15)
            cyc += 50
            s = rng.choice(fake_stocks)
            side = rng.choice(["BUY", "SELL"])
            tp = rng.choice(["ADD", "MOD", "DEL", "EXEC"])
            try:
                inp_q.put_nowait({
                    "cycle": cyc, "type": tp, "ticker": s["ticker"],
                    "sym": s["idx"], "price": s["base_price"] + rng.randint(-200, 200),
                    "qty": rng.choice([50, 100, 200]), "side": side,
                })
            except queue.Full:
                pass

            # Fake books
            books = {}
            for fs in fake_stocks:
                bp = fs["base_price"]
                books[fs["idx"]] = {
                    "bids": [(bp - 50 - i*30, rng.randint(50, 400)) for i in range(3)],
                    "asks": [(bp + 50 + i*30, rng.randint(50, 400)) for i in range(3)],
                }
            try:
                bk_q.put_nowait({"cycle": cyc, "books": books})
            except queue.Full:
                pass

            if rng.random() < 0.15:
                trade_n += 1
                d = rng.choice(["BUY", "SELL"])
                try:
                    tr_q.put_nowait({
                        "number": trade_n, "direction": d,
                        "spread_dollars": rng.uniform(-3, 3),
                        "computed_index": 525.0 + rng.uniform(-5, 5),
                        "actual_price": 52500 + rng.randint(-200, 200),
                        "cycle": cyc,
                    })
                except queue.Full:
                    pass

            try:
                st_q.put_nowait({
                    "cycle": cyc, "order_count": cyc // 50,
                    "net_position": rng.randint(-500, 500),
                    "computed_index": 525.0 + rng.uniform(-2, 2),
                    "trades": trade_n,
                })
            except queue.Full:
                pass

    pump = threading.Thread(target=fake_data_pump, daemon=True)
    pump.start()

    dash = HFTDashboard(inp_q, bk_q, tr_q, st_q, ctl_q,
                         fake_stocks, ticker_map, FakeTracker())
    dash.run()
"""
hft_dashboard.py — Browser-based HFT Pipeline Trading Dashboard

Opens a dark-themed trading terminal in your default browser.
Works from a background thread (no tkinter main-thread restriction).

Panels:
  1. Live market feed (scrolling input messages)
  2. Order book viewer with symbol selector
  3. S&P 500 index order book
  4. Trade signal output blotter
  5. Pause / Play controls + status bar
"""

import http.server
import json
import queue
import threading
import webbrowser
import time
import socket

_state = {
    "inputs": [],
    "books": {},
    "trades": [],
    "status": {},
    "history": [],
    "stocks": [],
}
_state_lock = threading.Lock()
_control_q = None

MAX_HISTORY = 500

HTML_PAGE = r"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>HFT Pipeline — Trading Dashboard</title>
<style>
  @import url('https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;600&family=Inter:wght@400;500;600;700&display=swap');
  * { margin: 0; padding: 0; box-sizing: border-box; }
  :root {
    --bg-dark: #0a0e17; --bg-panel: #111827; --bg-card: #1a2332;
    --bg-input: #0d1520; --border: #2a3a52; --text: #e2e8f0;
    --text-dim: #64748b; --text-muted: #475569;
    --bid: #22c55e; --bid-bar: rgba(34,197,94,0.25);
    --ask: #ef4444; --ask-bar: rgba(239,68,68,0.25);
    --accent: #3b82f6; --accent-dim: #1e3a5f;
    --warn: #f59e0b; --purple: #a78bfa;
    --mono: 'JetBrains Mono', 'Consolas', monospace;
    --sans: 'Inter', -apple-system, sans-serif;
  }
  body { background: var(--bg-dark); color: var(--text); font-family: var(--sans); font-size: 13px; overflow: hidden; height: 100vh; }

  .header { background: #0f172a; height: 52px; display: flex; align-items: center; padding: 0 20px; border-bottom: 1px solid var(--border); gap: 24px; }
  .header .logo { font-size: 15px; font-weight: 700; color: var(--accent); letter-spacing: 0.5px; }
  .header .logo span { color: var(--warn); }
  .stat { font-family: var(--mono); font-size: 12px; color: var(--text-dim); }
  .stat b { color: var(--text); font-weight: 600; }
  .stat.green b { color: var(--bid); }
  .stat.amber b { color: var(--warn); }
  .stat.red b { color: var(--ask); }
  .controls { margin-left: auto; display: flex; gap: 8px; }
  .btn { font-family: var(--sans); font-size: 12px; font-weight: 600; padding: 6px 16px; border: 1px solid var(--border); border-radius: 6px; background: var(--bg-card); color: var(--text); cursor: pointer; transition: all 0.15s; }
  .btn:hover { background: var(--accent-dim); border-color: var(--accent); }
  .btn.play { color: var(--bid); }
  .btn.pause { color: var(--warn); }

  .grid { display: grid; grid-template-columns: 1fr 1.4fr; grid-template-rows: 1fr 1fr; gap: 6px; padding: 6px; height: calc(100vh - 52px); }
  .panel { background: var(--bg-panel); border: 1px solid var(--border); border-radius: 8px; display: flex; flex-direction: column; overflow: hidden; }
  .panel-header { padding: 10px 14px; font-size: 12px; font-weight: 600; color: var(--accent); border-bottom: 1px solid var(--border); display: flex; align-items: center; gap: 10px; flex-shrink: 0; }
  .panel-header .icon { font-size: 15px; }
  .panel-body { flex: 1; overflow-y: auto; padding: 0; background: var(--bg-input); }
  .panel-body::-webkit-scrollbar { width: 6px; }
  .panel-body::-webkit-scrollbar-track { background: transparent; }
  .panel-body::-webkit-scrollbar-thumb { background: var(--border); border-radius: 3px; }

  .feed-line { font-family: var(--mono); font-size: 11px; padding: 2px 12px; white-space: nowrap; border-bottom: 1px solid rgba(42,58,82,0.3); }
  .feed-line.buy { color: var(--bid); }
  .feed-line.sell { color: var(--ask); }
  .feed-line.mod { color: var(--warn); }
  .feed-line.exec { color: var(--purple); }
  .feed-line.del { color: var(--text-dim); }

  .book-controls { padding: 8px 14px; display: flex; align-items: center; gap: 12px; border-bottom: 1px solid var(--border); flex-shrink: 0; }
  .book-controls select { background: var(--bg-card); color: var(--text); border: 1px solid var(--border); border-radius: 4px; padding: 4px 8px; font-family: var(--mono); font-size: 11px; }
  .book-controls input[type=range] { flex: 1; accent-color: var(--accent); }
  .book-controls label { font-size: 11px; color: var(--text-dim); }
  .book-controls .snap-label { font-family: var(--mono); font-size: 10px; color: var(--warn); min-width: 70px; text-align: right; }

  .book-table { width: 100%; border-collapse: collapse; }
  .book-table th { font-size: 10px; color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.5px; padding: 6px 12px; border-bottom: 1px solid var(--border); position: sticky; top: 0; background: var(--bg-input); }
  .book-table th:nth-child(1), .book-table th:nth-child(2) { text-align: right; }
  .book-table th:nth-child(4), .book-table th:nth-child(5) { text-align: left; }
  .book-table th:nth-child(3) { text-align: center; }
  .book-row td { font-family: var(--mono); font-size: 12px; padding: 5px 12px; }
  .book-row .bid-price { color: var(--bid); text-align: right; font-weight: 600; }
  .book-row .bid-qty { color: var(--text-dim); text-align: right; }
  .book-row .spacer { width: 20px; }
  .book-row .ask-price { color: var(--ask); text-align: left; font-weight: 600; }
  .book-row .ask-qty { color: var(--text-dim); text-align: left; }
  .mid-label { text-align: center; font-size: 10px; color: var(--text-muted); padding: 6px; font-family: var(--mono); }

  .trade-header { font-family: var(--mono); font-size: 10px; color: var(--text-muted); padding: 6px 12px; border-bottom: 1px solid var(--border); background: var(--bg-input); position: sticky; top: 0; white-space: pre; }
  .trade-line { font-family: var(--mono); font-size: 11px; padding: 3px 12px; border-bottom: 1px solid rgba(42,58,82,0.3); white-space: pre; }
  .trade-line.sell { color: var(--ask); }
  .trade-line.buy { color: var(--bid); }

  .idx-stats { padding: 8px 14px; display: flex; gap: 24px; font-family: var(--mono); font-size: 13px; border-bottom: 1px solid var(--border); flex-shrink: 0; }
  .idx-stats .computed { color: var(--bid); }
  .idx-stats .actual { color: var(--text); }
  .idx-stats .spread { color: var(--warn); }
</style>
</head>
<body>
<div class="header">
  <div class="logo">&#9889; HFT PIPELINE <span>SIMULATOR</span></div>
  <div class="stat" id="s-cycle">CYCLE <b>0</b></div>
  <div class="stat" id="s-orders">ORDERS <b>0</b></div>
  <div class="stat amber" id="s-trades">TRADES <b>0</b></div>
  <div class="stat" id="s-pos">POS <b>0</b></div>
  <div class="stat green" id="s-idx">IDX <b>$0.00</b></div>
  <div class="controls">
    <button class="btn play" onclick="sendCmd('play')">&#9654; Play</button>
    <button class="btn pause" onclick="sendCmd('pause')">&#9208; Pause</button>
  </div>
</div>
<div class="grid">
  <div class="panel">
    <div class="panel-header"><span class="icon">&#128202;</span> LIVE MARKET FEED</div>
    <div class="panel-body" id="feed"></div>
  </div>
  <div class="panel">
    <div class="panel-header"><span class="icon">&#128214;</span> ORDER BOOK VIEWER</div>
    <div class="book-controls">
      <label>Symbol:</label>
      <select id="sym-select"></select>
      <label>History:</label>
      <input type="range" id="hist-slider" min="0" max="0" value="0" oninput="onHistSlide()">
      <span class="snap-label" id="snap-label">LIVE</span>
    </div>
    <div class="panel-body" id="book-view"></div>
  </div>
  <div class="panel">
    <div class="panel-header"><span class="icon">&#127919;</span> TRADE SIGNALS</div>
    <div class="panel-body" id="trades">
      <div class="trade-header">  #    DIR      SPREAD      COMPUTED      ACTUAL      CYCLE</div>
    </div>
  </div>
  <div class="panel">
    <div class="panel-header"><span class="icon">&#128200;</span> S&amp;P 500 INDEX BOOK</div>
    <div class="idx-stats">
      <span class="computed" id="idx-comp">Computed: --</span>
      <span class="actual" id="idx-actual">Best bid: --</span>
      <span class="spread" id="idx-spread">Best ask: --</span>
    </div>
    <div class="panel-body" id="idx-book"></div>
  </div>
</div>
<script>
const POLL_MS = 120;
let allBooks = {}, historySnaps = [], selectedSym = 0, idxSym = 0, stockList = [];
function $(id) { return document.getElementById(id); }

async function poll() {
  try {
    const r = await fetch('/api/state');
    const d = await r.json();
    if (d.stocks && d.stocks.length && !stockList.length) {
      stockList = d.stocks;
      idxSym = d.stocks[d.stocks.length - 1].idx;
      const sel = $('sym-select');
      sel.innerHTML = '';
      d.stocks.forEach(s => { const o = document.createElement('option'); o.value = s.idx; o.textContent = s.ticker + ' (' + s.idx + ')'; sel.appendChild(o); });
      selectedSym = d.stocks[0].idx;
      sel.onchange = function() { selectedSym = parseInt(this.value); renderBook(); };
    }
    if (d.inputs && d.inputs.length) {
      const feed = $('feed');
      d.inputs.forEach(ev => {
        const div = document.createElement('div');
        const tag = ev.type==='ADD' ? (ev.side==='BUY' ? 'buy' : 'sell') : ev.type==='MOD' ? 'mod' : ev.type==='EXEC' ? 'exec' : 'del';
        div.className = 'feed-line ' + tag;
        div.textContent = `${String(ev.cycle).padStart(7)}  ${ev.type.padEnd(5)} ${ev.ticker.padEnd(8)} ${ev.side==='BUY'?'B':'S'}  $${(ev.price/100).toFixed(2).padStart(9)} x ${String(ev.qty).padEnd(5)}`;
        feed.appendChild(div);
      });
      while (feed.children.length > 400) feed.removeChild(feed.firstChild);
      feed.scrollTop = feed.scrollHeight;
    }
    if (d.books && Object.keys(d.books).length) allBooks = d.books;
    if (d.history && d.history.length) { historySnaps = d.history; $('hist-slider').max = Math.max(0, historySnaps.length - 1); }
    if (d.trades && d.trades.length) {
      const tc = $('trades');
      d.trades.forEach(t => {
        const div = document.createElement('div');
        div.className = 'trade-line ' + t.direction.toLowerCase();
        div.textContent = `${String(t.number).padStart(4)}   ${t.direction.padStart(4)}   $${t.spread_dollars.toFixed(2).padStart(10)}   $${t.computed_index.toFixed(2).padStart(10)}   $${(t.actual_price/100).toFixed(2).padStart(8)}   ${String(t.cycle).padStart(8)}`;
        tc.appendChild(div);
      });
      tc.scrollTop = tc.scrollHeight;
    }
    if (d.status && d.status.cycle) {
      const st = d.status;
      $('s-cycle').innerHTML = 'CYCLE <b>' + st.cycle.toLocaleString() + '</b>';
      $('s-orders').innerHTML = 'ORDERS <b>' + st.order_count + '</b>';
      $('s-trades').innerHTML = 'TRADES <b>' + st.trades + '</b>';
      const np = st.net_position;
      $('s-pos').className = 'stat ' + (np>0?'green':np<0?'red':'');
      $('s-pos').innerHTML = 'POS <b>' + (np>=0?'+':'') + np + '</b>';
      $('s-idx').innerHTML = 'IDX <b>$' + st.computed_index.toFixed(2) + '</b>';
    }
    renderBook(); renderIdxBook();
  } catch(e) {}
  setTimeout(poll, POLL_MS);
}

function renderBookTable(container, bookData) {
  if (!bookData) { container.innerHTML = '<div class="mid-label">No data</div>'; return; }
  const bids = bookData.bids || [], asks = bookData.asks || [];
  const n = Math.max(bids.length, asks.length, 1);
  let html = '<table class="book-table"><thead><tr><th>Qty</th><th>Bid</th><th></th><th>Ask</th><th>Qty</th></tr></thead><tbody>';
  for (let i = 0; i < n; i++) {
    html += '<tr class="book-row">';
    html += i < bids.length ? `<td class="bid-qty">${bids[i][1]}</td><td class="bid-price">$${(bids[i][0]/100).toFixed(2)}</td>` : '<td></td><td></td>';
    html += '<td class="spacer"></td>';
    html += i < asks.length ? `<td class="ask-price">$${(asks[i][0]/100).toFixed(2)}</td><td class="ask-qty">${asks[i][1]}</td>` : '<td></td><td></td>';
    html += '</tr>';
  }
  html += '</tbody></table>';
  if (bids.length && asks.length) html += `<div class="mid-label">mid $${((bids[0][0]+asks[0][0])/200).toFixed(2)}  &middot;  spread $${((asks[0][0]-bids[0][0])/100).toFixed(2)}</div>`;
  container.innerHTML = html;
}

function renderBook() {
  const idx = parseInt($('hist-slider').value);
  let bd;
  if (idx < historySnaps.length - 1) { const s = historySnaps[idx]; $('snap-label').textContent = 'CYC ' + s[0]; bd = s[1][selectedSym]; }
  else { $('snap-label').textContent = 'LIVE'; bd = allBooks[selectedSym]; }
  renderBookTable($('book-view'), bd);
}

function renderIdxBook() {
  const bd = allBooks[idxSym];
  renderBookTable($('idx-book'), bd);
  if (bd) {
    if (bd.bids && bd.bids.length) $('idx-actual').textContent = 'Best bid: $' + (bd.bids[0][0]/100).toFixed(2);
    if (bd.asks && bd.asks.length) $('idx-spread').textContent = 'Best ask: $' + (bd.asks[0][0]/100).toFixed(2);
  }
}

function onHistSlide() { renderBook(); }
async function sendCmd(cmd) { try { await fetch('/api/control?cmd='+cmd); } catch(e) {} }
poll();
</script>
</body>
</html>"""


class DashboardHandler(http.server.BaseHTTPRequestHandler):
    def log_message(self, fmt, *args): pass

    def do_GET(self):
        if self.path == '/' or self.path == '/index.html':
            self.send_response(200)
            self.send_header('Content-Type', 'text/html')
            self.end_headers()
            self.wfile.write(HTML_PAGE.encode())
        elif self.path == '/api/state':
            self.send_response(200)
            self.send_header('Content-Type', 'application/json')
            self.send_header('Cache-Control', 'no-cache')
            self.end_headers()
            with _state_lock:
                payload = {"inputs": list(_state["inputs"]), "books": dict(_state["books"]),
                           "trades": list(_state["trades"]), "status": dict(_state["status"]),
                           "history": list(_state["history"]), "stocks": _state["stocks"]}
                _state["inputs"].clear()
                _state["trades"].clear()
            self.wfile.write(json.dumps(payload).encode())
        elif self.path.startswith('/api/control'):
            cmd = self.path.split('cmd=')[-1] if 'cmd=' in self.path else ''
            if _control_q and cmd:
                try: _control_q.put_nowait(cmd)
                except queue.Full: pass
            self.send_response(200)
            self.send_header('Content-Type', 'text/plain')
            self.end_headers()
            self.wfile.write(b'ok')
        else:
            self.send_response(404)
            self.end_headers()


def _find_free_port():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(('', 0))
        return s.getsockname()[1]


class HFTDashboard:
    def __init__(self, input_q, book_q, trade_q, status_q, control_q,
                 stocks, ticker_map, tracker):
        global _control_q
        self.input_q = input_q
        self.book_q = book_q
        self.trade_q = trade_q
        self.status_q = status_q
        self.control_q = control_q
        _control_q = control_q
        self.tracker = tracker
        self.port = _find_free_port()
        self._running = True
        with _state_lock:
            _state["stocks"] = stocks

    def run(self):
        server = http.server.HTTPServer(('127.0.0.1', self.port), DashboardHandler)
        server.timeout = 0.1
        srv_t = threading.Thread(target=self._serve, args=(server,), daemon=True)
        srv_t.start()
        url = f'http://127.0.0.1:{self.port}'
        print(f"\n  ⚡ Dashboard: {url}\n")
        webbrowser.open(url)
        while self._running:
            self._drain_queues()
            time.sleep(0.05)
        server.shutdown()

    def _serve(self, server):
        while self._running:
            server.handle_request()

    def stop(self):
        self._running = False

    def _drain_queues(self):
        batch = []
        for _ in range(50):
            try: batch.append(self.input_q.get_nowait())
            except queue.Empty: break
        if batch:
            with _state_lock: _state["inputs"].extend(batch)
        try:
            while True:
                bk = self.book_q.get_nowait()
                with _state_lock:
                    _state["books"] = bk.get("books", {})
                    _state["history"].append((bk.get("cycle", 0), dict(_state["books"])))
                    if len(_state["history"]) > MAX_HISTORY:
                        _state["history"] = _state["history"][-MAX_HISTORY:]
        except queue.Empty: pass
        tb = []
        try:
            while True: tb.append(self.trade_q.get_nowait())
        except queue.Empty: pass
        if tb:
            with _state_lock: _state["trades"].extend(tb)
        try:
            while True:
                st = self.status_q.get_nowait()
                with _state_lock: _state["status"] = st
        except queue.Empty: pass


if __name__ == "__main__":
    import random as _rng

    inp_q, bk_q, tr_q, st_q, ctl_q = queue.Queue(5000), queue.Queue(5000), queue.Queue(1000), queue.Queue(500), queue.Queue(100)
    fake_stocks = [{"ticker": "AAPL", "idx": 0, "base_price": 19200}, {"ticker": "MSFT", "idx": 1, "base_price": 42500},
                   {"ticker": "AMZN", "idx": 2, "base_price": 18700}, {"ticker": "NVDA", "idx": 3, "base_price": 88000},
                   {"ticker": "GOOGL", "idx": 4, "base_price": 17500}, {"ticker": "SPY", "idx": 5, "base_price": 52500}]
    class FakeTracker:
        history = []
    def pump():
        r = _rng.Random(99); cyc = 0; tn = 0
        while True:
            time.sleep(0.2); cyc += 50; s = r.choice(fake_stocks)
            try: inp_q.put_nowait({"cycle": cyc, "type": r.choice(["ADD","MOD","DEL","EXEC"]), "ticker": s["ticker"], "sym": s["idx"], "price": s["base_price"]+r.randint(-200,200), "qty": r.choice([50,100,200]), "side": r.choice(["BUY","SELL"])})
            except queue.Full: pass
            books = {fs["idx"]: {"bids": [(fs["base_price"]-50-i*30, r.randint(50,400)) for i in range(3)], "asks": [(fs["base_price"]+50+i*30, r.randint(50,400)) for i in range(3)]} for fs in fake_stocks}
            try: bk_q.put_nowait({"cycle": cyc, "books": books})
            except queue.Full: pass
            if r.random() < 0.15:
                tn += 1
                try: tr_q.put_nowait({"number": tn, "direction": r.choice(["BUY","SELL"]), "spread_dollars": r.uniform(-3,3), "computed_index": 525+r.uniform(-5,5), "actual_price": 52500+r.randint(-200,200), "cycle": cyc})
                except queue.Full: pass
            try: st_q.put_nowait({"cycle": cyc, "order_count": cyc//50, "net_position": r.randint(-500,500), "computed_index": 525+r.uniform(-2,2), "trades": tn})
            except queue.Full: pass
    threading.Thread(target=pump, daemon=True).start()
    dash = HFTDashboard(inp_q, bk_q, tr_q, st_q, ctl_q, fake_stocks, {}, FakeTracker())
    dash.run()
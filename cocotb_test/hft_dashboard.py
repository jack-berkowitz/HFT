"""
hft_dashboard.py — Browser-based HFT Pipeline Trading Dashboard
Bar-graph order books, auto-rotates to active symbol.
"""

import http.server, json, queue, threading, webbrowser, time, socket

_state = {"inputs":[],"books":{},"trades":[],"status":{},"stocks":[],"active_sym":0}
_state_lock = threading.Lock()
_control_q = None

HTML_PAGE = r"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>HFT Pipeline Dashboard</title>
<style>
@import url('https://fonts.googleapis.com/css2?family=JetBrains+Mono:wght@400;600&family=Inter:wght@400;500;600;700&display=swap');
*{margin:0;padding:0;box-sizing:border-box}
:root{
  --bg:#0a0e17;--panel:#111827;--card:#1a2332;--input:#0d1520;
  --border:#2a3a52;--text:#e2e8f0;--dim:#64748b;--muted:#475569;
  --bid:#22c55e;--bid-fill:rgba(34,197,94,0.12);
  --ask:#ef4444;--ask-fill:rgba(239,68,68,0.12);
  --accent:#3b82f6;--accent-dim:#1e3a5f;--warn:#f59e0b;--purple:#a78bfa;
  --mono:'JetBrains Mono',Consolas,monospace;--sans:'Inter',-apple-system,sans-serif;
}
body{background:var(--bg);color:var(--text);font-family:var(--sans);font-size:13px;overflow:hidden;height:100vh}

.hdr{background:#0f172a;height:52px;display:flex;align-items:center;padding:0 20px;border-bottom:1px solid var(--border);gap:20px}
.hdr .logo{font-size:15px;font-weight:700;color:var(--accent)}.hdr .logo span{color:var(--warn)}
.st{font-family:var(--mono);font-size:11px;color:var(--dim)}.st b{color:var(--text);font-weight:600}
.st.g b{color:var(--bid)}.st.a b{color:var(--warn)}.st.r b{color:var(--ask)}
.ctrls{margin-left:auto;display:flex;gap:8px}
.btn{font-family:var(--sans);font-size:12px;font-weight:600;padding:6px 16px;border:1px solid var(--border);border-radius:6px;background:var(--card);color:var(--text);cursor:pointer}
.btn:hover{background:var(--accent-dim);border-color:var(--accent)}
.btn.play{color:var(--bid)}.btn.pause{color:var(--warn)}

.grid{display:grid;grid-template-columns:1fr 1.5fr;grid-template-rows:1fr 1fr;gap:6px;padding:6px;height:calc(100vh - 52px)}
.pnl{background:var(--panel);border:1px solid var(--border);border-radius:8px;display:flex;flex-direction:column;overflow:hidden}
.pnl-h{padding:10px 14px;font-size:12px;font-weight:600;color:var(--accent);border-bottom:1px solid var(--border);display:flex;align-items:center;gap:10px;flex-shrink:0}
.pnl-b{flex:1;overflow-y:auto;background:var(--input)}
.pnl-b::-webkit-scrollbar{width:6px}
.pnl-b::-webkit-scrollbar-thumb{background:var(--border);border-radius:3px}

/* Feed */
.fl{font-family:var(--mono);font-size:11px;padding:3px 12px;white-space:nowrap;border-bottom:1px solid rgba(42,58,82,0.3)}
.fl.buy{color:var(--bid)}.fl.sell{color:var(--ask)}.fl.mod{color:var(--warn)}.fl.exec{color:var(--purple)}.fl.del{color:var(--dim)}

/* Order book */
.ob-title{text-align:center;font-family:var(--mono);font-size:15px;font-weight:700;padding:10px 0 2px;color:var(--text)}
.ob-sub{text-align:center;font-family:var(--mono);font-size:11px;color:var(--dim);padding-bottom:8px}
.ob-container{display:flex;flex-direction:column;padding:0 12px 12px}
.ob-label{font-family:var(--mono);font-size:10px;color:var(--muted);text-transform:uppercase;letter-spacing:1px;padding:6px 0 2px}
.ob-row{display:flex;align-items:center;height:30px;margin:1px 0}
.ob-row .price{font-family:var(--mono);font-size:12px;font-weight:600;min-width:95px}
.ob-row .price.bid{color:var(--bid);text-align:right;padding-right:8px}
.ob-row .price.ask{color:var(--ask);text-align:left;padding-left:8px}
.ob-row .bar-wrap{flex:1;height:22px;position:relative;border-radius:3px;overflow:hidden}
.ob-row .bar{height:100%;border-radius:3px;transition:width 0.3s ease;min-width:3px}
.ob-row .bar.bid{background:var(--bid);opacity:0.5;float:right}
.ob-row .bar.ask{background:var(--ask);opacity:0.5;float:left}
.ob-row .qty{font-family:var(--mono);font-size:11px;color:var(--dim);min-width:55px}
.ob-row .qty.r{text-align:right;padding-right:6px}
.ob-row .qty.l{text-align:left;padding-left:6px}
.ob-mid{text-align:center;font-family:var(--mono);font-size:12px;color:var(--warn);padding:8px 0;margin:4px 0;border-top:1px dashed var(--border);border-bottom:1px dashed var(--border);font-weight:600}

/* Trades */
.th-row{font-family:var(--mono);font-size:10px;color:var(--muted);padding:6px 12px;border-bottom:1px solid var(--border);background:var(--input);position:sticky;top:0;white-space:pre}
.tl{font-family:var(--mono);font-size:11px;padding:4px 12px;border-bottom:1px solid rgba(42,58,82,0.3);white-space:pre}
.tl.sell{color:var(--ask)}.tl.buy{color:var(--bid)}

/* Index stats */
.idx-st{padding:8px 14px;display:flex;gap:20px;font-family:var(--mono);font-size:12px;border-bottom:1px solid var(--border);flex-shrink:0;flex-wrap:wrap}
.idx-st .c{color:var(--bid)}.idx-st .ac{color:var(--text)}.idx-st .sp{color:var(--warn)}.idx-st .diff{color:var(--ask)}
</style>
</head>
<body>
<div class="hdr">
  <div class="logo">&#9889; HFT PIPELINE <span>SIMULATOR</span></div>
  <div class="st" id="sc">CYCLE <b>0</b></div>
  <div class="st" id="so">ORDERS <b>0</b></div>
  <div class="st a" id="st">TRADES <b>0</b></div>
  <div class="st" id="sp">POS <b>0</b></div>
  <div class="st g" id="si">IDX <b>$0.00</b></div>
  <div class="ctrls">
    <button class="btn play" onclick="sendCmd('play')">&#9654; Play</button>
    <button class="btn pause" onclick="sendCmd('pause')">&#9208; Pause</button>
  </div>
</div>
<div class="grid">
  <div class="pnl">
    <div class="pnl-h">&#128202; LIVE MARKET FEED</div>
    <div class="pnl-b" id="feed"></div>
  </div>
  <div class="pnl">
    <div class="pnl-h">&#128214; ORDER BOOK &#8212; <span id="book-sym">...</span></div>
    <div class="pnl-b" id="book-view" style="overflow-y:auto"></div>
  </div>
  <div class="pnl">
    <div class="pnl-h">&#127919; TRADE OUTPUT</div>
    <div class="pnl-b" id="trades">
      <div class="th-row">   #   DIR      PRICE       SPREAD      CYCLE</div>
    </div>
  </div>
  <div class="pnl">
    <div class="pnl-h">&#128200; S&amp;P 500 INDEX BOOK</div>
    <div class="idx-st">
      <span class="c" id="ic">Computed: --</span>
      <span class="ac" id="ia">Actual: --</span>
      <span class="sp" id="is">Spread: --</span>
    </div>
    <div class="pnl-b" id="idx-book"></div>
  </div>
</div>
<script>
let allBooks={},activeSym=0,idxSym=0,stockList=[],stockMap={},lastComputed=0;
function $(id){return document.getElementById(id)}

function renderBarBook(container, bookData, ticker) {
  if (!bookData || (!bookData.bids.length && !bookData.asks.length)) {
    container.innerHTML = '<div style="text-align:center;color:#475569;padding:40px;font-family:var(--mono)">Waiting for data...</div>';
    return;
  }
  const bids = bookData.bids || [], asks = bookData.asks || [];
  const allQ = [...bids.map(b=>b[1]), ...asks.map(a=>a[1])];
  const maxQ = Math.max(...allQ, 1);

  let html = '';
  if (ticker) {
    html += '<div class="ob-title">' + ticker + '</div>';
    if (bids.length && asks.length) {
      const mid = (bids[0][0] + asks[0][0]) / 200;
      const spread = (asks[0][0] - bids[0][0]) / 100;
      html += '<div class="ob-sub">Mid $' + mid.toFixed(2) + ' &middot; Spread $' + spread.toFixed(2) + '</div>';
    }
  }

  html += '<div class="ob-container">';

  // Asks — reversed (worst on top, best near mid)
  if (asks.length) {
    html += '<div class="ob-label">Asks (sell)</div>';
    const rev = [...asks].reverse();
    for (const [p, q] of rev) {
      const w = Math.max(2, (q / maxQ) * 100);
      html += '<div class="ob-row">'
        + '<div class="qty r">' + q.toLocaleString() + '</div>'
        + '<div class="bar-wrap" style="background:var(--ask-fill)"><div class="bar ask" style="width:' + w + '%"></div></div>'
        + '<div class="price ask">$' + (p/100).toFixed(2) + '</div>'
        + '</div>';
    }
  }

  // Mid price
  if (bids.length && asks.length) {
    const mid = (bids[0][0] + asks[0][0]) / 200;
    html += '<div class="ob-mid">$' + mid.toFixed(2) + '</div>';
  }

  // Bids — best on top
  if (bids.length) {
    html += '<div class="ob-label">Bids (buy)</div>';
    for (const [p, q] of bids) {
      const w = Math.max(2, (q / maxQ) * 100);
      html += '<div class="ob-row">'
        + '<div class="price bid">$' + (p/100).toFixed(2) + '</div>'
        + '<div class="bar-wrap" style="background:var(--bid-fill)"><div class="bar bid" style="width:' + w + '%"></div></div>'
        + '<div class="qty l">' + q.toLocaleString() + '</div>'
        + '</div>';
    }
  }

  html += '</div>';
  container.innerHTML = html;
}

async function poll() {
  try {
    const r = await fetch('/api/state');
    const d = await r.json();
    if (d.stocks && d.stocks.length && !stockList.length) {
      stockList = d.stocks;
      idxSym = stockList[stockList.length - 1].idx;
      stockList.forEach(s => { stockMap[s.idx] = s.ticker; });
      activeSym = stockList[0].idx;
    }
    if (d.inputs && d.inputs.length) {
      const f = $('feed');
      d.inputs.forEach(ev => {
        const div = document.createElement('div');
        const tag = ev.type==='ADD'?(ev.side==='BUY'?'buy':'sell'):ev.type==='MOD'?'mod':ev.type==='EXEC'?'exec':'del';
        div.className = 'fl ' + tag;
        const arrow = ev.side==='BUY'?'\u25B2':'\u25BC';
        div.textContent = `${String(ev.cycle).padStart(7)}  ${ev.type.padEnd(5)} ${ev.ticker.padEnd(8)} ${arrow}  $${(ev.price/100).toFixed(2).padStart(9)} \u00d7 ${String(ev.qty).padEnd(5)}`;
        f.appendChild(div);
      });
      while (f.children.length > 300) f.removeChild(f.firstChild);
      f.scrollTop = f.scrollHeight;
    }
    if (d.books && Object.keys(d.books).length) allBooks = d.books;
    if (d.active_sym !== undefined && d.active_sym !== null) activeSym = d.active_sym;
    if (d.trades && d.trades.length) {
      const tc = $('trades');
      d.trades.forEach(t => {
        const div = document.createElement('div');
        div.className = 'tl ' + t.direction.toLowerCase();
        div.textContent = `${String(t.number).padStart(4)}   ${t.direction.padStart(4)}   $${(t.actual_price/100).toFixed(2).padStart(10)}   $${t.spread_dollars.toFixed(2).padStart(10)}   ${String(t.cycle).padStart(8)}`;
        tc.appendChild(div);
      });
      tc.scrollTop = tc.scrollHeight;
    }
    if (d.status && d.status.cycle) {
      const s = d.status;
      $('sc').innerHTML = 'CYCLE <b>' + s.cycle.toLocaleString() + '</b>';
      $('so').innerHTML = 'ORDERS <b>' + s.order_count + '</b>';
      $('st').innerHTML = 'TRADES <b>' + s.trades + '</b>';
      const np = s.net_position;
      $('sp').className = 'st ' + (np>0?'g':np<0?'r':'');
      $('sp').innerHTML = 'POS <b>' + (np>=0?'+':'') + np + '</b>';
      // computed_index from DUT is in cents (Q44.20 already divided by Q_ONE in Python)
      lastComputed = s.computed_index;
      $('si').innerHTML = 'IDX <b>$' + (s.computed_index / 100).toFixed(2) + '</b>';
      $('ic').textContent = 'Computed: $' + (s.computed_index / 100).toFixed(2);
    }
    // Active stock book
    const ticker = stockMap[activeSym] || ('SYM ' + activeSym);
    $('book-sym').textContent = ticker;
    renderBarBook($('book-view'), allBooks[activeSym], ticker);
    // Index book + spread info
    const ib = allBooks[idxSym];
    renderBarBook($('idx-book'), ib, 'SPY (Index)');
    if (ib) {
      if (ib.bids && ib.bids.length && ib.asks && ib.asks.length) {
        const actualMid = (ib.bids[0][0] + ib.asks[0][0]) / 2;  // cents
        $('ia').textContent = 'Actual: $' + (actualMid / 100).toFixed(2);
        const diff = lastComputed - actualMid;
        $('is').textContent = 'Spread: $' + (diff / 100).toFixed(2);
        $('is').className = diff > 0 ? 'diff' : 'sp';
      }
    }
  } catch(e) {}
  setTimeout(poll, 120);
}
async function sendCmd(c){try{await fetch('/api/control?cmd='+c)}catch(e){}}
poll();
</script>
</body>
</html>"""


class DashboardHandler(http.server.BaseHTTPRequestHandler):
    def log_message(self, *a): pass
    def do_GET(self):
        if self.path in ('/','/index.html'):
            self.send_response(200); self.send_header('Content-Type','text/html'); self.end_headers()
            self.wfile.write(HTML_PAGE.encode())
        elif self.path=='/api/state':
            self.send_response(200)
            self.send_header('Content-Type','application/json')
            self.send_header('Cache-Control','no-cache')
            self.end_headers()
            with _state_lock:
                p={"inputs":list(_state["inputs"]),"books":dict(_state["books"]),
                   "trades":list(_state["trades"]),"status":dict(_state["status"]),
                   "stocks":_state["stocks"],"active_sym":_state["active_sym"]}
                _state["inputs"].clear(); _state["trades"].clear()
            self.wfile.write(json.dumps(p).encode())
        elif self.path.startswith('/api/control'):
            cmd=self.path.split('cmd=')[-1] if 'cmd=' in self.path else ''
            if _control_q and cmd:
                try: _control_q.put_nowait(cmd)
                except: pass
            self.send_response(200); self.send_header('Content-Type','text/plain'); self.end_headers()
            self.wfile.write(b'ok')
        else: self.send_response(404); self.end_headers()

def _find_port():
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s: s.bind(('',0)); return s.getsockname()[1]

class HFTDashboard:
    def __init__(self, input_q, book_q, trade_q, status_q, control_q, stocks, ticker_map, tracker):
        global _control_q
        self.iq=input_q;self.bq=book_q;self.tq=trade_q;self.sq=status_q
        _control_q=control_q;self.port=_find_port();self._on=True
        with _state_lock: _state["stocks"]=stocks
    def run(self):
        srv=http.server.HTTPServer(('127.0.0.1',self.port),DashboardHandler)
        srv.timeout=0.1
        threading.Thread(target=self._serve,args=(srv,),daemon=True).start()
        url=f'http://127.0.0.1:{self.port}'
        print(f"\n  \u26a1 Dashboard: {url}\n")
        webbrowser.open(url)
        while self._on: self._drain(); time.sleep(0.04)
        srv.shutdown()
    def _serve(self,s):
        while self._on: s.handle_request()
    def stop(self): self._on=False
    def _drain(self):
        b=[]
        for _ in range(60):
            try: b.append(self.iq.get_nowait())
            except: break
        if b:
            with _state_lock: _state["inputs"].extend(b)
        try:
            while True:
                bk=self.bq.get_nowait()
                with _state_lock:
                    _state["books"]=bk.get("books",{})
                    if bk.get("active_sym") is not None: _state["active_sym"]=bk["active_sym"]
        except: pass
        tb=[]
        try:
            while True: tb.append(self.tq.get_nowait())
        except: pass
        if tb:
            with _state_lock: _state["trades"].extend(tb)
        try:
            while True:
                with _state_lock: _state["status"]=self.sq.get_nowait()
        except: pass


if __name__=="__main__":
    import random as R
    iq,bq,tq,sq,cq=queue.Queue(5000),queue.Queue(5000),queue.Queue(1000),queue.Queue(500),queue.Queue(100)
    fs=[{"ticker":"AAPL","idx":0,"base_price":19200},{"ticker":"MSFT","idx":1,"base_price":42500},
        {"ticker":"AMZN","idx":2,"base_price":18700},{"ticker":"SPY","idx":3,"base_price":52500}]
    def pump():
        r=R.Random(99);cyc=0;tn=0
        while True:
            time.sleep(0.5);cyc+=50;s=r.choice(fs)
            try:iq.put_nowait({"cycle":cyc,"type":r.choice(["ADD","MOD","DEL","EXEC"]),"ticker":s["ticker"],"sym":s["idx"],"price":s["base_price"]+r.randint(-200,200),"qty":r.choice([50,100,200,500]),"side":r.choice(["BUY","SELL"])})
            except:pass
            books={f["idx"]:{"bids":[(f["base_price"]-50-i*80,r.randint(50,800)) for i in range(3)],"asks":[(f["base_price"]+50+i*80,r.randint(50,800)) for i in range(3)]} for f in fs}
            try:bq.put_nowait({"cycle":cyc,"books":books,"active_sym":s["idx"]})
            except:pass
            if r.random()<0.2:
                tn+=1
                try:tq.put_nowait({"number":tn,"direction":r.choice(["BUY","SELL"]),"spread_dollars":r.uniform(-3,3),"computed_index":32500+r.uniform(-500,500),"actual_price":52500+r.randint(-200,200),"cycle":cyc})
                except:pass
            try:sq.put_nowait({"cycle":cyc,"order_count":cyc//50,"net_position":r.randint(-500,500),"computed_index":32500+r.uniform(-200,200),"trades":tn})
            except:pass
    threading.Thread(target=pump,daemon=True).start()
    class FT: history=[]
    d=HFTDashboard(iq,bq,tq,sq,cq,fs,{},FT())
    d.run()
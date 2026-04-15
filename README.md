# HFT Pipeline RTL + Verification Suite

This repository contains a parameterized SystemVerilog HFT pipeline and two
verification flows:

- module-level + integration RTL testbenches under `HFT_Modules/test`
- an end-to-end cocotb market simulation + browser dashboard under `cocotb_test`

The top-level DUT is `hft_pipeline_top`, which ingests AXI-stream Ethernet
frames carrying XDP-like market data, maintains per-symbol state, computes a
weighted synthetic index, generates arbitrage trade signals, and emits TX order
packets over AXI-stream.

## Architecture

Data path in `HFT_Modules/verilog/hft_pipeline_top.sv`:

```
RX AXIS
  -> ipv4_udp_port_filter
  -> xdp_packet_framer
  -> xdp_msg_demux
  -> order_lookup (backed by skewed_hash_table + dp_ram)
  -> top_of_book
  -> index_arb_engine
  -> order_gen_tx
TX AXIS
```

### 1) `ipv4_udp_port_filter`

- Parses early Ethernet/IP/UDP header beats.
- Passes only IPv4 + UDP packets matching `FILTER_PORT` (`XDP_PORT` default).
- Drops non-matching frames while preserving AXI backpressure behavior.

### 2) `xdp_packet_framer`

- Strips a configurable number of leading bytes (`HEADER_BYTES`).
- Realigns payload bytes across 64-bit AXI beats using `tkeep`.
- Emits packed message payload stream for the parser.

### 3) `xdp_msg_demux`

- Collects one payload message and decodes into `pillar_msg_t`.
- Supports ADD/MOD/DEL/EXEC/REPLACE message types from `sys_defs.svh`.
- Outputs one-cycle `out_msg.valid` once a message is fully collected.

### 4) `order_lookup`

- Owns order-id keyed state transitions for all message types.
- Uses `skewed_hash_table` for lookup/insert/delete of `{price, qty, side}`.
- Emits `order_lookup_out_t` including prior state (`old_price/qty/side`) so
  downstream book logic can apply deltas correctly.

### 5) `top_of_book`

- Maintains per-symbol bid/ask ladders (depth `N_LEVELS`) across `N_SYMBOLS`.
- Applies remove-then-add semantics for MOD/REPLACE and partial remove for EXEC.
- Emits symbol-level `tob_out_t` snapshots containing best bid/ask.

### 6) `index_arb_engine`

- Stores component weights (`wt_wr_en/wt_wr_addr/wt_wr_data`) in Q-format.
- Tracks component mid-price deltas and incrementally updates `index_accum`.
- Compares computed synthetic index vs actual index symbol mid-price.
- Fires `trade_signal_t` when absolute spread exceeds `threshold`.

### 7) `order_gen_tx`

- Gates output with `enable`, position checks (`MAX_POS`), and cooldown
  (`COOLDOWN_CYC`).
- Builds a 10-beat Ethernet+IPv4+UDP+order payload packet.
- Tracks `order_count`, signed `net_position`, and one-cycle `pkt_sent`.

## Core Types and Constants

Shared in `HFT_Modules/verilog/sys_defs.svh`:

- AXI width: `DATA_W=64`, `KEEP_W=8`
- Message type IDs and normalized internal enums (`MSG_ADD`...`MSG_REPL`)
- Default TOB dimensions: `TOB_LEVELS=3`, `TOB_SYMBOLS=500`
- Index settings: `IDX_SYMBOL=500`
- Fixed-point config: `WEIGHT_FRAC_BITS=20`, `WEIGHT_W=32`, `ACCUM_W=64`
- Key packed structs: `pillar_msg_t`, `order_lookup_out_t`, `tob_out_t`,
  `trade_signal_t`, `hash_entry_t`

## Repository Layout

```
HFT_Modules/
├── verilog/
│   ├── sys_defs.svh
│   ├── hft_pipeline_top.sv
│   ├── ipv4_udp_port_filter.sv
│   ├── xdp_packet_framer.sv
│   ├── xdp_msg_demux.sv
│   ├── order_lookup.sv
│   ├── skewed_hash_table.sv
│   ├── dp_ram.sv
│   ├── top_of_book.sv
│   ├── index_arb_engine.sv
│   └── order_gen_tx.sv
├── test/
│   ├── *_tb.sv                # module and integration testbenches
│   ├── xdp_chain_tb.sv        # filter+framer+demux integration
│   ├── hft_pipeline_tb.sv     # full 7-stage integration
│   └── hft_pipeline_top_tb.sv # top wrapper integration
└── Makefile                   # VCS simulation + synthesis helpers

cocotb_test/
├── Makefile
├── test_hft_pipeline_top.py   # randomized market generator + DUT driver
└── hft_dashboard.py           # browser dashboard server/UI
```

## RTL Simulation (`HFT_Modules`)

From repo root:

```bash
cd HFT_Modules
```

Typical targets:

```bash
# Front-end chain integration (default goal in current Makefile)
make xdp_chain.out
make xdp_chain.pass

# Full pipeline integration (no wrapper)
make hft_pipeline.out
make hft_pipeline.pass

# Top-level wrapper integration
make hft_pipeline_top.out
make hft_pipeline_top.pass

# Single module examples
make order_lookup.out
make top_of_book.out
make index_arb_engine.out
```

Useful extras:

```bash
make <target>.verdi      # waveform debug (Verdi)
make <module>.synth      # synth/<module>.vg
make <module>.syn.out    # gate-level sim output
make <module>.syn.pass   # pass/fail grep
make slack               # grep slack from synth reports
make clean               # remove build artifacts
```

## Cocotb End-to-End Simulation (`cocotb_test`)

The cocotb test (`test_hft_pipeline_top.py`) drives realistic multi-symbol
traffic through `hft_pipeline_top`:

- optional Yahoo Finance bootstrap prices (`yfinance`), with deterministic fallback
- randomized ADD/MOD/DEL/EXEC event generation
- sector sweeps and burst regimes to create spread excursions
- runtime programming of component weights and arbitrage threshold
- TX monitor that decodes generated order packets

It can run headless or with a browser dashboard (`hft_dashboard.py`) that shows
live feed, order books, trade output, and index spread metrics.

Run:

```bash
cd cocotb_test
make
```

Key knobs:

```bash
make HFT_NUM_COMPONENTS=500   # scale components/index symbol
make HFT_GUI_ENABLE=0         # headless mode
make HFT_DELAY=0.1            # inter-event pacing
make SIM=verilator            # use Verilator
make clean
```

Environment variables consumed by the Python test:

| Variable | Default | Purpose |
|---|---:|---|
| `HFT_NUM_COMPONENTS` | `20` | Number of component symbols (index symbol is set to this value) |
| `HFT_GUI_ENABLE` | `1` | Enables browser dashboard thread |
| `HFT_DELAY` | `0.4` | Extra pacing delay between event bursts |

## Notes

- The design is heavily parameterized in `hft_pipeline_top.sv`; cocotb passes
  matching overrides (especially for component/index sizing) through simulator
  arguments in `cocotb_test/Makefile`.
- `order_gen_tx` includes risk controls (`MAX_POS`) and cooldown-based trade
  rate limiting to reduce repeated triggers on the same spread condition.
- `skewed_hash_table` supports multi-way hashing with bounded eviction chains
  (`MAX_CHAIN`) and explicit insert failure signaling.
# HFT Pipeline — Cocotb Testbench + Trading Dashboard

## Overview

A comprehensive cocotb-based testbench that drives realistic S&P 500 market
data through the 7-stage HFT pipeline and displays everything in a live
dark-themed trading dashboard.

```
  Network RX ─► Filter ─► Framer ─► Demux ─► Order Lookup ─► Top-of-Book
                                                               ─► Index Arb ─► Order Gen TX ─► Network TX
```

## Directory Layout

```
cocotb_test/
├── Makefile                    # cocotb build (icarus / verilator)
├── test_hft_pipeline_top.py    # Main cocotb test (drivers, monitors, market data)
├── hft_dashboard.py            # Dark-themed tkinter GUI dashboard
├── requirements.txt            # Python dependencies
└── README.md                   # This file
```

Place this directory alongside your existing `verilog/` and `test/` dirs:
```
HFT_Modules/
├── verilog/
│   ├── sys_defs.svh
│   ├── hft_pipeline_top.sv
│   ├── ipv4_udp_port_filter.sv
│   └── ...
├── test/
├── cocotb_test/          ◄── this directory
└── Makefile
```

## Prerequisites

```bash
pip install cocotb yfinance
sudo apt install iverilog          # Icarus Verilog (12+ recommended)
# or: sudo apt install verilator   # Alternative simulator
```

## Running

```bash
cd cocotb_test/

# Default: 20 components, GUI enabled
make

# Full S&P 500 (slow — thousands of packets)
make HFT_NUM_COMPONENTS=500

# Headless (no GUI, just console output)
make HFT_GUI_ENABLE=0

# Use Verilator instead of Icarus
make SIM=verilator

# Clean build artifacts
make clean
```

## Test the GUI standalone (no simulator needed)

```bash
python hft_dashboard.py
```

This launches the dashboard with fake data to verify the UI looks correct.

## Dashboard Panels

| Panel | Description |
|-------|-------------|
| **Live Market Feed** | Scrolling log of all input messages (ADD/MOD/DEL/EXEC) colour-coded by type |
| **Order Book Viewer** | Dropdown to select any symbol, shows bid/ask price ladder with quantity bars |
| **Trade Signals** | Blotter of arbitrage trades fired by the pipeline |
| **S&P 500 Index Book** | The index instrument's order book + computed vs actual index values |

### Controls

- **Play** — resume simulation
- **Pause** — freeze the simulation loop
- **Step** — advance one batch then pause
- **History slider** — scrub through order book snapshots at any past cycle

## Configuration

| Environment Variable | Default | Description |
|---------------------|---------|-------------|
| `HFT_NUM_COMPONENTS` | 20 | Number of index components (1–500) |
| `HFT_GUI_ENABLE` | 1 | Set to 0 for headless mode |

## Architecture Notes

The testbench and GUI run in the same process but different threads:

```
┌─────────────────────┐       queues       ┌──────────────────┐
│  cocotb test        │ ═══════════════════►│  tkinter GUI     │
│  (simulator thread) │  input_q, book_q   │  (GUI thread)    │
│                     │  trade_q, status_q  │                  │
│                     │ ◄═══════════════════│                  │
│                     │    control_q        │                  │
└─────────────────────┘                     └──────────────────┘
```

The Python-side `BookTracker` maintains a shadow copy of the order books,
updated as each market event is sent. This is what the GUI displays.
Periodic snapshots are stored for the history slider.
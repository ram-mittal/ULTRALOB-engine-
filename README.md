# UltraLOB Engine

A standalone C++20 low-latency limit order book, matching engine, FIX 4.4 gateway, journaling system, market-data publisher, and quantitative Monte Carlo pricing library.

This repository contains the standalone C++ engine only. It excludes the separate UltraLOB web platform, broker account management, copy-trading application, MT5 bridge, private strategy catalogue, and associated commercial services.

## Core components

- Price-time-priority limit order book and matching engine
- Preallocated order pool and intrusive price-level structures
- FIX 4.4 framing, message handling, sessions, and gateway components
- Binary TCP gateway and market-data publisher
- Memory-mapped journal and deterministic event processing
- Pre-trade risk checks
- CPU Monte Carlo pricing with optional CUDA backend
- Unit, integration, and performance benchmark suites

## Build

Requirements: CMake 3.20+, a C++20-compatible compiler, and network access during initial configuration for FetchContent dependencies.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

CUDA support is optional and depends on a compatible CUDA toolkit. See the CMake options and source layout for details.

## Scope and status

This is an engineering codebase, not a certified exchange, broker, investment service, or guarantee of trading performance. Benchmark results are hardware- and workload-dependent. The web application and live broker integrations are maintained separately and are not included here.

## License

MIT. See [LICENSE](LICENSE).

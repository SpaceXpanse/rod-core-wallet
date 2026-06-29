# Architecture Overview

## Project type

SpaceXpanse ROD Core Wallet is a Bitcoin/Namecoin-derived C++ node, wallet, CLI, and optional Qt GUI repository with SpaceXpanse-specific binaries and protocol extensions ([`README.md`](../../README.md), [`src/Makefile.am`](../../src/Makefile.am)).

## Main directories

- [`src/`](../../src/) — core implementation, build targets, wallet, RPC, networking, names, crypto, and tests.
- [`test/`](../../test/) — functional, util, and lint test entrypoints.
- [`doc/`](../../doc/) — developer, build, API, and release documentation.
- [`docs/`](../../docs/) — maintainer wiki and Carbon Memory durable documentation.
- [`depends/`](../../depends/) — dependency builder used by some CI and reproducible build paths.
- [`build_msvc/`](../../build_msvc/) — Windows-native Visual Studio build flow.
- [`ci/`](../../ci/) — Docker-oriented CI orchestration.

## Binary surface

Primary executables are `spacexpansed`, `spacexpanse-cli`, `spacexpanse-tx`, `spacexpanse-wallet`, `spacexpanse-util`, and optional GUI/multiprocess binaries defined in [`configure.ac`](../../configure.ac) and [`src/Makefile.am`](../../src/Makefile.am).

## Internal layering

The build separates reusable code into `libbitcoin_server`, `libbitcoin_common`, `libbitcoin_util`, `libbitcoin_cli`, optional wallet libs, and optional ZMQ/multiprocess targets. Placement matters because feature flags and linkability depend on these library boundaries ([`src/Makefile.am`](../../src/Makefile.am)).

At code level, the repository is dominated by C/C++ sources under [`src/`](../../src/) and embedded upstream subtrees such as [`src/secp256k1`](../../src/secp256k1), [`src/leveldb`](../../src/leveldb), [`src/univalue`](../../src/univalue), and [`src/crc32c`](../../src/crc32c). A local scan recorded 2,209 files total, with 1,788 categorized as code and 214 as documentation.

## Data and feature flow

- Consensus, chainstate, mempool, networking, and RPC code live primarily in server/common libraries.
- Wallet code is optional and must compile out cleanly with `--disable-wallet`.
- GUI code depends on interfaces and RPC-visible functionality, with project guidance favoring RPC-first external features.

## Testing surface

- C++ unit and fuzz tests live mostly in [`src/test/`](../../src/test/) and [`src/wallet/test/`](../../src/wallet/test/).
- Python functional coverage is concentrated in [`test/functional/`](../../test/functional/) with a large shared framework under [`test/functional/test_framework/`](../../test/functional/test_framework/).
- Shell lint and validation entrypoints live under [`test/lint/`](../../test/lint/).

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

More specific directory responsibilities from the current scan and build files:

- [`src/names/`](../../src/names/) and [`src/rpc/names.cpp`](../../src/rpc/names.cpp) carry the Namecoin-derived name-value logic extended for SpaceXpanse/ROD behavior.
- [`src/node/`](../../src/node/) and [`src/interfaces/`](../../src/interfaces/) define newer interface boundaries between node, wallet, and GUI-facing code, while preserving the project-specific [`lowerCamelCase`](../../doc/developer-notes.md) exception for interface methods.
- [`src/qt/`](../../src/qt/) contains the optional desktop GUI plus Qt-specific name-management screens such as [`managenamespage.cpp`](../../src/qt/managenamespage.cpp) and [`configurenamedialog.cpp`](../../src/qt/configurenamedialog.cpp), showing that the SpaceXpanse fork exposes naming functionality beyond a stock wallet shell.
- [`test/functional/test_framework/`](../../test/functional/test_framework/) is a substantial shared Python harness, not just helper scripts; functional-test architecture depends on it for RPC, P2P, wallet, auxpow, and SpaceXpanse-specific fixtures.

## Data and feature flow

- Consensus, chainstate, mempool, networking, and RPC code live primarily in server/common libraries.
- Wallet code is optional and must compile out cleanly with `--disable-wallet`.
- GUI code depends on interfaces and RPC-visible functionality, with project guidance favoring RPC-first external features.

## Testing surface

- C++ unit and fuzz tests live mostly in [`src/test/`](../../src/test/) and [`src/wallet/test/`](../../src/wallet/test/).
- Python functional coverage is concentrated in [`test/functional/`](../../test/functional/) with a large shared framework under [`test/functional/test_framework/`](../../test/functional/test_framework/).
- Shell lint and validation entrypoints live under [`test/lint/`](../../test/lint/).

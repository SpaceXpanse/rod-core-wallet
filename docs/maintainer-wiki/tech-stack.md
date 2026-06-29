# Tech Stack

## Core stack

- C++17 node and wallet codebase built with GNU Autotools ([`configure.ac`](../../configure.ac), [`Makefile.am`](../../Makefile.am)).
- Python 3.6-based test and tooling surface; repository pins [`3.6.12`](../../.python-version) and lint rules check for unsupported newer syntax ([`.python-version`](../../.python-version), [`test/lint/lint-python.sh`](../../test/lint/lint-python.sh)).
- Optional Qt GUI build with minimum Qt 5.9.5, documented as 5.12.11 in dependency references ([`configure.ac`](../../configure.ac), [`doc/dependencies.md`](../../doc/dependencies.md)).

## Repository composition snapshot

- A local structural scan found 2,209 files total with category breakdown: 1,788 code, 214 docs, 116 config, 81 scripts, 5 infra, 4 markup, and 1 data file.
- Dominant languages by file count include C headers/sources, C++ sources, Python, Markdown, JSON, shell, and Qt UI/translation assets (scan summary captured in [`.understand-anything-scan.json`](../../.understand-anything-scan.json)).

## Build system

- Primary build: `./autogen.sh` → `./configure` → `make` from repository root ([`autogen.sh`](../../autogen.sh), [`configure.ac`](../../configure.ac)).
- Native Windows alternative: `build_msvc/msvc-autogen.py` plus `msbuild` in [`build_msvc/README.md`](../../build_msvc/README.md).

## Key dependencies

- Boost >= 1.64.0 ([`doc/dependencies.md`](../../doc/dependencies.md)).
- libevent >= 2.0.21 ([`configure.ac`](../../configure.ac), [`doc/dependencies.md`](../../doc/dependencies.md)).
- Optional Berkeley DB 4.8.x and SQLite >= 3.7.17 for wallet variants ([`configure.ac`](../../configure.ac), [`doc/dependencies.md`](../../doc/dependencies.md)).
- Optional ZeroMQ, MiniUPnPc, and libnatpmp toggled through configure flags ([`configure.ac`](../../configure.ac)).

## Testing and linting

- Boost unit tests compiled into `test/test_spacexpanse` via [`src/Makefile.test.include`](../../src/Makefile.test.include).
- Functional tests in [`test/functional/`](../../test/functional/) using the Python harness and framework docs ([`test/README.md`](../../test/README.md), [`test/functional/README.md`](../../test/functional/README.md)).
- Shell-based lint entrypoints under [`test/lint/`](../../test/lint/) with aggregate scripts in CI ([`test/README.md`](../../test/README.md), [`ci/README.md`](../../ci/README.md)).

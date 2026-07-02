# AGENTS.md

This file provides guidance to agents when working with code in this repository.

- This repo is an Autotools/C++17 build, not CMake: bootstrap with `./autogen.sh`, configure from repo root, then build with `make` ([`autogen.sh`](autogen.sh), [`configure.ac`](configure.ac), [`Makefile.am`](Makefile.am)).
- Windows has a separate native flow under [`build_msvc/README.md`](build_msvc/README.md) using [`build_msvc/msvc-autogen.py`](build_msvc/msvc-autogen.py) before `msbuild`; don’t mix that output tree with Autotools artifacts.
- The shipped binary names are SpaceXpanse-specific: [`spacexpansed`](src/Makefile.am), [`spacexpanse-cli`](src/Makefile.am), [`spacexpanse-tx`](src/Makefile.am), [`spacexpanse-wallet`](src/Makefile.am), [`spacexpanse-util`](src/Makefile.am), [`spacexpanse-qt`](configure.ac).
- Unit-test entrypoint is [`test/test_spacexpanse`](src/Makefile.test.include:10); build via `make check` or `make spacexpanse_test`.
- Single C++ suite flow is non-obvious: run `make src/test/foo_tests.cpp.test` from repo root; the make rule extracts the Boost suite name and writes a sibling log file ([`src/Makefile.test.include`](src/Makefile.test.include:363)).
- Functional tests are Python entrypoints under [`test/functional/`](test/functional); run one directly or through [`test/functional/test_runner.py`](test/functional/test_runner.py) with explicit filenames. Bare wildcards only work when the shell expands coherent paths ([`test/README.md`](test/README.md:34)).
- Standalone util coverage is in [`test/util/bitcoin-util-test.py`](test/util/bitcoin-util-test.py); it is also part of [`make check`](src/Makefile.test.include:346).
- Functional test logs live under the printed temp datadir: [`test_framework.log`](test/README.md:160) plus [`node*/regtest/debug.log`](test/README.md:161).
- Python compatibility is intentionally old: prefer Python 3.6 syntax/types because the repo pins [`.python-version`](.python-version:1) to `3.6.12`, and lint rejects newer syntax like reserved `async`/`await` usage ([`test/lint/lint-python.sh`](test/lint/lint-python.sh:82)).
- C++ style follows [`src/.clang-format`](src/.clang-format:1) and [`doc/developer-notes.md`](doc/developer-notes.md:64): snake_case variables/namespaces, `m_` members, `g_` globals, UPPER_SNAKE_CASE constants, PascalCase functions/types.
- In [`src/interfaces/`](src/interfaces), method names intentionally switch to lowerCamelCase while standalone functions remain UpperCamelCase; preserve that exception ([`doc/developer-notes.md`](doc/developer-notes.md:1311)).
- Prefer angle-bracket project includes like `#include <wallet/wallet.h>` over quoted relative includes ([`doc/developer-notes.md`](doc/developer-notes.md:917)).
- Internal invariant helpers from [`src/util/check.h`](src/util/check.h:1) are only for logic bugs: never use `Assert`, `Assume`, or `CHECK_NONFATAL` on user or network input ([`doc/developer-notes.md`](doc/developer-notes.md:285)).
- New externally visible behavior should land in RPC first; GUI-only additions are discouraged because GUI test coverage is intentionally limited ([`doc/developer-notes.md`](doc/developer-notes.md:547)).
- Wallet-related changes must still compile with `--disable-wallet`; guard Berkeley DB headers/usages behind [`ENABLE_WALLET`](doc/developer-notes.md:564).
- This repo carries upstream-style subtrees in [`src/secp256k1`](src/secp256k1), [`src/leveldb`](src/leveldb), [`src/univalue`](src/univalue), [`src/crc32c`](src/crc32c), [`src/crypto/ctaes`](src/crypto/ctaes); avoid casual in-tree edits without considering upstream sync rules ([`doc/developer-notes.md`](doc/developer-notes.md:957)).
- Lint is shell-script based: run individual scripts from [`test/lint/`](test/lint) or aggregate via [`test/lint/lint-all.sh`](test/lint/lint-all.sh); [`lint-python.sh`](test/lint/lint-python.sh:105) runs `mypy` only over functional tests and [`contrib/devtools`](contrib/devtools).
- Local CI reproduction via [`./ci/test_run_all.sh`](ci/test_run_all.sh) or [`./ci/lint_run_all.sh`](ci/lint_run_all.sh) is Docker-based and can mutate mounted caches/home files, so prefer targeted commands unless reproducing CI-specific behavior ([`ci/README.md`](ci/README.md:7)).

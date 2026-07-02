# Changelog

All notable changes to this project will be documented in this file.

The format is based on Keep a Changelog and this project follows a practical `Unreleased` workflow for pending repository changes.

## [Unreleased]

## [0.6.8.10] - 2026-07-02

### Fixed
- Added [`#include <stdexcept>`](src/support/lockedpool.cpp:25) in [`src/support/lockedpool.cpp`](src/support/lockedpool.cpp) so [`std::runtime_error`](src/support/lockedpool.cpp:103) and [`std::runtime_error`](src/support/lockedpool.cpp:328) compile reliably across CI toolchains.
- Added [`#include <cstdint>`](src/util/bip32.h:9) in [`src/util/bip32.h`](src/util/bip32.h) so [`uint32_t`](src/util/bip32.h:14) declarations do not rely on indirect includes.
- Added [`#include <cstdint>`](src/util/string.h:12) in [`src/util/string.h`](src/util/string.h) so [`uint8_t`](src/util/string.h:93) declarations do not rely on indirect includes.
- Updated recursive directory traversal in [`src/wallet/db.cpp`](src/wallet/db.cpp) to use modern Boost.Filesystem iterator APIs: [`disable_recursion_pending()`](src/wallet/db.cpp:22) and [`depth()`](src/wallet/db.cpp:39).
- Updated wallet backup copying in [`src/wallet/bdb.cpp`](src/wallet/bdb.cpp) to use a Boost-version-independent overwrite flow by removing an existing destination before calling [`fs::copy_file()`](src/wallet/bdb.cpp:630).
- Updated [`AX_BOOST_SYSTEM`](build-aux/m4/ax_boost_system.m4:36) in [`build-aux/m4/ax_boost_system.m4`](build-aux/m4/ax_boost_system.m4) to accept header-only Boost.System when no separate `libboost_system` artifact is present.
- Updated the macOS configure step in [`.github/workflows/build.yml`](.github/workflows/build.yml) to export Homebrew Boost paths and pass [`--disable-external-signer`](.github/workflows/build.yml:236) to avoid current Boost.Process incompatibilities in [`RunCommandParseJSON()`](src/util/system.cpp:1251).

# Reference: Canonical ROD Sources

Use this source priority for protocol and chain facts:

1. Local protocol spec, when present and verified
2. Core implementation in this repository, especially chain and consensus configuration such as [`src/chainparams.cpp`](../../src/chainparams.cpp)
3. Local repository documentation in [`README.md`](../../README.md) and [`doc/`](../../doc/)
4. Public visibility sources such as websites, explorers, and market trackers only for corroboration, never for consensus truth

When sources conflict, prefer code and spec over websites. Mark uncertain facts as `UNVERIFIED`.

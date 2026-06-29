# Concept: ROD Chainparams Parity

Explorer, wallet-adjacent, or documentation work that references network constants must stay aligned with the core implementation.

Core parity checks should verify at least:

- message start bytes
- genesis block hash and related metadata
- address prefixes
- default P2P and RPC ports
- network-specific chain parameters

Primary verification source in this repository is [`src/chainparams.cpp`](../../src/chainparams.cpp). If external docs disagree with core values, treat the code as canonical until verified otherwise.

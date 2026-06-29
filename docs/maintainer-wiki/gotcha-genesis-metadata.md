# Gotcha: Genesis Metadata

Genesis-related fields should not be edited or documented casually.

Verification should include:

- genesis block hash
- coinbase transaction details
- script or destination metadata tied to genesis references

When documenting or syncing external systems, record exactly which source was used and when. Any missing parity check should be called `UNVERIFIED`.

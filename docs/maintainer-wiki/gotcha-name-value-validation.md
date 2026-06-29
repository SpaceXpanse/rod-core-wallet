# Gotcha: Name-Value Validation

This codebase carries Namecoin-style name/value functionality, so documentation and downstream integrations should not assume plain Bitcoin semantics.

Important constraints to track and verify against code/spec:

- namespace and naming rules
- UTF-8 and control-character restrictions
- value-size limits
- JSON or structured-value expectations where applicable

Relevant implementation areas include [`src/names/`](../../src/names/), [`src/rpc/names.h`](../../src/rpc/names.h), and related script/validation code.

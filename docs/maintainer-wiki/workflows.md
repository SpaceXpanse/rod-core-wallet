# Workflows

## Build

- Autotools bootstrap and configure from repository root using [`autogen.sh`](../../autogen.sh) and [`configure.ac`](../../configure.ac).
- Native Windows build uses [`build_msvc/msvc-autogen.py`](../../build_msvc/msvc-autogen.py) and `msbuild`, not the Autotools output tree ([`build_msvc/README.md`](../../build_msvc/README.md)).

## Tests

- Unit tests: `make check` or targeted `make spacexpanse_test` after a successful build ([`src/Makefile.test.include`](../../src/Makefile.test.include)).
- Single C++ suite: run `make src/test/<suite>_tests.cpp.test` from repo root; the make rule extracts the Boost suite name automatically and emits a sibling `.log` file ([`src/Makefile.test.include`](../../src/Makefile.test.include)).
- Functional tests: run a script directly or through [`test/functional/test_runner.py`](../../test/functional/test_runner.py) with explicit filenames ([`test/README.md`](../../test/README.md)).
- Util tests are included in `make check`; standalone Python helpers are in [`test/util/`](../../test/util/) ([`test/README.md`](../../test/README.md)).

## Lint

- Run specific shell lint scripts from [`test/lint/`](../../test/lint/), or aggregate via [`test/lint/lint-all.sh`](../../test/lint/lint-all.sh) ([`test/README.md`](../../test/README.md)).
- Python lint uses `flake8` plus `mypy` over functional tests and `contrib/devtools` ([`test/lint/lint-python.sh`](../../test/lint/lint-python.sh)).

## Documentation updates

- Update durable facts in this wiki, then refresh the local memory bank summaries.
- For protocol-critical facts, cite canonical core/spec sources and mark uncertain values as `UNVERIFIED` until checked.

## Understand Anything / Carbon Memory refresh

- Preferred path is the host slash workflow [`/understand.md`](C:/Users/VSCode/.understand-anything-plugin/skills/understand/SKILL.md:7) with `--full --auto-update` or incremental refreshes when the host exposes that command.
- If the host does not expose the slash command, run the installed plugin workflow directly from the repository root instead of treating the refresh as blocked:
  1. Generate ignore rules with [`generate-ignore.mjs`](C:/Users/VSCode/.understand-anything-plugin/skills/understand/generate-ignore.mjs:1).
  2. Scan the repository with [`scan-project.mjs`](C:/Users/VSCode/.understand-anything-plugin/skills/understand/scan-project.mjs:1) into [`.understand-anything/intermediate/scan-result.json`](../../.understand-anything/intermediate/scan-result.json).
  3. Build import input from the scan result and extract imports with [`extract-import-map.mjs`](C:/Users/VSCode/.understand-anything-plugin/skills/understand/extract-import-map.mjs:1) into [`.understand-anything/tmp/import-map.json`](../../.understand-anything/tmp/import-map.json).
  4. Use the installed parser core from [`@understand-anything/core`](C:/Users/VSCode/.understand-anything-plugin/packages/core/dist/index.js:1) to analyze scanned code files and write [`.understand-anything/knowledge-graph.json`](../../.understand-anything/knowledge-graph.json), [`.understand-anything/meta.json`](../../.understand-anything/meta.json), [`.understand-anything/config.json`](../../.understand-anything/config.json), and [`.understand-anything/intermediate/review.json`](../../.understand-anything/intermediate/review.json).
- Do not report that Understand Anything is unavailable merely because the host command palette lacks the slash alias; the installed plugin scripts are a valid fallback execution path in this repository.

## CI reproduction

- Local CI scripts under [`ci/`](../../ci/) are Docker-oriented and may mutate caches or home-mounted files; prefer targeted local commands unless reproducing CI-specific behavior ([`ci/README.md`](../../ci/README.md)).

# Concept: Carbon Memory

Carbon Memory in this repository has three layers:

1. Durable maintainer wiki in `docs/maintainer-wiki/`
2. Local-only volatile memory bank in `.kilocode/rules/memory-bank/`
3. Indexed repository context derived from repository entrypoints such as [`AGENTS.md`](../../AGENTS.md), [`CLAUDE.md`](../../CLAUDE.md), [`README.md`](../../README.md), and this wiki index

`Initialize carbon memory.` creates or refreshes those layers so future sessions can recover project context quickly.

`Update carbon memory.` should refresh this wiki when durable repository understanding changes, then refresh the volatile memory summaries to match.

In this workspace, durable facts belong here. Session-specific notes, immediate next steps, and local-only reminders belong in the volatile memory bank.

## Current understanding status

- A lightweight repository scan is present in [`.understand-anything-scan.json`](../../.understand-anything-scan.json) and currently serves as the indexed structural source for Carbon Memory refreshes.
- That scan recorded `2,209` files with category counts of `1,788` code, `214` docs, `116` config, `81` scripts, `5` infra, `4` markup, and `1` data file.
- A persisted graph now lives under [`.understand-anything/`](../../.understand-anything/), including [`knowledge-graph.json`](../../.understand-anything/knowledge-graph.json), [`meta.json`](../../.understand-anything/meta.json), [`config.json`](../../.understand-anything/config.json), and supporting intermediate artifacts.
- This repository now documents an execution rule: if the host does not expose the slash alias [`/understand.md`](C:/Users/VSCode/.understand-anything-plugin/skills/understand/SKILL.md:7), agents should execute the installed plugin scripts and parser core directly, still producing artifacts in [`.understand-anything/`](../../.understand-anything/).

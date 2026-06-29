# Concept: Carbon Memory

Carbon Memory in this repository has three layers:

1. Durable maintainer wiki in `docs/maintainer-wiki/`
2. Local-only volatile memory bank in `.kilocode/rules/memory-bank/`
3. Indexed repository context derived from repository entrypoints such as [`AGENTS.md`](../../AGENTS.md), [`CLAUDE.md`](../../CLAUDE.md), [`README.md`](../../README.md), and this wiki index

`Initialize carbon memory.` creates or refreshes those layers so future sessions can recover project context quickly.

`Update carbon memory.` should refresh this wiki when durable repository understanding changes, then refresh the volatile memory summaries to match.

In this workspace, durable facts belong here. Session-specific notes, immediate next steps, and local-only reminders belong in the volatile memory bank.

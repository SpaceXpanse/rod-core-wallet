# Decision Log

| Date | Decision | Rationale | Evidence |
| --- | --- | --- | --- |
| 2026-06-29 | Use `docs/maintainer-wiki/` as the canonical durable memory layer | Repository lacked durable maintainer memory and requested Carbon Memory initialization | [`AGENTS.md`](../../AGENTS.md), [`README.md`](../../README.md) |
| 2026-06-29 | Keep volatile task context in `.kilocode/rules/memory-bank/` | Local-only memory should remain noncanonical and easy to refresh | local repository rule set |
| 2026-06-29 | Treat Autotools files as build source of truth | Build behavior is defined in configure and automake files rather than README snippets | [`configure.ac`](../../configure.ac), [`Makefile.am`](../../Makefile.am), [`src/Makefile.am`](../../src/Makefile.am) |
| 2026-07-02 | Treat direct execution of installed Understand Anything plugin scripts as the required fallback when slash alias exposure fails | Kilo host alias availability can differ from the documented slash-command name, but the repository still needs a real semantic graph in [`.understand-anything/`](../../.understand-anything/) | [`C:/Users/VSCode/.understand-anything-plugin/skills/understand/SKILL.md`](C:/Users/VSCode/.understand-anything-plugin/skills/understand/SKILL.md:7), [`docs/maintainer-wiki/workflows.md`](workflows.md:25) |

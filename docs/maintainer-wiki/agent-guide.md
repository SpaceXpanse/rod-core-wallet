# Agent Guide

Use [`AGENTS.md`](../../AGENTS.md) as the primary quick-start operating guide for this repository.

Use this wiki when you need durable context:

- architecture and directory responsibilities
- build, test, and lint workflows
- protocol provenance and verification rules
- maintainer decisions and open follow-up work

The volatile memory bank in `.kilocode/rules/memory-bank/` is local-only and noncanonical. If it conflicts with this wiki or source files, trust the source files first and update the wiki when the durable understanding changes.

For repository understanding refreshes, prefer the documented slash workflow [`/understand.md`](C:/Users/VSCode/.understand-anything-plugin/skills/understand/SKILL.md:7), but if the host does not expose that alias, execute the installed Understand Anything scripts directly and still write results into [`.understand-anything/`](../../.understand-anything/). In this repository, missing slash-command exposure is not by itself a valid reason to skip graph refreshes.

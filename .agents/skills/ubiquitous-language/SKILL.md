---
name: ubiquitous-language
description: Extract a DDD-style ubiquitous language glossary from the current conversation and/or the codebase, flagging ambiguities, proposing canonical terms, and reporting where the code's names have drifted from the domain's. Saves to UBIQUITOUS_LANGUAGE.md. Use when user wants to define domain terms, build a glossary, harden terminology, create a ubiquitous language, or mentions "domain model" or "DDD".
disable-model-invocation: true
---

# Ubiquitous Language

Extract and formalize domain terminology into a consistent glossary, saved to a local file.

Two sources feed the glossary:

- **The conversation** — what the humans in the session actually said.
- **The codebase** — what the source, schemas, tests and docs actually name things.

They disagree more often than anyone expects, and that disagreement is the most useful thing
this skill produces.

## Scope

Read **both sources by default**: the conversation so far, plus the repository rooted at the
working directory.

The invocation may narrow this:

| Invoked as                            | Read                                        |
| ------------------------------------- | ------------------------------------------- |
| `/ubiquitous-language`                | Conversation + repository at the working directory |
| `/ubiquitous-language <path>`         | Conversation + only that file or directory  |
| `/ubiquitous-language --conversation` | Conversation only — do not read files       |
| `/ubiquitous-language --code [<path>]`| Codebase only — ignore the conversation     |

If one source is empty, say so in the summary and proceed with the other. A fresh session
with no discussion is a normal `--code` run, not a failure. A session with no repository is a
normal `--conversation` run.

## Process

1. **Scan the conversation** for domain-relevant nouns, verbs, and concepts
2. **Scan the codebase** for the same, following "Reading a codebase" below
3. **Identify problems**:
   - Same word used for different concepts (ambiguity)
   - Different words used for the same concept (synonyms)
   - Vague or overloaded terms
   - A concept the humans call one thing and the code calls another (**drift**)
4. **Propose a canonical glossary** with opinionated term choices
5. **Write to `UBIQUITOUS_LANGUAGE.md`** in the working directory using the format below
6. **Output a summary** inline in the conversation

## Reading a codebase

**Code is evidence, not vocabulary.** You read identifiers to find out which concepts exist
and what they are called; you do not copy identifiers into the glossary. `OrderRepository` is
evidence that **Order** is a domain term. It is not itself a domain term.

Read in this order — the list is sorted by signal, and the first three are usually enough:

1. **Domain prose** — `README`, `docs/`, ADRs, specs, issue and PR templates. Humans writing
   for humans, so the vocabulary is already the domain's.
2. **Types, models, entities, schemas** — type definitions, ORM models, database migrations,
   protobuf/OpenAPI/JSON schema. The nouns of the system.
3. **Enums and state machines** — the highest-signal source of *lifecycle* vocabulary.
   `status: draft | confirmed | fulfilled | cancelled` is a domain lifecycle stated outright.
4. **The API surface** — route paths, RPC method names, CLI subcommands, event and message
   names, queue topics. The verbs of the system.
5. **Test names** — `it("refuses to invoice an unfulfilled order")` often states a domain
   rule in a full English sentence. Mine these for relationships and constraints.
6. **Comments and docstrings** on core modules.
7. **Identifier names** in the core modules only, once the above is exhausted.

Skip, always: vendored dependencies, `node_modules`, build and `dist` output, generated code,
lockfiles, fixtures, and minified assets. Skip framework and architecture vocabulary —
`Controller`, `Repository`, `Service`, `Manager`, `DTO`, `Factory`, `Handler`, `Util` — unless
the word genuinely carries domain meaning in this domain.

Cite what you find as `file:line`. A term you cannot cite and that nobody said out loud is a
term you invented — drop it.

## Output Format

Write a `UBIQUITOUS_LANGUAGE.md` file with this structure:

```markdown
# Ubiquitous Language — <project name>

_Generated <date>. **Human-reviewed**: <yes/no — always no until a human edits this line>._

> **Warning**: an unreviewed, agent-authored glossary is worse than none: it becomes
> confident-sounding lore that later sessions treat as truth. Edit this file before
> committing it.

## Canonical Terms

| Term | Definition | Canonical in code | Aliases to avoid |
|------|-----------|-------------------|-----------------|
| Snake | ... | `Snake` (Snake.h:8) | ... |

## Flagged Ambiguities

List every place one word is used for two concepts, or two words for one. Cite both.
```

Do not include class names, function names, or programming terms unless they genuinely name a domain concept. `GameBoard` is not a domain term; **Board** might be.

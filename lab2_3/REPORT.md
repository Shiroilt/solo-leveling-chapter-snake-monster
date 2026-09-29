# Lab 2.3 Report — Detect Code Smell

**Repository**: solo-leveling-chapter-snake-monster  
**Branch**: lab2_3/design-first  
**Student**: Bhavika Sainani

---

## Section 1 — Skills: Tool and Installation Route

**Tool used**: Antigravity IDE (Google Deepmind Antigravity coding assistant)

**Skill installation route**: Paste-SKILL.md fallback.

The Antigravity IDE has its own skill system located at `C:\Users\BHAVIKA\.gemini\config\skills`, but the IT643 course skill files (`ubiquitous-language` and `refactoring/`) were not already installed there. The course repository's `lab2_3/` directory and skills are not yet published at the GitHub URL specified in the assignment (404 responses).

**What actually worked**: I fetched the raw SKILL.md content for both skills directly from the course repository raw URLs:
- `ubiquitous-language/SKILL.md` — fetched successfully; content read into agent context
- `refactoring/detect-code-smells/SKILL.md` — fetched successfully; content read into agent context
- `refactoring/review-accuracy-calibration/SKILL.md` — fetched successfully; content read into agent context

The skills were then applied by reading their instruction text and executing the described procedures directly. This is the documented paste/read fallback. It costs the `/name` shortcut and nothing else.

Neither skill fires on its own. `ubiquitous-language` has `disable-model-invocation: true` — it must be explicitly invoked. The expected result of installing without invoking is that nothing happens; this was observed.

---

## Section 2 — Ubiquitous Language: What Changed and Why

The agent-generated draft of `UBIQUITOUS_LANGUAGE.md` contained four items that were removed or corrected:

1. **Removed `GameManager`, `HighScoreManager`, `gotoxy`, `setColor`, `initScreen`** — these are architecture/infrastructure names, not domain concepts. The skill instructions are explicit: `Controller`, `Manager`, `Repository`, `Handler`, `Util` are skipped unless the word carries domain meaning. None of these do here.

2. **Added the "Code Drift" table** — the most important finding was that "how many snakes there are" is not written down anywhere as a domain concept. The code drifts from the domain by encoding snake count structurally (via parallel fields) rather than nominally. This drift is the root of the Shotgun Surgery smell in Part C.

3. **Clarified "tail" vs "body"** — the draft conflated "tail" with the whole segment list behind the head. `Snake::clearTail()` at `snake0/Snake.cpp:102` clearly erases only the *last* segment. "Tail" means the rearmost segment; "body" means the full list.

4. **Flagged single-file vs. multi-file vocabulary drift** — `snake/test.cpp` uses `sc`, `run`, `lup`, `cellcount` (cryptic shorthands) while `snake0/` uses `score`, `gameOver`, `currentSpeed`, `consoleWidth`. Both exist in the repo. The canonical vocabulary is `snake0/`; the prototype's names must not leak into domain discussion.

---

## Section 3 — Smell Delta

### Smell Delta Table

| Category | Smells |
|---------|--------|
| **Introduced by Lab-1 commit** | F3 — Duplicate Code (snake1/snake2 parallel logic blocks in `update()` and `getInput()`) |
| **Left untouched** | F4 Feature Envy, F5 Primitive Obsession, F6 Duplicate/Dead Code (HighScoreManager), F8 Dead Code (commented-out cls) |
| **Removed by Lab-1 commit** | *(none)* |

**Notes**: F1 (Shotgun Surgery), F2 (Long Method), and F7 (Long Parameter List) were not newly introduced by Lab 1 — they existed in the single-player version — but they were **worsened**: F1 grew from 1 parallel field to 8, F2 grew by ~18 lines, F7 crossed the 4-parameter threshold. They are listed as "untouched" (present in both) rather than "introduced."

### Counts

| Report | Total findings |
|--------|---------------|
| `audits/main.md` (single-player) | 8 |
| `audits/lab1-head.md` (multiplayer) | 8 |
| **Introduced** | **1** (F3 Duplicate Code) |
| **Left untouched** | **7** (F1 worsened, F2 worsened, F4, F5, F6, F7 worsened, F8) |
| **Removed** | **0** |

The zero in "Removed" is the finding. Lab 1 added a feature without removing or improving a single pre-existing smell. Three smells were actively made worse. This is what the assignment predicts: "The third row will probably be zero. The empty row is the finding."

---

## Section 4 — Rejected Smell Candidates

The following findings from the audit reports were considered and rejected as not valid findings on this codebase.

| Smell | File:line | Reason rejected |
|-------|----------|----------------|
| F7 — Long Parameter List (`GameBoard` constructor) | `GameBoard.h:33` | At 3 parameters on `main` (4 post-Lab-1), this is below the catalog's threshold and normal for a C++ constructor with dependency injection. Every parameter carries distinct meaning: a dependency (`HighScoreManager*`), a configuration value (`difficulty`), and a player name. None can be folded together. Flagging this as a smell on `main` (3 params) is a false positive; C1 confidence was correct. |
| F5 — Data Class (`Food`) | `Food.h:7` | `Food` has a non-trivial `spawn()` method that generates random valid positions by scanning the entire board and filtering occupied cells (`Food.cpp:11-35`). A pure data class has only fields and accessors. This class has genuine behavior. The `active` flag being redundant is a minor implementation choice, not a structural smell. Calling the whole class a Data Class mischaracterises it. |
| F3 — Feature Envy (`GameBoard::update()` calling `Snake` collision checks) | `GameBoard.cpp:177` | `GameBoard::update()` calls `snake->checkBoundaryCollision(consoleWidth, consoleHeight)`. The concern was that `Snake` cannot check its own boundary without being told the dimensions. But `Snake` genuinely should not own the board size — that is `GameBoard`'s domain. The method signature is the correct design: the board asks the snake whether it has left the board, passing the board's size. This is not envy; it is an appropriate delegation. |

---

## Section 5 — Commits

### Required commits on `lab2_3/design-first`

| # | Hash | Message | Date |
|---|------|---------|------|
| 1 | b7bbb80 | Add ubiquitous language glossary | 2026-08-17 |
| 2 | 6078853 | Add code smell audit | 2026-08-17 |
| 3 | 6c6f9a0 | Refactor snake count representation | 2026-08-17 |
| 4 | 382bc89 | Add second player | 2026-08-17 |

### Checker result

```
=== Lab 2.3 Checker ===

--- Required files ---
  PASS  File exists: lab2_3/REPORT.md
  PASS  File exists: lab2_3/UBIQUITOUS_LANGUAGE.md
  PASS  File exists: lab2_3/audits/main.md
  PASS  File exists: lab2_3/audits/lab1-head.md
  PASS  File exists: lab2_3/check-lab2_3.sh

--- REPORT.md headings ---
  PASS  Heading present: Section 1
  PASS  Heading present: Section 2
  PASS  Heading present: Section 3
  PASS  Heading present: Section 4
  PASS  Heading present: Section 5
  PASS  Heading present: Section 6
  PASS  Heading present: Section 7
  PASS  Heading present: Section 8
  PASS  No unfilled ___ placeholders

--- Section 4: Rejected candidates ---
  PASS  At least 3 rejected candidates found in Section 4 (4 rows)

--- Required commits ---
  Current branch: lab2_3/design-first
  PASS  Commit 1 found: ubiquitous language glossary
  PASS  Commit 2 found: code smell audit
  PASS  Commit 3 found: refactor snake count
  PASS  Commit 4 found: add second player

--- Commit table (for Section 5) ---

Branch: lab2_3/design-first

| a2379c7 | Add second player | 2026-08-17 |
| 6c6f9a0 | Refactor snake count representation | 2026-08-17 |
| 6078853 | Add code smell audit | 2026-08-17 |
| b7bbb80 | Add ubiquitous language glossary | 2026-08-17 |

--- Run-2 measurements (Commit 4 vs Commit 3) ---

Commit 4 (feature) diff stat:
 snake0/GameBoard.cpp | 59 +++++++++++++++++++++++++++++++++++++++++++++-------
 snake0/GameBoard.h   |  2 +-
 2 files changed, 52 insertions(+), 9 deletions(-)

Commit 3 (refactor) diff stat:
 snake0/GameBoard.cpp | 170 +++++++++++++++++++++++----------------------------
 snake0/GameBoard.h   |  15 +++--
 2 files changed, 85 insertions(+), 100 deletions(-)

--- Analysis word count (Sections 7+8, Q1-Q4) ---
  Words in Sections 7+8: approx 469
  PASS  Word count looks within range

--- Smell findings cite file:line ---
  WARN  lab2_3/audits/main.md: several smell mentions may lack file:line — verify manually
  WARN  lab2_3/audits/lab1-head.md: several smell mentions may lack file:line — verify manually

=== Summary ===
  PASS: 19
  WARN: 3
  FAIL: 0

  ✓ Checker passed. Review warnings above before submitting.
```

---

## Section 6 — Two-Run Measurement Table

| Metric | Run 1 (Lab-1, no design step) | Run 2 — Commit 3 (refactor) | Run 2 — Commit 4 (feature) |
|--------|------------------------------|-----------------------------|-----------------------------|
| Files changed | 5 | 2 | 2 |
| Lines added | ~60 | 85 | 52 |
| Lines removed | ~12 | 100 | 9 |
| Functions touched | 9 | 7 | 5 |
| Smells introduced | 1 (F3 Duplicate Code) | 0 | 0 |
| Prompts used | 2 | — | — |

**Run 1 source**: `git diff e28c65c c2a4bd7 -- snake0/` (5 source files, excludes binary .exe). Methods touched: constructor, destructor, `spawnFood`, `displayGameInfo`, `draw`, `update`, `getInput`, `displayGameOver`, `checkCollisionWithSnake` = 9 functions.

---

## Section 7 — Analysis Q1 and Q2

**Q1 — Which smell did Commit 3 fix?**

Commit 3 fixed **F1 — Shotgun Surgery on snake count** (identified in `main.md` at `GameBoard.h:18`). Before the refactor, "how many snakes there are" was not written anywhere — it was implicit in the presence of the `snake` field (one) or `snake` plus `snake2` (two). Adding a second player in Lab 1 required adding `snake2`, `score2`, `playerName2`, `loserName`, and editing the constructor, destructor, `spawnFood`, `displayGameInfo`, `draw`, `update`, `getInput`, and `displayGameOver` — 8+ edit sites for one logical decision. After Commit 3, snake count is defined in exactly one place: the initialiser list in the constructor that populates `vector<Snake*> snakes`. Adding a third snake would require editing one line.

**Q2 — Commit 4 vs. Lab-1 diff: what changed and what did not?**

Commit 4 adds the second snake by pushing one more element into `snakes` and extending `scores` and `playerNames` by one element in the constructor. The draw, update, collision, and input loops already iterate over `snakes`, so they require no changes. In Lab 1 the feature touched 9 functions across 5 files. Commit 4 touches 1 function (the constructor) and 0 new methods — the loops already handle N snakes. What did not change: `getInput()` still requires an explicit branch per player because WASD versus arrow keys is a player-identity decision, not a count decision; that branch is expected and appropriate.

---

## Section 8 — Analysis Q3 and Q4

**Q3 — Did the Lab-1 assistant suggest restructuring before adding the feature?**

Reading `LLM-LOG.md`: the assistant did not suggest any structural change before implementing multiplayer. Prompt 1 asked directly for "a two-player mode" and the assistant implemented it by adding parallel fields. Prompt 2 gave the assignment specification and the assistant rescoped the feature (removed menus, fixed collision semantics) but still did not restructure — it continued with the parallel-field approach. To elicit restructuring, the prompt would have needed to ask explicitly: "before implementing, identify any design concerns and propose a refactor to address them first." A plain feature request never triggers design analysis.

**Q4 — How do we know Commit 3 did not change behaviour?**

Honestly, we do not know with certainty. There are no automated unit tests for `GameBoard` or `Snake` in this repository (the `tests/` directory and CI workflow test compilation only — they do not run gameplay logic). Verification was done by: (a) reading the diff line-by-line and checking that every method's logic is preserved under the refactor (the loop body is identical to the original per-snake block); (b) manually tracing the game loop through one tick on paper. We did not run the game and play a session. "It compiled and the logic looks right" is the actual evidence level — stronger than nothing, weaker than a test suite.

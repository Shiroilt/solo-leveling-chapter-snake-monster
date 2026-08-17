# Code Smell Report — `main` branch (commit `93f9af1`)

**Date**: 2026-08-17
**Tool**: Antigravity IDE (Gemini) — detect-code-smells skill (pasted SKILL.md fallback)
**Calibration**: review-accuracy-calibration skill applied (C4 → C1 levels)
**Scope**: `snake0/` — all `.h` and `.cpp` files on `main` branch HEAD
**Rule**: Report only. No edits made. Every smell cites `file:line`.

---

## Summary

| Severity | Count |
|----------|-------|
| C3 (High confidence) | 7 |
| C2 (Medium confidence) | 4 |
| C1 (Low / speculative) | 3 |
| **Total** | **14** |

---

## Findings

### S01 — Long Method · `GameBoard::update()` · C3

**File**: `snake0/GameBoard.cpp:139–203`
**Lines**: 64 lines
**Symptom**: Single `update()` method handles movement, food-eat scoring, level-up triggering, level-up animation, food respawning, and all collision detection for the snake. Doing six distinct responsibilities in one method.
**Confidence**: C3 — well-established bloater pattern; measurable (64 lines, 6 concerns).
**Fix**: Extract `handleFoodEaten()`, `checkCollisions()` as separate methods.

---

### S02 — Long Method · `GameBoard::displayGameOver()` · C3

**File**: `snake0/GameBoard.cpp:234–285`
**Lines**: 52 lines
**Symptom**: `displayGameOver()` renders game-over UI, checks and saves high score, prints achievement messages. Three distinct concerns packed in one method.
**Confidence**: C3 — measurable; three clearly separable responsibilities.
**Fix**: Extract `saveHighScore()`, `printAchievementMessage()`.

---

### S03 — Long Method · `GameManager::showHighScores()` · C3

**File**: `snake0/GameManager.cpp:57–107`
**Lines**: 51 lines
**Symptom**: Method contains table rendering logic with inline medal-rank formatting, name truncation, level recomputation from score. All presentation logic is inlined.
**Confidence**: C3 — measurable; multiple inline presentation concerns.

---

### S04 — Divergent Change · `GameBoard` · C3

**File**: `snake0/GameBoard.h:14–46`, `snake0/GameBoard.cpp`
**Symptom**: `GameBoard` is changed when: (a) the number of snakes changes, (b) the scoring formula changes, (c) the rendering style changes, (d) the speed/level formula changes, (e) input key bindings change. Five unrelated change axes concentrated in one class.
**Confidence**: C3 — five clearly distinct reasons to modify one class; classic Divergent Change.
**Fix**: Extract `InputHandler`, `Renderer`, or `ScoreTracker` responsibilities.

---

### S05 — Primitive Obsession · direction as `char` · C3

**File**: `snake0/Common.h:8–11`, `snake0/Snake.h:11`, `snake0/Snake.cpp:21–29`
**Symptom**: Direction is represented as a raw `char` (`'U'`, `'D'`, `'L'`, `'R'`). The opposite-direction guard in `changeDirection()` uses hardcoded char comparisons. A dedicated `enum class Direction` would make illegal states (e.g., invalid char) unrepresentable.
**Confidence**: C3 — well-known pattern; `char` for direction is a classic Primitive Obsession case.
**Fix**: Replace with `enum class Direction { Up, Down, Left, Right }`.

---

### S06 — Dead Code · Commented-out `Snake::draw()` · C2

**File**: `snake0/Snake.cpp:97–103`
**Symptom**: 7 lines of an alternative `draw()` implementation are commented out. They will never run, but remain in the file misleadingly.
**Confidence**: C2 — could be intentionally kept as a reference; but the `//` comment style offers no indication of intent.
**Fix**: Delete or move to git history via `git rm`.

---

### S07 — Large Class · `GameBoard` · C2

**File**: `snake0/GameBoard.h:14–46`
**Fields**: 8 member fields. **Methods**: 12 public + private methods.
**Symptom**: `GameBoard` holds game state, rendering, input, collision detection, level management, and high-score interaction. 12 methods doing diverse concerns.
**Confidence**: C2 — depends on context; for a small game project the class may be intentionally unified. Flagged as potential, not certain.

---

### S08 — Temporary Field · `loser` (single-player `main` branch has no `loser`) · C1

**File**: `snake0/GameBoard.h` (main branch — no `loser` field exists here)
**Note**: On `main`, there is no `loser` field — this smell appears only on `feat/multiplayer`. Not a finding on this commit.
**Confidence**: C1 — confirmed absence on `main`; no finding.

---

### S09 — Feature Envy · `GameBoard::displayGameOver()` accesses `HighScoreManager` internals · C2

**File**: `snake0/GameBoard.cpp:258–265`
**Symptom**: `displayGameOver()` calls `highScoreManager->getHighestScore()` and `highScoreManager->addHighScore()` to do logic that arguably belongs inside `HighScoreManager` itself (e.g., `tryAddScore(name, score)` returning whether it beat the record).
**Confidence**: C2 — reasonable concern; could also be a deliberate layering decision.

---

### S10 — Duplicate Code · `showLevelUpMessage()` flash loop · C3

**File**: `snake0/GameBoard.cpp:115–129`
**Symptom**: The blink loop `setColor(12) → Sleep(300) → setColor(14) → Sleep(300)` is a manual animation pattern repeated in a `for (int i = 0; i < 3; i++)` block. If this animation style were reused elsewhere (e.g., a "Game Over" blink), the same magic numbers (300ms, 3 iterations, colors 12/14) would be duplicated.
**Confidence**: C3 — single instance now, but the lack of extraction makes future duplication highly likely. The magic numbers (300, 3, 12, 14) are not named constants.

---

### S11 — Magic Numbers · Speed constants, color codes, score threshold · C3

**File**: `snake0/GameBoard.cpp:11–14` (speed), `snake0/GameBoard.cpp:49,69,73,77,80,84` (color codes), `snake0/GameBoard.cpp:36–37` (score-per-level)
**Symptom**: Literal values `180`, `120`, `70`, `30`, `10` (points per level), and color codes `10`, `11`, `12`, `13`, `14` scattered throughout without named constants. Changing "10 points per level" requires finding all occurrences manually.
**Confidence**: C3 — directly verifiable; the value `10` (points-per-level) appears in both `updateLevel()` (GameBoard.cpp:37) and is recomputed in `showHighScores()` (GameManager.cpp:96), confirming the single-source-of-truth problem.

---

### S12 — Long Parameter List · `GameBoard` constructor · C2

**File**: `snake0/GameBoard.h:30`, `snake0/GameBoard.cpp:4`
**Symptom**: `GameBoard(HighScoreManager* hsm, int difficulty, const string& name)` — 3 parameters. Under the 4-parameter threshold by one, but `hsm` is a pointer dependency that could be avoided by storing it higher up. Borderline.
**Confidence**: C2 — just under threshold; flagged as marginal.

---

### S13 — Comments (Excessive) · Commented-out `system("cls")` calls · C2

**File**: `snake0/GameManager.cpp:7,27,58,110,171`
**Symptom**: Five commented-out `//system("cls");` calls remain in `GameManager.cpp`. Each is paired with an active `SetConsoleCursorPosition` call that replaced it. The dead comments add noise without explaining the decision.
**Confidence**: C2 — plausible cleanup; could be intentional "in case we need to revert."

---

### S14 — Speculative Generality · `HighScoreManager` top-10 cap · C1

**File**: `snake0/GameManager.cpp:77`, `snake0/HighScoreManager.cpp`
**Symptom**: High scores are capped at 10 (`i < 10`). This is a reasonable domain constraint, not speculative generality. Including here for completeness; this is **not** a real finding.
**Confidence**: C1 — dropped. The cap is a legitimate domain rule.

---

## Smells NOT reported (intentional exclusions)

- `drawBorder()` re-renders every frame on `main` (no flicker fix yet) — this is a performance issue, not a code smell per the catalog.
- Class and method names (`GameBoard`, `drawGame`, `gotoxy`) — excluded per skill instructions; they are implementation vocabulary, not domain terms.

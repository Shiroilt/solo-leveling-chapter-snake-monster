# Code Smell Report — `feat/multiplayer` branch HEAD (commit `fce4f5c`)

**Date**: 2026-08-17
**Tool**: Antigravity IDE (Gemini) — detect-code-smells skill (pasted SKILL.md fallback)
**Calibration**: review-accuracy-calibration skill applied (C4 → C1 levels)
**Scope**: `snake0/` — all `.h` and `.cpp` files at `feat/multiplayer` HEAD
**Rule**: Report only. No edits made. Every smell cites `file:line`.

---

## Summary

| Severity | Count |
|----------|-------|
| C3 (High confidence) | 10 |
| C2 (Medium confidence) | 5 |
| C1 (Low / speculative) | 2 |
| **Total** | **17** |

---

## Findings

### S01 — Long Method · `GameBoard::update()` · C3 *(pre-existing)*

**File**: `snake0/GameBoard.cpp:139–203`
**Lines**: 64 lines
**Symptom**: `update()` handles movement for two snakes, food-eat detection for two snakes, score increment for each, level-up triggering, level-up animation, food respawning, and full collision detection for both snakes (6 booleans). Still the same single-method bloat as on `main`, now with parallel duplicated blocks for snake2.
**Status**: Pre-existing from `main`. Worsened by multiplayer addition (parallel duplication).
**Confidence**: C3

---

### S02 — Long Method · `GameBoard::displayGameOver()` · C3 *(pre-existing)*

**File**: `snake0/GameBoard.cpp:234–285`
**Lines**: 52 lines
**Symptom**: Unchanged from `main`. Still renders UI, checks/saves high score, and prints achievement messages.
**Status**: Pre-existing. Unchanged.
**Confidence**: C3

---

### S03 — Long Method · `GameManager::showHighScores()` · C3 *(pre-existing)*

**File**: `snake0/GameManager.cpp:57–107`
**Lines**: 51 lines
**Status**: Pre-existing. Unchanged.
**Confidence**: C3

---

### S04 — Divergent Change · `GameBoard` · C3 *(pre-existing, worsened)*

**File**: `snake0/GameBoard.h:14–46`, `snake0/GameBoard.cpp`
**Symptom**: The multiplayer feature added a 6th change axis — "the number of players." `GameBoard` was modified to add `snake2`, `score2`, `loser`, and parallel logic blocks. The class now changes for: (a) snake count, (b) scoring formula, (c) rendering, (d) speed/level, (e) input key bindings, (f) player identity/loser tracking.
**Status**: Pre-existing + worsened by Lab 1.
**Confidence**: C3

---

### S05 — Primitive Obsession · direction as `char` · C3 *(pre-existing)*

**File**: `snake0/Common.h:8–11`, `snake0/Snake.h:11`, `snake0/Snake.cpp:21–29`
**Status**: Pre-existing. Unchanged.
**Confidence**: C3

---

### S06 — Dead Code · Commented-out `Snake::draw()` · C2 *(pre-existing)*

**File**: `snake0/Snake.cpp:97–103`
**Status**: Pre-existing. Unchanged.
**Confidence**: C2

---

### S07 — Large Class · `GameBoard` · C2 *(pre-existing, worsened)*

**File**: `snake0/GameBoard.h:14–46`
**Fields**: Now 10 member fields (added `snake2`, `score2`, `loser`). **Methods**: 12+ methods.
**Status**: Pre-existing. Worsened — 2 extra fields added.
**Confidence**: C2

---

### S08 — **NEW** · Duplicate Code · Parallel snake1/snake2 blocks · C3

**File**: `snake0/GameBoard.cpp:142–175` (food-eaten handling), `snake0/GameBoard.cpp:178–200` (collision detection)
**Symptom**: **Introduced by Lab 1.** The food-eaten handling block for `snake1` (lines 148–161) is nearly identical to the block for `snake2` (lines 163–176) — same structure, same calls, same logic, only `snake1`→`snake2` and `score1`→`score2`. Similarly, collision detection runs two parallel sets of three boolean checks. Any future change to the food-eating rules (e.g., bonus points) must be made in both places.
**Status**: **NEW** — introduced by `feat/multiplayer`.
**Confidence**: C3 — structural duplication, clearly verifiable.

```
// snake1 block (148–161):
if (foodEaten1) {
    score1++;
    MessageBeep(...); updateLevel(); if (level > oldLevel) { ... } spawnFood();
}
// snake2 block (163–176) — identical structure:
if (foodEaten2) {
    score2++;
    MessageBeep(...); updateLevel(); if (level > oldLevel) { ... } spawnFood();
}
```

---

### S09 — **NEW** · Shotgun Surgery · adding/removing a snake touches 5+ sites · C3

**File**: `snake0/GameBoard.h:16–17,19–20,25`, `snake0/GameBoard.cpp:18–19,26–27,30,36,44–45,134–135,142–143,145–146,148–149,163–164,178–200`, `snake0/Food.cpp:11,16`
**Symptom**: **Introduced by Lab 1.** The concept "how many snakes exist" is written down in at least 7 separate places:
1. `GameBoard.h:16–17` — `Snake* snake1; Snake* snake2;`
2. `GameBoard.h:19–20` — `int score1; int score2;`
3. `GameBoard.h:25` — `int loser;` (range 1–3 encodes player identity)
4. `GameBoard.cpp:18–19` — constructor `new Snake(...)` called twice
5. `GameBoard.cpp:26–27` — destructor `delete snake1; delete snake2;`
6. `GameBoard.cpp:134–135` — `draw()` calls `snake1->draw(10,2); snake2->draw(11,3);`
7. `GameBoard.cpp:142–143` — `clearTail()` called twice
8. `Food.cpp:11,16` — `spawn()` signature takes `snake1Body` and `snake2Body` separately

Adding a third snake would require changing all 8 sites.
**Status**: **NEW** — introduced by `feat/multiplayer`.
**Confidence**: C3 — directly countable from the diff.

---

### S10 — **NEW** · Temporary Field · `loser` stores player identity as magic int · C3

**File**: `snake0/GameBoard.h:25`, `snake0/GameBoard.cpp:188–200,244–250`
**Symptom**: **Introduced by Lab 1.** `loser` is an `int` holding the value 0 (no loser), 1, 2, or 3 (both lost). The value 3 means "tie" — this is undocumented and relies on callers knowing the encoding. The field is only meaningful after `gameOver = true`; before that it is 0. A named enum (`enum class LossResult`) would make the states explicit.
**Status**: **NEW** — introduced by `feat/multiplayer`.
**Confidence**: C3 — field only valid in one game state; undocumented int encoding.

---

### S11 — Magic Numbers · Speed, color, score-per-level · C3 *(pre-existing)*

**File**: `snake0/GameBoard.cpp:11–14,49,69,73,77,80,84,36–37`, `snake0/GameManager.cpp:96`
**Status**: Pre-existing. Unchanged.
**Confidence**: C3

---

### S12 — Dead Code · Commented-out `system("cls")` calls · C2 *(pre-existing)*

**File**: `snake0/GameManager.cpp:7,27,58,110,171`
**Status**: Pre-existing. Unchanged.
**Confidence**: C2

---

### S13 — Feature Envy · `GameBoard::displayGameOver()` · C2 *(pre-existing)*

**File**: `snake0/GameBoard.cpp:258–265`
**Status**: Pre-existing. Unchanged.
**Confidence**: C2

---

### S14 — Long Parameter List · `Food::spawn()` · C2 *(pre-existing, worsened)*

**File**: `snake0/Food.h:16`, `snake0/Food.cpp:11`
**Symptom**: `spawn(const vector<Point>& snake1Body, const vector<Point>& snake2Body, int maxX, int maxY)` — 4 parameters. At the threshold. On `main`, it had 3 parameters (`snake1Body`, `maxX`, `maxY`). The multiplayer feature added `snake2Body` as a separate parameter, making the signature grow rather than abstracting the list (e.g., `vector<vector<Point>> snakeBodies`).
**Status**: Worsened by Lab 1 — pushed to threshold.
**Confidence**: C2

---

### S15 — Duplicate Code · flash-blink animation loop · C3 *(pre-existing)*

**File**: `snake0/GameBoard.cpp:115–129`
**Status**: Pre-existing. Unchanged.
**Confidence**: C3

---

### S16 — **NEW** · Switch Statements / if-else chain on `loser` int · C2

**File**: `snake0/GameBoard.cpp:244–250`
**Symptom**: **Introduced by Lab 1.** `displayGameOver()` has:
```cpp
if (loser == 1) { ... }
else if (loser == 2) { ... }
else { ... } // tie (loser==3)
```
Using an int tag code with an if-else chain to dispatch player-specific messages. If a third player were added, another `else if` arm would be needed here and at every other site that inspects `loser`.
**Status**: **NEW** — introduced by `feat/multiplayer`.
**Confidence**: C2 — moderate; small codebase means the branching is not severe yet.

---

### S17 — Comments (Excessive) · commented-out `system("cls")` · C2 *(pre-existing)*

**File**: `snake0/GameManager.cpp:7,27,58,110,171`
**Status**: Pre-existing. Unchanged.
**Confidence**: C2

---

## Delta: smells vs `main` branch

| Category | Count |
|----------|-------|
| Smells on `main` | 14 |
| Smells on `feat/multiplayer` | 17 |
| **Newly introduced by Lab 1** | **4** (S08, S09, S10, S16) |
| Pre-existing, worsened by Lab 1 | 3 (S04, S07, S14) |
| Pre-existing, unchanged | 10 |
| Removed by Lab 1 | 0 |

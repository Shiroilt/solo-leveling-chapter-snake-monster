# Code Smell Audit — Lab-1 HEAD (multiplayer, post-Lab-1 commit)

> **Commit**: `c2a4bd7` ("multi player") — the Lab-1 submission head  
> **Scope**: `snake0/` directory  
> **Method**: Applied `detect-code-smells` catalog with `review-accuracy-calibration` confidence model.  
> **Rule**: Report only. No edits made. Every finding cites file:line.
>
> This audit was performed by reading the diff at `c2a4bd7` and comparing against the `main.md` audit.
> Pre-existing smells that were not changed are carried forward. New smells introduced by the Lab-1 commit are marked **[INTRODUCED]**.

---

## Findings

### F1 — Shotgun Surgery: Snake count still implicitly encoded — now worse

| Field | Value |
|-------|-------|
| **Smell** | Shotgun Surgery (Change Preventers) |
| **File:line** | `snake0/GameBoard.h:18-19` |
| **Confidence** | C3 — High |
| **Severity** | HIGH |
| **Status** | **[INTRODUCED / WORSENED]** |

The Lab-1 commit added a second snake by adding a parallel `Snake* snake2` field (line 19), alongside a parallel `int score2` (line 22), `string playerName2` (line 30), and `string loserName` (line 27). The single-player version encoded "1 snake" implicitly in one field. The multiplayer version now encodes "2 snakes" across **8 parallel fields** and every method in `GameBoard.cpp`. Adding a hypothetical 3rd player would require editing: `GameBoard.h` (4 new fields), and `GameBoard.cpp` at `constructor:18`, `destructor:22`, `spawnFood:41`, `drawBorder:—`, `displayGameInfo:88`, `draw:143`, `update:168-190`, `getInput:209-212`, `displayGameOver:239`. That is ≥9 edit sites for one logical decision.

---

### F2 — Long Method: `GameBoard::update()` grew longer

| Field | Value |
|-------|-------|
| **Smell** | Long Method (Bloaters) |
| **File:line** | `snake0/GameBoard.cpp:147` |
| **Confidence** | C3 — High |
| **Severity** | MEDIUM |
| **Status** | **[WORSENED]** — was 47 lines, now ~65 lines |

The Lab-1 commit added snake2 move, food-eat, collision detection, and game-over logic directly into `update()`. The method now handles both players' movement, food, collision, and game-end logic in one block. Lines 168–192 are new.

---

### F3 — Duplicate Code: Snake 1 and Snake 2 logic duplicated in `update()` and `getInput()`

| Field | Value |
|-------|-------|
| **Smell** | Duplicate Code (Dispensables) |
| **File:line** | `snake0/GameBoard.cpp:150-165` (snake1 block) and `snake0/GameBoard.cpp:168-175` (snake2 block) |
| **Confidence** | C3 — High |
| **Severity** | MEDIUM |
| **Status** | **[INTRODUCED]** |

The update method has two nearly identical blocks — one for each snake — differing only by which pointer is used. Similarly, `getInput()` at lines 202–212 (snake1 arrow keys) and lines 209–212 (snake2 WASD) is a duplicated dispatch pattern. Any change to how a snake eats, moves, or collides must be applied to both blocks identically.

---

### F4 — Feature Envy: Collision logic in `GameBoard::update()` reaches into `Snake`

| Field | Value |
|-------|-------|
| **Smell** | Feature Envy (Couplers) |
| **File:line** | `snake0/GameBoard.cpp:177-182` |
| **Confidence** | C2 — Medium |
| **Severity** | LOW |
| **Status** | Unchanged from main |

Same as F3 in `main.md`. Not changed.

---

### F5 — Primitive Obsession: Direction as raw `char`

| Field | Value |
|-------|-------|
| **Smell** | Primitive Obsession (Bloaters) |
| **File:line** | `snake0/Common.h:20-23` |
| **Confidence** | C2 — Medium |
| **Severity** | LOW |
| **Status** | Unchanged from main |

---

### F6 — Duplicate/Dead Code: `isHighScore()` vs `qualifiesAsHighScore()`

| Field | Value |
|-------|-------|
| **Smell** | Duplicate Code / Dead Code (Dispensables) |
| **File:line** | `snake0/HighScoreManager.h:33-34` |
| **Confidence** | C3 — High |
| **Severity** | LOW |
| **Status** | Unchanged from main. The `#include <ctime>` was added (`HighScoreManager.cpp:3`) but the duplicate methods remain. |

---

### F7 — Long Parameter List: `GameBoard` constructor

| Field | Value |
|-------|-------|
| **Smell** | Long Parameter List (Bloaters) |
| **File:line** | `snake0/GameBoard.h:33` |
| **Confidence** | C2 — Medium |
| **Severity** | LOW |
| **Status** | **[WORSENED]** — now 4 parameters: `hsm`, `difficulty`, `name`, `name2` |

The constructor at `GameBoard.h:33` now has a 4th parameter `const string& name2 = "Player 2"`. Four parameters is the boundary the smell catalog uses. The default argument hides the growth.

---

### F8 — Dead Code: Commented-out `system("cls")` calls

| Field | Value |
|-------|-------|
| **Smell** | Dead Code / Excessive Comments |
| **File:line** | `snake0/GameBoard.cpp:135`, `225`; `snake0/GameManager.cpp:7`, `27`, `58`, `110` |
| **Confidence** | C3 — High |
| **Severity** | LOW |
| **Status** | Unchanged from main |

---

## Summary Table

| # | Smell | File:line | Confidence | Severity | Status |
|---|-------|----------|------------|---------|--------|
| F1 | Shotgun Surgery (snake count) | `GameBoard.h:18-19` | C3 | HIGH | **WORSENED** |
| F2 | Long Method (update) | `GameBoard.cpp:147` | C3 | MEDIUM | **WORSENED** |
| F3 | Duplicate Code (snake1/snake2 blocks) | `GameBoard.cpp:150` | C3 | MEDIUM | **INTRODUCED** |
| F4 | Feature Envy (collision in update) | `GameBoard.cpp:177` | C2 | LOW | Unchanged |
| F5 | Primitive Obsession (direction char) | `Common.h:20` | C2 | LOW | Unchanged |
| F6 | Duplicate/Dead Code (isHighScore) | `HighScoreManager.h:33` | C3 | LOW | Unchanged |
| F7 | Long Parameter List (ctor) | `GameBoard.h:33` | C2 | LOW | **WORSENED** |
| F8 | Dead Code (commented-out cls) | `GameBoard.cpp:135` | C3 | LOW | Unchanged |

**Total findings: 8** (same count, but F1/F2/F7 worsened and F3 newly introduced)

---

## Delta vs. `main.md`

| Category | Smells |
|---------|--------|
| **Introduced** | F3 (Duplicate Code, snake1/snake2 blocks) |
| **Worsened** | F1 (Shotgun Surgery — now 8 parallel fields instead of 1), F2 (Long Method — grown by ~18 lines), F7 (Long Parameter List — now at 4 params) |
| **Left untouched** | F4, F5, F6, F8 |
| **Removed** | *(none)* |

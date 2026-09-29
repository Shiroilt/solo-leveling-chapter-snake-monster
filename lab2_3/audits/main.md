# Code Smell Audit — `main` branch (single-player, pre-Lab-1)

> **Commit**: `40f0085` (HEAD of main at time of audit)
> **Scope**: `snake0/` directory — the modular C++ console snake game.
> **Method**: Applied `detect-code-smells` catalog with `review-accuracy-calibration` confidence model.
> **Rule**: Report only. No edits made. Every finding cites file:line.

---

## Findings

### F1 — Shotgun Surgery: Snake count implicitly encoded as individual fields

| Field | Value |
|-------|-------|
| **Smell** | Shotgun Surgery (Change Preventers) |
| **File:line** | `snake0/GameBoard.h:18` (`Snake* snake`) |
| **Confidence** | C3 — High |
| **Severity** | MEDIUM |

`GameBoard` holds exactly one snake via `Snake* snake` (line 18). The number of snakes is implicitly one — not stated anywhere. If a second snake is needed (Lab 1), the change requires: adding `Snake* snake2` in `GameBoard.h`, adding `score2`, `playerName2`, `loserName` in `GameBoard.h`, and editing every method in `GameBoard.cpp` that touches the snake (constructor, destructor, `draw`, `update`, `spawnFood`, `getInput`, `displayGameInfo`, `displayGameOver`). That is 8+ edit sites for what is logically a single decision: "how many snakes are there."

---

### F2 — Long Method: `GameBoard::update()` is too long and does too much

| Field | Value |
|-------|-------|
| **Smell** | Long Method (Bloaters) |
| **File:line** | `snake0/GameBoard.cpp:147` |
| **Confidence** | C3 — High |
| **Severity** | MEDIUM |

`GameBoard::update()` spans lines 147–193 (47 lines). It handles: food-eat detection, score increment, level check, level-up display trigger, food respawn, self-collision check, boundary collision check — all in one method. A method that does 4 distinct things (move, eat, collide, level) cannot be easily tested or changed in isolation.

---

### F3 — Feature Envy: Collision logic split across `GameBoard` and `Snake`

| Field | Value |
|-------|-------|
| **Smell** | Feature Envy (Couplers) |
| **File:line** | `snake0/GameBoard.cpp:177-180` |
| **Confidence** | C2 — Medium |
| **Severity** | LOW |

`GameBoard::update()` calls `snake->checkSelfCollision()`, `snake->checkBoundaryCollision(consoleWidth, consoleHeight)` at line 177–179. The boundary collision check requires the board's dimensions as arguments, meaning `Snake` cannot check its own boundary without being told where it lives. This is not a strong smell given the design — `Snake` genuinely does not own the board size — but it creates an awkward dependency.

---

### F4 — Primitive Obsession: Direction represented as a raw `char`

| Field | Value |
|-------|-------|
| **Smell** | Primitive Obsession (Bloaters) |
| **File:line** | `snake0/Common.h:20-23` |
| **Confidence** | C2 — Medium |
| **Severity** | LOW |

Direction is encoded as `const char DIR_UP = 'U'`, `DIR_DOWN = 'D'`, `DIR_LEFT = 'L'`, `DIR_RIGHT = 'R'`. A raw `char` field gives no compile-time safety — any `char` can be stored in `direction`. An enum or enum class would constrain valid values and make `changeDirection()` self-documenting.

---

### F5 — Data Class: `Food` has no meaningful behavior beyond data

| Field | Value |
|-------|-------|
| **Smell** | Data Class (Dispensables) |
| **File:line** | `snake0/Food.h:7` |
| **Confidence** | C2 — Medium |
| **Severity** | LOW |

`Food` holds `position` and `active`, with `spawn()`, `deactivate()`, `draw()`, and `isActive()`. The `spawn()` method has real logic (finds unoccupied cells), so this is not a pure data class — but the `active` flag is redundant. Food's "not yet spawned" state is always managed externally; the flag is set to `false` in `Food()` then never read except in `draw()` and `move()`. The class is borderline.

---

### F6 — Dead Code: `isHighScore()` and `qualifiesAsHighScore()` are duplicates

| Field | Value |
|-------|-------|
| **Smell** | Duplicate Code / Dead Code (Dispensables) |
| **File:line** | `snake0/HighScoreManager.h:33-34` / `snake0/HighScoreManager.cpp:66-74` |
| **Confidence** | C3 — High |
| **Severity** | LOW |

`HighScoreManager` declares both `isHighScore(int)` (line 33) and `qualifiesAsHighScore(int)` (line 34). Their implementations at `HighScoreManager.cpp:66` and `HighScoreManager.cpp:71` are nearly identical — the only difference is `>` vs `>=`. Neither is called anywhere in `GameBoard.cpp` or `GameManager.cpp` (only `getHighestScore()` is called directly). One of these is dead code; the other is unused by the active codebase path.

---

### F7 — Long Parameter List: `GameBoard` constructor has a growing parameter list

| Field | Value |
|-------|-------|
| **Smell** | Long Parameter List (Bloaters) |
| **File:line** | `snake0/GameBoard.h:33` |
| **Confidence** | C1 — Low |
| **Severity** | NIT |

`GameBoard(HighScoreManager* hsm, int difficulty, const string& name)` has 3 parameters, which is within normal range. Flagged speculatively because adding a second player name will push this to 4 — still acceptable.

---

### F8 — Comments: Commented-out `system("cls")` calls throughout `GameBoard.cpp`

| Field | Value |
|-------|-------|
| **Smell** | Excessive Comments / Dead Code |
| **File:line** | `snake0/GameBoard.cpp:135`, `snake0/GameBoard.cpp:225`, `snake0/GameManager.cpp:7`, `snake0/GameManager.cpp:27`, `snake0/GameManager.cpp:58`, `snake0/GameManager.cpp:110` |
| **Confidence** | C3 — High |
| **Severity** | LOW |

Six commented-out `system("cls")` calls were replaced with `SetConsoleCursorPosition` but never removed. They are noise in the code; anyone reading them must verify whether the substitution was intentional.

---

## Summary Table

| # | Smell | File:line | Confidence | Severity |
|---|-------|----------|------------|---------|
| F1 | Shotgun Surgery (snake count) | `GameBoard.h:18` | C3 | MEDIUM |
| F2 | Long Method (update) | `GameBoard.cpp:147` | C3 | MEDIUM |
| F3 | Feature Envy (collision in update) | `GameBoard.cpp:177` | C2 | LOW |
| F4 | Primitive Obsession (direction char) | `Common.h:20` | C2 | LOW |
| F5 | Data Class (Food) | `Food.h:7` | C2 | LOW |
| F6 | Duplicate/Dead Code (isHighScore vs qualifiesAsHighScore) | `HighScoreManager.h:33` | C3 | LOW |
| F7 | Long Parameter List (GameBoard ctor) | `GameBoard.h:33` | C1 | NIT |
| F8 | Dead Code (commented-out system cls) | `GameBoard.cpp:135` | C3 | LOW |

**Total findings: 8**

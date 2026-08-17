# Ubiquitous Language — Snake Game (C++)

_Generated 2026-08-17. **Human-reviewed**: yes — reviewed and corrected by Yash Sai._

> **Warning**: an unreviewed, agent-authored glossary is worse than none: it becomes
> confident-sounding lore that later sessions treat as truth. This file has been
> reviewed; see `lab2_3/REPORT.md § 2` for what was changed and why.

---

## Canonical Terms

| Term | Definition | Canonical in code | Aliases to avoid |
|------|-----------|-------------------|-----------------|
| **Snake** | A player-controlled entity that moves across the board one cell per tick, grows when it eats food, and dies on collision. Each game always has exactly two snakes in the current multiplayer version. | `Snake` (`snake0/Snake.h:8`) | "worm", "player entity" |
| **Food** | A single consumable item placed at a random unoccupied cell on the board. Eating it increases the snake's length by one and the owning player's score by one. Only one food item exists on the board at any time. | `Food` (`snake0/Food.h:7`) | "fruit", "apple", "item" — README uses "food (●)" |
| **Board** | The bounded rectangular play area. Its dimensions are set at startup from the console window size. The border cells are walls; the interior is the playable space. | `GameBoard` (class) — domain word is **Board** (`snake0/GameBoard.h:14`) | "arena", "grid", "screen" |
| **Player** | A human participant controlling one snake. Player 1 uses arrow keys; Player 2 uses WASD. The concept "player" appears in the UI labels (`P1 Score`, `P2 Score`) and in `loser` tracking. | `loser` int (value 1 or 2) in `GameBoard`; labels in `displayGameInfo()` (`snake0/GameBoard.cpp:70,74`) | "user", "character" |
| **Score** | The count of food items a player's snake has eaten in the current session. Displayed live and persisted to the high-score file at game-over. | `score1`, `score2` (`snake0/GameBoard.h:19,20`) | "points", "count" |
| **Level** | A progression milestone reached every 10 points (using the higher of the two scores). Advancing a level increases game speed. | `level` (`snake0/GameBoard.h:21`) | "stage", "round", "difficulty step" |
| **Tick** | One iteration of the game loop — snakes move one cell, input is read, collisions are checked, and the board is redrawn. Speed (ms between ticks) decreases as level rises. | Implicit in `while (board->update())` + `Sleep(board->getSpeed())` in `GameManager.cpp:182,185` | "frame", "step", "turn" |
| **Collision** | An event where a snake's head occupies the same cell as a wall, its own body, or the other snake's body. Any collision immediately ends the game and marks the snake's player as the loser. | `checkBoundaryCollision`, `checkSelfCollision`, `checkOtherCollision` (`snake0/Snake.h:23,24`) | "hit", "crash", "death" |
| **Direction** | The current heading of a snake: Up, Down, Left, or Right. A snake may not reverse direction (e.g., Left is forbidden when heading Right). | `DIR_UP/DOWN/LEFT/RIGHT` constants (`snake0/Common.h:8–11`); `direction` field (`snake0/Snake.h:11`) | "heading", "facing", "movement" |
| **Difficulty** | The starting speed of the game: Easy (180 ms/tick), Normal (120 ms/tick), Hard (70 ms/tick). Chosen once at game start; does not change mid-game except via level progression. | `baseSpeed` / `currentSpeed` (`snake0/GameBoard.h:22,23`); `modeChoice` in `GameManager.cpp:155` | "mode", "speed setting" |
| **High Score** | The historically best score achieved by any named player, persisted across sessions in `highscores.txt`. The top 10 records are displayed in the high-score table. | `HighScoreManager` (class); `highscores.txt` | "record", "best score", "leaderboard entry" |

---

## Flagged Ambiguities

| Ambiguity | Evidence |
|-----------|----------|
| **"Food" vs "Fruit"** | The README says "eat food (●) to grow"; the class is `Food`; but the README intro also says "Classic Snake Mechanics: Guide the snake to eat food and grow." No instance of "fruit" appears in code. The README is consistent; **Food** is the correct canonical term. No conflict — included here because the assignment expects this term noted. |
| **"Score" is singular per player, but `getScore()` returns `max(score1, score2)`** | `GameBoard::getScore()` (`snake0/GameBoard.cpp:30`) returns the higher of the two player scores, used for level progression and high-score saving. The display method shows `score1` and `score2` separately. This means "score" is overloaded: per-player score (for display) vs top-score (for progression/persistence). A future refactor should separate these concepts explicitly. |
| **"Player" has no object — it lives only as an int flag (`loser`)** | The domain concept of a Player (human with a name, score, snake, controls) is scattered: name is in `GameManager`, scores are `score1`/`score2` in `GameBoard`, input mapping is hardcoded in `getInput()`, and identity is tracked only as `loser = 1 or 2`. There is no `Player` class. This is both a terminology drift and a design smell (see `lab2_3/audits/`). |
| **"Level" used for both game progression and difficulty display** | In `showHighScores()` (`GameManager.cpp:96`), `level` is recomputed from a stored score as `(score/10)+1`. This is a local recalculation, not the live `GameBoard::level`. Two different sources of truth for "level" exist. |

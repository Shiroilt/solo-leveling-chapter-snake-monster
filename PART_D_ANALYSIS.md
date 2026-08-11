# Part D — Measurement and Analysis

## Measurement table

| Field | Value |
|---|---|
| Functions changed (headline number) | **10** (9 modified + 1 new) |
| Names of those functions | `GameBoard::GameBoard()`, `GameBoard::~GameBoard()`, `GameBoard::getScore()`, `GameBoard::spawnFood()`, `GameBoard::displayGameInfo()`, `GameBoard::draw()`, `GameBoard::update()`, `GameBoard::getInput()`, `GameBoard::displayGameOver()`, `Snake::checkCollisionWithSnake()` (new) |
| Files changed | 4 — `Snake.h`, `Snake.cpp`, `GameBoard.h`, `GameBoard.cpp` |
| Lines added / deleted | 57 / 10 |
| % of codebase touched | 67 lines changed ÷ 954 total lines across the files the build actually compiles (`main.cpp`, `Common.*`, `HighScoreManager.*`, `Food.*`, `Snake.*`, `GameBoard.*`, `GameManager.*`) ≈ **7.0%**. This excludes the orphaned `game.cpp` in the same folder, which is not part of the `g++` build command. |
| Hours spent | *[fill in — group's actual hours]* |
| LLM model + tool | Claude Sonnet 5, via Claude.ai web chat with code-execution tools |
| LLM prompts to working code | 2 (see LLM-LOG.md) |
| What code was given to the LLM | The whole repo, self-fetched by the assistant via `git clone` — not pasted in by hand |
| Assignment doc pasted into the LLM? | Yes, partway through — see LLM-LOG.md |

*Note on the header files: `GameBoard.h` and `Snake.h` also changed (new member variables, one new method declaration), but those are declarations at class/global scope, not function bodies — counted as **(global)**, not part of the 10 functions above, per the assignment's own guidance on lines changed outside any function.*

## Analysis

**Q1 — Which functions did you have to change that are not about snakes at all? Why did a second snake reach them?**

Half of the functions touched have nothing to do with a snake's own movement: `getScore()` (which needed a `getScore2()` counterpart), `displayGameInfo()` (rendering/UI), `displayGameOver()` (end-of-game UI and messaging), `spawnFood()` (food placement), and `getInput()` (keyboard handling). A second snake reached all of them because each one silently assumed there was exactly *one* of everything — one score to read, one score to print, one set of coordinates food had to avoid, one key-to-direction mapping. None of these functions are "snake" concerns individually, but they are all downstream of the assumption "there is one snake." Adding a second snake therefore forced a second score variable, a second HUD line, a second exclusion zone for food spawning, and a second branch in the key-input switch.

**Q2 — Was "there is exactly one snake" written down in one place, or spread across the code?**

Spread. There was no single `NUM_SNAKES = 1` constant to flip. The assumption was implicit in five separate places: the `Snake* snake` member declaration, the food-spawn exclusion list, the HUD print statements, the input switch statement, and the collision/game-over check in `update()`. Each of these had to be found and edited independently — nothing failed to compile in a way that would have pointed us toward the others, so the search was manual.

**Q3 — Prediction vs. reality.**

Our Lecture 3 prediction (made by reading only, with no compiler) named `getInput()`, `update()`, `draw()`, `displayGameInfo()`, and the constructor/destructor as needing to change — and all of those did. What we missed by reading alone: `getScore()` needing a sibling `getScore2()` accessor, and the exact shape of the fix in `spawnFood()`. We had predicted `spawnFood()` "must change" but hadn't anticipated that food could spawn *inside* the second snake specifically — a detail that only becomes obvious once you trace how `snake->getBody()` is used as the sole exclusion list. We also underestimated `displayGameOver()`: from reading, it looked like a pure end-of-game print function, but implementing the change revealed it needed a `loserName` decided during `update()`, coupling two functions we hadn't predicted were linked.

**Q4 — What one design change would have made this a twenty-minute job?**

If `GameBoard` held a `vector<Snake*> snakes` (and a parallel `vector<int> scores`) instead of a single named `Snake* snake`, nearly every touched function would have become a loop over that vector instead of a hand-duplicated second code path. `getInput()` would map a key to an index into that vector instead of a hardcoded pointer; `spawnFood()` would fold over all snakes' bodies; `displayGameInfo()` would iterate and print one score line per snake. The two-player case wouldn't need to be a special mode at all — it would simply be "the vector has two elements."

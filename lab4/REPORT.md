# Lab 4: The Tests You Cannot Write

## 1. Five rules

1. The snake moves exactly one cell in its current direction on each movement update.
2. The snake ignores a direction request that would reverse it by 180 degrees.
3. Pressing a movement key (W/A/S/D or an arrow key) changes the snake's direction.
4. Food never spawns on a cell occupied by the snake.
5. When the snake's head reaches active food, the snake grows by exactly one segment and the food becomes inactive.

## 2. Part B — Try to test all five

| Rule | Test written without source change? | Blocking dependency (file:line in `lab4-base`) |
|---|---|---|
| 1. Snake moves one cell | Yes | — |
| 2. Opposite direction is ignored | Yes | — |
| 3. Keyboard input changes direction | No | `snake0/GameBoard.cpp:166-168` — `_kbhit()` and `_getch()` are called directly, with no input provider that a test can supply. |
| 4. Food avoids the snake | Yes | — |
| 5. Eating food grows the snake and deactivates food | No | `snake0/Food.cpp:33` chooses the food position with `rand()`, while `snake0/Snake.cpp:45` only enters the eating path when the new head exactly equals that hidden position. `snake0/Food.h:9-18` exposes no way to inject/set the position. |

**How many of five could be tested without changing the source? 4 of 5.**

The two blocked rules are blocked for different reasons: keyboard input is directly coupled to the console, and food consumption cannot be arranged deterministically because the food position is private and selected through `rand()`.

## 3. Part C — Coverage

Coverage was measured on the pre-seam Part B suite using `gcov -b`.

| Source file | Line coverage | Branch coverage |
|---|---:|---:|
| `snake0/Snake.cpp` | 47.54% | 68.42% |
| `snake0/Food.cpp` | 78.57% | 87.50% |

### Rule that runs but is not fully checked

**Rule 1 — the snake moves exactly one cell in its current direction.**

The suite executes the movement branches in `snake0/Snake.cpp:36-40` for multiple directions, but the assertions only verify the exact resulting position for the initial RIGHT movement. The UP, LEFT, and DOWN executions are used for coverage but their resulting coordinates are not asserted.

The missing assertions should verify the expected head coordinate after each of those three moves.

## 4. Part D — One seam, one PR

### Rule made testable

Rule 5: when the snake reaches active food, it grows by exactly one segment and the food becomes inactive.

### Commit 1 — seam

Commit: `4e2fc8b` — `Lab 4: add food object seam`

The seam introduces `FoodPort`, an object interface containing the food operations needed by `Snake::move()`. `Food` implements the interface, and `Snake::move()` now receives a `FoodPort&` instead of depending directly on the concrete `Food` class.

### Seam details

| Field | Answer |
|---|---|
| Seam kind | **Object seam** |
| Enabling point | `Snake::move(FoodPort& food)` — the collaborator is supplied by the caller. |
| What production code gave up | `Snake` is no longer coupled directly to the concrete `Food` class; it now depends on the smaller `FoodPort` abstraction. Existing production behavior is unchanged because `Food` implements `FoodPort`. |

The seam commit contains no new test behavior and no gameplay-rule change.

### Commit 2 — tests

Commit 2 contains the Lab 4 tests and report. It does **not** modify production source files.

The new test supplies a deterministic `FakeFood` through the object seam. The fake places food directly in front of the snake and records the `deactivate()` call, allowing the eating rule to be asserted without randomness.

## 5. Part E — Name the double

The test uses a **fake** called `FakeFood`.

It is a fake because it provides a controllable implementation of the `FoodPort` collaborator for the test and records whether `deactivate()` was called. `Snake` asks the collaborator questions (`isActive()` and `getPosition()`) and also tells it to perform an action (`deactivate()`); the test checks both the returned state and the requested action.

## 6. Part F — Two smells in the tests

### Smell 1 — Eager test

- **Location:** `lab4/tests.cpp:7-23` (`rule1_snake_moves_one_cell`).
- **Smell:** Eager test — one test exercises RIGHT, UP, LEFT, and DOWN movement behaviors.
- **One-line fix:** Split the four movement directions into focused test functions so a failure identifies the broken direction immediately.

### Smell 2 — Assertion roulette

- **Location:** `lab4/tests.cpp:41-43` (`rule3_food_spawn_avoids_snake`).
- **Smell:** Assertion roulette — the loop contains several bare assertions with no failure message.
- **One-line fix:** Assert the whole collection/property with a named helper or add a message identifying the offending snake segment.

## Submission notes

- `lab4-base` is pinned to the `main` commit used for this lab.
- The PR should be `lab4 -> main`.
- There are exactly two commits for the lab work.
- Commit 1 contains only the seam.
- Commit 2 contains the tests/report and no production source changes.
- Every Part B blocker refers to a line present in `lab4-base`.

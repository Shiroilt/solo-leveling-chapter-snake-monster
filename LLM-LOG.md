# LLM-LOG.md — Lab Assignment 3 (Make it Multiplayer)

## Setup

| Field | Value |
|---|---|
| Model(s) | Claude Sonnet 5 |
| Tool / harness | Claude.ai web chat, with code-execution/file tools enabled. The assistant could `git clone` the repository into its own sandbox and read/edit/compile files directly. This is not a pure "paste code into chat" session, nor a repo-connected IDE agent like Cursor. |
| IDE / editor | None on the assistant's side (sandbox shell only). On my side: Windows CMD and File Explorer to move the finished files into my local clone, and g++/MinGW to compile. |
| Did you paste this assignment document into it? | **Yes — partway through, not at the start.** I first asked for a multiplayer implementation without giving the assignment doc, got a working but out-of-spec version, then pasted the full assignment doc afterward. This caused a rewrite scoped to the actual requirements. |

## Session (multiplayer feature only)

**Prompt 1:**
> "Add a two-player mode to the Snake game in the repo you already have cloned. Both players should be able to move and play at the same time — one snake controlled with the arrow keys, the other with WASD."

Result: A fully working two-player implementation — a second `Snake*`, WASD controls for player 2, shared food, separate scores, and snake-vs-snake collision. Since no constraints had been given yet, it also added things that weren't asked for: a "1 Player / 2 Player" mode-select menu, distinct draw colors per snake, and game logic that kept playing until *both* snakes had died instead of ending on the first crash.

**Prompt 2:**
> "Here is the full assignment specification — please re-read it and bring the implementation in line with it exactly, keeping the change scoped to only what the spec requires."
> [Full text of the Lab Assignment 3 document pasted into chat]

Result: The assistant identified that the Prompt 1 version violated explicit constraints in the assignment doc — no new menus, no color changes, and "the game ends when either snake hits a wall, itself, or the other snake" (not both). It discarded the first version, re-cloned the repository fresh, and rebuilt the change scoped to exactly four files: `Snake.h`, `Snake.cpp`, `GameBoard.h`, `GameBoard.cpp`. The menu and colors were removed, and `update()` was corrected to end the game on the first collision rather than waiting for both snakes to die. It verified the result compiled cleanly using a cross-compiled MinGW toolchain and confirmed the diff was limited to those four files via `git diff --stat`.

**This second version is the one submitted in the PR.**

## Totals

| Field | Value |
|---|---|
| Total prompts to working, spec-compliant code | **2** |
| What code I gave it | The whole repo — the assistant cloned the GitHub repository itself (`git clone`) and read every file in `snake0/` directly using its own file tools. I did not paste file contents into chat. It also had to identify on its own that `snake0/` (the multi-file version) was the relevant target, as opposed to an unused single-file `game.cpp` sitting in the same folder that isn't part of the build command. |
| Known pre-existing bug flagged, not fixed | `HighScoreManager.cpp` is missing `#include <ctime>`, which fails to compile on a strict MinGW toolchain (`time`, `localtime`, `strftime` undeclared). This is unrelated to the multiplayer change and was **not** fixed in the diff, per the assignment's "do not fix unrelated build errors" instruction. The multiplayer logic was verified by temporarily patching this include in a throwaway build to confirm compilation, then discarding that patch — the submitted `HighScoreManager.cpp` is byte-identical to the original. |
 

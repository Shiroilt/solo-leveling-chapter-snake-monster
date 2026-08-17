#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Common.h"
#include "Snake.h"
#include "Food.h"
#include "HighScoreManager.h"
#include <conio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

// Number of players — the single source of truth for snake count.
// Changing this constant is the only edit needed to change player count.
static const int NUM_PLAYERS = 2;

// Per-player configuration: controls, colors, and spawn position fraction.
// All player-specific settings live here; no player data is hardcoded elsewhere.
struct PlayerConfig {
    int  headColor;
    int  bodyColor;
    char initialDirection;
    // Spawn X position as a fraction of console width (numerator out of 3)
    int  spawnXFraction;
};

static const PlayerConfig PLAYER_CONFIG[NUM_PLAYERS] = {
    { 10, 2, DIR_RIGHT, 1 },  // Player 1: green head, arrow keys, left-third spawn
    { 11, 3, DIR_LEFT,  2 },  // Player 2: cyan head, WASD keys, right-third spawn
};

class GameBoard {
private:
    Snake* snakes[NUM_PLAYERS];
    Food food;
    int scores[NUM_PLAYERS];
    int level;
    int baseSpeed;
    int currentSpeed;
    bool gameOver;
    int loser;
    HighScoreManager* highScoreManager;
    string playerName;

public:
    GameBoard(HighScoreManager* hsm, int difficulty, const string& name);
    ~GameBoard();
    int getScore();
    int getLevel();
    int getSpeed();
    bool isGameOver();
    void updateLevel();
    void spawnFood();
    void drawBorder();
    void displayGameInfo();
    void displayInstructions();
    void showLevelUpMessage();
    void draw();
    bool update();
    void getInput();
    void displayGameOver();
};

#endif
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
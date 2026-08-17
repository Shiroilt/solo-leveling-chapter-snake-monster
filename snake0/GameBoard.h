#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "Common.h"
#include "Snake.h"
#include "Food.h"
#include "HighScoreManager.h"
#ifdef _WIN32
#include <conio.h>
#endif
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

class GameBoard {
private:
    // Snake count is defined in exactly one place: snakes.size().
    // Adding or removing a player requires changing only the constructor.
    std::vector<Snake*> snakes;
    std::vector<int> scores;
    std::vector<std::string> playerNames;
    Food food;
    int level;
    int baseSpeed;
    int currentSpeed;
    bool gameOver;
    string loserName;
    HighScoreManager* highScoreManager;

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
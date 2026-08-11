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

class GameBoard {
private:
    Snake* snake1;
    Snake* snake2;
    Food food;
    int score1;
    int score2;
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
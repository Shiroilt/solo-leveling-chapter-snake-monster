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

class GameBoard {
private:
    Snake* snake;
    Snake* snake2;
    Food food;
    int score;
    int score2;
    int level;
    int baseSpeed;
    int currentSpeed;
    bool gameOver;
    string loserName;
    HighScoreManager* highScoreManager;
    string playerName;
    string playerName2;

public:
    GameBoard(HighScoreManager* hsm, int difficulty, const string& name, const string& name2 = "Player 2");
    ~GameBoard();
    int getScore();
    int getScore2();
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
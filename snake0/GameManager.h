#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "Common.h"
#include "GameBoard.h"
#include "HighScoreManager.h"
#include <conio.h>
#include <iostream>
#include <iomanip>

class GameManager {
private:
    string playerName;
    HighScoreManager highScoreManager;

public:
    GameManager();
    void showMainMenu();
    void showLevelProgressionInfo();
    void showHighScores();
    void showInstructions();
    void startGame();
    void run();
};

#endif
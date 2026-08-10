#include "GameBoard.h"
#include <iostream>
using namespace std;
GameBoard::GameBoard(HighScoreManager* hsm, int difficulty, const string& name) : 
    score1(0), score2(0), level(1), gameOver(false), loser(0), highScoreManager(hsm), playerName(name) {
    srand(time(0));
    initScreen();
    
    switch(difficulty) {
        case 1: baseSpeed = 180; break;
        case 2: baseSpeed = 120; break;
        case 3: baseSpeed = 70;  break;
        default: baseSpeed = 120; break;
    }
    currentSpeed = baseSpeed;
    
    snake1 = new Snake(consoleWidth / 3, consoleHeight / 2, 3, DIR_RIGHT);
    snake2 = new Snake(2 * consoleWidth / 3, consoleHeight / 2, 3, DIR_LEFT);
}

GameBoard::~GameBoard() { 
    delete snake1; 
    delete snake2; 
}

int GameBoard::getScore() { return max(score1, score2); }
int GameBoard::getLevel() { return level; }
int GameBoard::getSpeed() { return currentSpeed; }
bool GameBoard::isGameOver() { return gameOver; }

void GameBoard::updateLevel() {
    int topScore = max(score1, score2);
    int newLevel = (topScore / 10) + 1;
    if (newLevel > level) {
        level = newLevel;
        currentSpeed = max(30, baseSpeed - (level - 1) * (baseSpeed / 10));
    }
}

void GameBoard::spawnFood() {
    food.spawn(snake1->getBody(), snake2->getBody(), consoleWidth, consoleHeight);
}

void GameBoard::drawBorder() {
    setColor(14);
    for (int x = 0; x < consoleWidth; x++) {
        gotoxy(x, 0); cout << "═";
        gotoxy(x, consoleHeight - 1); cout << "═";
    }
    for (int y = 0; y < consoleHeight; y++) {
        gotoxy(0, y); cout << "║";
        gotoxy(consoleWidth - 1, y); cout << "║";
    }
    
    gotoxy(0, 0); cout << "╔";
    gotoxy(consoleWidth - 1, 0); cout << "╗";
    gotoxy(0, consoleHeight - 1); cout << "╚";
    gotoxy(consoleWidth - 1, consoleHeight - 1); cout << "╝";
    
    setColor(7);
}

void GameBoard::displayGameInfo() {
    gotoxy(2, 0);
    setColor(10);
    cout << "P1 Score: " << score1;
    
    gotoxy(20, 0);
    setColor(11);
    cout << "P2 Score: " << score2;
    
    gotoxy(consoleWidth / 2, 0);
    setColor(13);
    cout << "Lvl: " << level;
    
    gotoxy(consoleWidth - 20, 0);
    setColor(10);
    cout << "Speed: " << currentSpeed << "ms";
    
    gotoxy(consoleWidth - 35, 0);
    setColor(6);
    cout << "Best: " << highScoreManager->getHighestScore();
}

void GameBoard::displayInstructions() {
    gotoxy(2, consoleHeight - 1);
    setColor(8);
    cout << "P1: Arrow Keys | P2: WASD | P: Pause";
}

void GameBoard::showLevelUpMessage() {
    int centerX = consoleWidth / 2 - 10;
    int centerY = consoleHeight / 2;
    MessageBeep(MB_OK);
    gotoxy(centerX, centerY - 1);
    setColor(14);
    cout << "╔══════════════════════╗";
    
    gotoxy(centerX, centerY);
    setColor(14);
    cout << "║      LEVEL UP!       ║";
    
    gotoxy(centerX, centerY + 1);
    setColor(14);
    cout << "║   Now at Level " << setw(2) << level << "   ║";
    
    gotoxy(centerX, centerY + 2);
    setColor(14);
    cout << "╚══════════════════════╝";
    
    for (int i = 0; i < 3; i++) {
        Sleep(300);
        gotoxy(centerX, centerY);
        setColor(12);
        cout << "║      LEVEL UP!       ║";
        Sleep(300);
        gotoxy(centerX, centerY);
        setColor(14);
        cout << "║      LEVEL UP!       ║";
    }
    
    for (int y = centerY - 1; y <= centerY + 2; y++) {
        gotoxy(centerX, y);
        cout << "                      ";
    }
}

void GameBoard::draw() {
    system("cls");
    drawBorder();
    displayGameInfo();
    displayInstructions();
    
    snake1->draw(10, 2);
    snake2->draw(11, 3);
    food.draw();
}

bool GameBoard::update() {
    if (gameOver) return false;
    
    snake1->clearTail();
    snake2->clearTail();
    
    bool foodEaten1 = snake1->move(food);
    bool foodEaten2 = snake2->move(food);
    
    if (foodEaten1) {
        score1++;
        MessageBeep(MB_ICONASTERISK);
        int oldLevel = level;
        updateLevel();
        
        if (level > oldLevel) {
            draw();
            showLevelUpMessage();
            draw();
        }
        
        spawnFood();
    }
    
    if (foodEaten2) {
        score2++;
        MessageBeep(MB_ICONASTERISK);
        int oldLevel = level;
        updateLevel();
        
        if (level > oldLevel) {
            draw();
            showLevelUpMessage();
            draw();
        }
        
        spawnFood();
    }
    
    bool p1Wall = snake1->checkBoundaryCollision(consoleWidth, consoleHeight);
    bool p1Self = snake1->checkSelfCollision();
    bool p1Other = snake1->checkOtherCollision(snake2->getBody());
    bool p1Lost = p1Wall || p1Self || p1Other;
    
    bool p2Wall = snake2->checkBoundaryCollision(consoleWidth, consoleHeight);
    bool p2Self = snake2->checkSelfCollision();
    bool p2Other = snake2->checkOtherCollision(snake1->getBody());
    bool p2Lost = p2Wall || p2Self || p2Other;
    
    if (p1Lost && p2Lost) {
        loser = 3;
        gameOver = true;
        return false;
    } else if (p1Lost) {
        loser = 1;
        gameOver = true;
        return false;
    } else if (p2Lost) {
        loser = 2;
        gameOver = true;
        return false;
    }
    
    return true;
}

void GameBoard::getInput() {
    if (_kbhit()) {
        int key = _getch();

        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
                case 72: snake1->changeDirection(DIR_UP); break;
                case 80: snake1->changeDirection(DIR_DOWN); break;
                case 75: snake1->changeDirection(DIR_LEFT); break;
                case 77: snake1->changeDirection(DIR_RIGHT); break;
            }
        } else {
            switch (tolower(key)) {
                case 'w': snake2->changeDirection(DIR_UP); break;
                case 'a': snake2->changeDirection(DIR_LEFT); break;
                case 's': snake2->changeDirection(DIR_DOWN); break;
                case 'd': snake2->changeDirection(DIR_RIGHT); break;
                case 'p':
                    gotoxy(consoleWidth / 2 - 5, consoleHeight / 2);
                    setColor(14);
                    cout << "PAUSED";
                    while (_getch() != 'p') {}
                    break;
            }
        }
    }
}

void GameBoard::displayGameOver() {
    system("cls");
    MessageBeep(MB_ICONHAND);
    setColor(12);
    cout << "\n\n";
    cout << "   ╔══════════════════════════════╗\n";
    cout << "   ║         GAME OVER!           ║\n";
    cout << "   ╚══════════════════════════════╝\n\n";
    
    setColor(14);
    if (loser == 1) {
        cout << "   ❌ Player 1 Lost! (Player 2 Wins!)\n\n";
    } else if (loser == 2) {
        cout << "   ❌ Player 2 Lost! (Player 1 Wins!)\n\n";
    } else {
        cout << "   ❌ Both Players Collided! (Tie Game)\n\n";
    }
    
    setColor(7);
    cout << "   Player 1 Score: " << score1 << endl;
    cout << "   Player 2 Score: " << score2 << endl;
    cout << "   Level Reached: " << level << endl;
    cout << "   High Score: " << highScoreManager->getHighestScore() << endl;
    
    int topScore = max(score1, score2);
    int highest = highScoreManager->getHighestScore();
    if (topScore > highest && topScore > 0) {
        highScoreManager->addHighScore(playerName, topScore);
        setColor(14);
        cout << "\n   🎉 NEW HIGH SCORE! 🎉\n";
        cout << "   Congratulations!\n";
    }
    else if (topScore == highest && topScore > 0) {
        setColor(11);
        cout << "\n   ⭐ You matched the high score! ⭐\n";
    }

    cout << "\n";
    setColor(11);
    if (level >= 10) {
        cout << "   🏆 LEGENDARY! You're Snake Masters!\n";
    } else if (level >= 7) {
        cout << "   ⭐ EXCELLENT! Great skills!\n";
    } else if (level >= 4) {
        cout << "   👍 GOOD JOB! Keep practicing!\n";
    } else {
        cout << "   💪 Nice try! You'll do better next time!\n";
    }
    
    setColor(10);
    cout << "\n   Press any key to continue...";
    _getch();
}
#include "GameBoard.h"
#include <iostream>
using namespace std;

GameBoard::GameBoard(HighScoreManager* hsm, int difficulty, const string& name) :
    level(1), gameOver(false), highScoreManager(hsm) {
    srand(time(0));
    initScreen();

    switch(difficulty) {
        case 1: baseSpeed = 180; break;
        case 2: baseSpeed = 120; break;
        case 3: baseSpeed = 70;  break;
        default: baseSpeed = 120; break;
    }
    currentSpeed = baseSpeed;

    // Snake count is defined here: one entry in the vector = one snake.
    snakes.push_back(new Snake(consoleWidth / 2, consoleHeight / 2, 3));
    scores.push_back(0);
    playerNames.push_back(name);
}

GameBoard::~GameBoard() {
    for (Snake* s : snakes) {
        delete s;
    }
}

int GameBoard::getScore() { return scores[0]; }
int GameBoard::getLevel() { return level; }
int GameBoard::getSpeed() { return currentSpeed; }
bool GameBoard::isGameOver() { return gameOver; }

void GameBoard::updateLevel() {
    int newLevel = (scores[0] / 10) + 1;
    if (newLevel > level) {
        level = newLevel;
        currentSpeed = max(30, baseSpeed - (level - 1) * (baseSpeed / 10));
    }
}

void GameBoard::spawnFood() {
    vector<Point> occupied;
    for (Snake* s : snakes) {
        const vector<Point>& b = s->getBody();
        occupied.insert(occupied.end(), b.begin(), b.end());
    }
    food.spawn(occupied, consoleWidth, consoleHeight);
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
    setColor(11);
    cout << "Player: " << playerNames[0];

    gotoxy(consoleWidth / 2 - 8, 0);
    setColor(14);
    cout << "Score: " << scores[0];

    gotoxy(consoleWidth / 2 + 10, 0);
    setColor(13);
    cout << "Level: " << level;

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
    cout << "Controls: Arrow Keys | P: Pause";
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
    //system("cls");
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    drawBorder();
    displayGameInfo();
    displayInstructions();

    for (Snake* s : snakes) {
        s->draw();
    }
    food.draw();
}

bool GameBoard::update() {
    if (gameOver) return false;

    for (int i = 0; i < (int)snakes.size(); i++) {
        snakes[i]->clearTail();
        bool foodEaten = snakes[i]->move(food);

        if (foodEaten) {
            scores[i]++;
            MessageBeep(MB_ICONASTERISK);
            if (i == 0) {
                int oldLevel = level;
                updateLevel();
                if (level > oldLevel) {
                    draw();
                    showLevelUpMessage();
                    draw();
                }
            }
            spawnFood();
        }
    }

    for (int i = 0; i < (int)snakes.size(); i++) {
        bool crashed = snakes[i]->checkSelfCollision() ||
                       snakes[i]->checkBoundaryCollision(consoleWidth, consoleHeight);
        if (crashed) {
            gameOver = true;
            loserName = playerNames[i];
            return false;
        }
    }

    return true;
}

void GameBoard::getInput() {
    if (_kbhit()) {
        int key = _getch();

        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
                case 72: snakes[0]->changeDirection(DIR_UP); break;
                case 80: snakes[0]->changeDirection(DIR_DOWN); break;
                case 75: snakes[0]->changeDirection(DIR_LEFT); break;
                case 77: snakes[0]->changeDirection(DIR_RIGHT); break;
            }
        } else {
            switch (tolower(key)) {
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
    //system("cls");
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    MessageBeep(MB_ICONHAND);
    setColor(12);
    cout << "\n\n";
    cout << "   ╔══════════════════════════════╗\n";
    cout << "   ║         GAME OVER!           ║\n";
    cout << "   ╚══════════════════════════════╝\n\n";

    setColor(7);
    cout << "   " << loserName << " lost!\n\n";
    cout << "   Player: " << playerNames[0] << endl;
    cout << "   Final Score: " << scores[0] << endl;
    cout << "   Level Reached: " << level << endl;
    cout << "   High Score: " << highScoreManager->getHighestScore() << endl;

    int highest = highScoreManager->getHighestScore();
    if (scores[0] > highest && scores[0] > 0) {
        highScoreManager->addHighScore(playerNames[0], scores[0]);
        setColor(14);
        cout << "\n   🎉 NEW HIGH SCORE! 🎉\n";
        cout << "   Congratulations, " << playerNames[0] << "!\n";
    }
    else if (scores[0] == highest && scores[0] > 0) {
        setColor(11);
        cout << "\n   ⭐ You matched the high score! ⭐\n";
    }

    cout << "\n";
    setColor(11);
    if (level >= 10) {
        cout << "   🏆 LEGENDARY! You're a Snake Master!\n";
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
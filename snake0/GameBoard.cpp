#include "GameBoard.h"
#include <iostream>
using namespace std;

// Player start positions and initial directions
static const char PLAYER_DIRS[NUM_PLAYERS]  = { DIR_RIGHT, DIR_LEFT };
static const int  HEAD_COLORS[NUM_PLAYERS]  = { 10, 11 };
static const int  BODY_COLORS[NUM_PLAYERS]  = {  2,  3 };

GameBoard::GameBoard(HighScoreManager* hsm, int difficulty, const string& name) :
    level(1), gameOver(false), loser(0), highScoreManager(hsm), playerName(name) {

    for (int i = 0; i < NUM_PLAYERS; i++) scores[i] = 0;

    srand(time(0));
    initScreen();
    system("cls");

    switch(difficulty) {
        case 1: baseSpeed = 180; break;
        case 2: baseSpeed = 120; break;
        case 3: baseSpeed = 70;  break;
        default: baseSpeed = 120; break;
    }
    currentSpeed = baseSpeed;

    // Snake positions: player 0 starts at 1/3 width, player 1 at 2/3 width
    snakes[0] = new Snake(consoleWidth / 3,     consoleHeight / 2, 3, PLAYER_DIRS[0]);
    snakes[1] = new Snake(2 * consoleWidth / 3, consoleHeight / 2, 3, PLAYER_DIRS[1]);

    drawBorder();
    displayInstructions();
}

GameBoard::~GameBoard() {
    for (int i = 0; i < NUM_PLAYERS; i++) {
        delete snakes[i];
    }
}

int GameBoard::getScore() {
    int top = 0;
    for (int i = 0; i < NUM_PLAYERS; i++) top = max(top, scores[i]);
    return top;
}
int GameBoard::getLevel() { return level; }
int GameBoard::getSpeed() { return currentSpeed; }
bool GameBoard::isGameOver() { return gameOver; }

void GameBoard::updateLevel() {
    int topScore = getScore();
    int newLevel = (topScore / 10) + 1;
    if (newLevel > level) {
        level = newLevel;
        currentSpeed = max(30, baseSpeed - (level - 1) * (baseSpeed / 10));
    }
}

void GameBoard::spawnFood() {
    // Collect all snake bodies to pass to food spawn
    vector<const vector<Point>*> bodies;
    for (int i = 0; i < NUM_PLAYERS; i++) bodies.push_back(&snakes[i]->getBody());
    food.spawn(bodies, consoleWidth, consoleHeight);
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
    cout << "P1 Score: " << scores[0];

    gotoxy(20, 0);
    setColor(11);
    cout << "P2 Score: " << scores[1];

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
    gotoxy(centerX, centerY - 1); setColor(14); cout << "╔══════════════════════╗";
    gotoxy(centerX, centerY);     setColor(14); cout << "║      LEVEL UP!       ║";
    gotoxy(centerX, centerY + 1); setColor(14); cout << "║   Now at Level " << setw(2) << level << "   ║";
    gotoxy(centerX, centerY + 2); setColor(14); cout << "╚══════════════════════╝";

    for (int i = 0; i < 3; i++) {
        Sleep(300); gotoxy(centerX, centerY); setColor(12); cout << "║      LEVEL UP!       ║";
        Sleep(300); gotoxy(centerX, centerY); setColor(14); cout << "║      LEVEL UP!       ║";
    }
    for (int y = centerY - 1; y <= centerY + 2; y++) {
        gotoxy(centerX, y); cout << "                      ";
    }
}

void GameBoard::draw() {
    displayGameInfo();
    for (int i = 0; i < NUM_PLAYERS; i++) {
        snakes[i]->draw(HEAD_COLORS[i], BODY_COLORS[i]);
    }
    food.draw();
}

bool GameBoard::update() {
    if (gameOver) return false;

    for (int i = 0; i < NUM_PLAYERS; i++) snakes[i]->clearTail();

    bool foodEaten[NUM_PLAYERS];
    for (int i = 0; i < NUM_PLAYERS; i++) foodEaten[i] = snakes[i]->move(food);

    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (foodEaten[i]) {
            scores[i]++;
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
    }

    // Collision detection for each snake
    bool lost[NUM_PLAYERS] = { false, false };
    for (int i = 0; i < NUM_PLAYERS; i++) {
        bool wall  = snakes[i]->checkBoundaryCollision(consoleWidth, consoleHeight);
        bool self  = snakes[i]->checkSelfCollision();
        bool other = snakes[i]->checkOtherCollision(snakes[1 - i]->getBody());
        lost[i] = wall || self || other;
    }

    if (lost[0] && lost[1]) {
        loser = 3;  // tie
        gameOver = true;
        return false;
    } else if (lost[0]) {
        loser = 1;
        gameOver = true;
        return false;
    } else if (lost[1]) {
        loser = 2;
        gameOver = true;
        return false;
    }

    return true;
}

void GameBoard::getInput() {
    while (_kbhit()) {
        int key = _getch();
        if (key == 0 || key == 224) {
            key = _getch();
            switch (key) {
                case 72: snakes[0]->changeDirection(DIR_UP);    break;
                case 80: snakes[0]->changeDirection(DIR_DOWN);  break;
                case 75: snakes[0]->changeDirection(DIR_LEFT);  break;
                case 77: snakes[0]->changeDirection(DIR_RIGHT); break;
            }
        } else {
            switch (tolower(key)) {
                case 'w': snakes[1]->changeDirection(DIR_UP);    break;
                case 'a': snakes[1]->changeDirection(DIR_LEFT);  break;
                case 's': snakes[1]->changeDirection(DIR_DOWN);  break;
                case 'd': snakes[1]->changeDirection(DIR_RIGHT); break;
                case 'p':
                    gotoxy(consoleWidth / 2 - 5, consoleHeight / 2);
                    setColor(14); cout << "PAUSED";
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
    for (int i = 0; i < NUM_PLAYERS; i++) {
        cout << "   Player " << (i + 1) << " Score: " << scores[i] << endl;
    }
    cout << "   Level Reached: " << level << endl;
    cout << "   High Score: " << highScoreManager->getHighestScore() << endl;

    int topScore = getScore();
    int highest = highScoreManager->getHighestScore();
    if (topScore > highest && topScore > 0) {
        highScoreManager->addHighScore(playerName, topScore);
        setColor(14);
        cout << "\n   🎉 NEW HIGH SCORE! 🎉\n";
        cout << "   Congratulations!\n";
    } else if (topScore == highest && topScore > 0) {
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
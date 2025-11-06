#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <iostream>
#pragma comment(lib, "Winmm.lib")

using namespace std;

#define MAX_LENGTH 1000
#define HIGHSCORE_FILE "highscores.txt"

const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}


void initScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct Point {
    int xCoord, yCoord;
    Point() : xCoord(0), yCoord(0) {}
    Point(int x, int y) : xCoord(x), yCoord(y) {}
    
    bool operator==(const Point& other) const {
        return xCoord == other.xCoord && yCoord == other.yCoord;
    }
};

struct HighScore {
    string playerName;
    int score;
    string date;
    
    HighScore() : playerName(""), score(0), date("") {} 
    HighScore(string name, int sc, string dt) : playerName(name), score(sc), date(dt) {}
    
    bool operator<(const HighScore& other) const {
        return score < other.score; 
    }
};

class HighScoreManager {
private:
    vector<HighScore> highScores;
    const int MAX_SCORES = 3;

public:
    HighScoreManager() {
        loadHighScores();
    }
    
   void loadHighScores() {
        highScores.clear();
        ifstream file(HIGHSCORE_FILE);
        if (!file.is_open()) {
            // optional debug
            // cerr << "Could not open " << HIGHSCORE_FILE << " for reading.\n";
            return;
        }
        string name, date;
        int score;
        while (file >> name >> score >> date) {
            replace(name.begin(), name.end(), '_', ' ');
            highScores.push_back(HighScore(name, score, date));
        }
        file.close();
        sort(highScores.begin(), highScores.end(), [](const HighScore &a, const HighScore &b){
            return a.score > b.score; // descending
        });
    }

    
    void saveHighScores() {
        ofstream file(HIGHSCORE_FILE);
        if (file.is_open()) {
            for (const auto& hs : highScores) {
                string name = hs.playerName;
                replace(name.begin(), name.end(), ' ', '_'); // Replace spaces with underscores for storage
                file << name << " " << hs.score << " " << hs.date << endl;
            }
            file.close();
        }
    }
    
    void addHighScore(const string& playerName, int score) {
        time_t now = time(0);
        tm* localtm = localtime(&now);
        char dateStr[11];
        strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localtm);

        highScores.push_back(HighScore(playerName, score, dateStr));
        sort(highScores.begin(), highScores.end(), [](const HighScore &a, const HighScore &b){
            return a.score > b.score;
        });

        if ((int)highScores.size() > MAX_SCORES) {
            highScores.resize(MAX_SCORES);
        }
        saveHighScores();
    }

    
    vector<HighScore> getHighScores() {
        return highScores;
    }
    
    int getHighestScore() {
        if (highScores.empty()) return 0;
        return highScores[0].score;
    }
    
    bool isHighScore(int score) {
        if (highScores.size() < MAX_SCORES) return true;
        return score > highScores.back().score;
    }
    
    // NEW METHOD: Check if score qualifies as high score (more accurate)
    bool qualifiesAsHighScore(int score) {
        if (highScores.empty()) return true;
        if (highScores.size() < MAX_SCORES) return true;
        return score >= highScores.back().score; // allow equal to the current lowest
    }

};

class Food {
private:
    Point position;
    bool active;

public:
    Food() : active(false) {}
    
    Point getPosition() { return position; }
    bool isActive() { return active; }
    
    void spawn(const vector<Point>& snakeBody, int maxX, int maxY) {
        // Try to find a valid position not occupied by snake
        vector<Point> validPositions;
        
        for (int x = 2; x < maxX - 2; x++) {
            for (int y = 2; y < maxY - 2; y++) {
                Point candidate(x, y);
                bool occupied = false;
                
                // Check if this position is occupied by snake
                for (const Point& segment : snakeBody) {
                    if (candidate == segment) {
                        occupied = true;
                        break;
                    }
                }
                
                if (!occupied) {
                    validPositions.push_back(candidate);
                }
            }
        }
        
        if (!validPositions.empty()) {
            position = validPositions[rand() % validPositions.size()];
            active = true;
        }
    }
    
    void deactivate() {
        active = false;
    }
    
    void draw() {
        if (active) {
            gotoxy(position.xCoord, position.yCoord);
            setColor(12); // Red
            cout << "●"; // Food symbol
        }
    }
};

class Snake {
private:
    vector<Point> body;
    char direction;
    int length;

public:
    Snake(int x, int y, int initialLength = 3) {
        // Initialize snake with specified length
        for (int i = 0; i < initialLength; i++) {
            body.push_back(Point(x - i, y));
        }
        direction = DIR_RIGHT;
        length = initialLength;
    }

    const vector<Point>& getBody() { return body; }
    int getLength() { return length; }
    char getDirection() { return direction; }
    Point getHead() { return body[0]; }

    void changeDirection(char newDirection) {
        if (newDirection == DIR_UP && direction != DIR_DOWN)
            direction = newDirection;
        else if (newDirection == DIR_DOWN && direction != DIR_UP)
            direction = newDirection;
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT)
            direction = newDirection;
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT)
            direction = newDirection;
    }

    bool move(Food& food) {
        // Store current tail position
        Point newHead = body[0];
        Point oldTail = body.back();
        
        // Move body segments (except head)
        for (int i = body.size() - 1; i > 0; i--) {
            body[i] = body[i - 1];
        }
        
        // Move head based on direction
        switch (direction) {
            case DIR_UP:    newHead.yCoord--; break;
            case DIR_DOWN:  newHead.yCoord++; break;
            case DIR_LEFT:  newHead.xCoord--; break;
            case DIR_RIGHT: newHead.xCoord++; break;
        }
        
        body[0] = newHead;
        
        // Check if food is eaten
        if (food.isActive() && newHead == food.getPosition()) {
            // Grow snake by adding old tail position
            body.push_back(oldTail);
            length++;
            food.deactivate();
            return true; // Food was eaten
        }
        
        return false; // Food was not eaten
    }
    
    bool checkSelfCollision() {
        Point head = body[0];
        // Check collision with body segments (skip the head itself)
        for (int i = 1; i < body.size(); i++) {
            if (head == body[i]) {
                return true;
            }
        }
        return false;
    }
    
    bool checkBoundaryCollision(int maxX, int maxY) {
        Point head = body[0];
        return (head.xCoord <= 0 || head.xCoord >= maxX - 1 ||
                head.yCoord <= 0 || head.yCoord >= maxY - 1);
    }
    
    void draw() {
        // Draw snake head
        gotoxy(body[0].xCoord, body[0].yCoord);
        setColor(10); // Bright green for head
        cout << "■";
        
        // Draw snake body
        for (int i = 1; i < body.size(); i++) {
            gotoxy(body[i].xCoord, body[i].yCoord);
            setColor(2); // Dark green for body
            cout << "□";
        }
    }
    
    void clearTail() {
        if (!body.empty()) {
            gotoxy(body.back().xCoord, body.back().yCoord);
            cout << " ";
        }
    }
};

class GameBoard {
private:
    Snake* snake;
    Food food;
    int score;
    int level;
    int baseSpeed;
    int currentSpeed;
    bool gameOver;
    HighScoreManager* highScoreManager;
    string playerName;

public:
    GameBoard(HighScoreManager* hsm, int difficulty, const string& name) : 
        score(0), level(1), gameOver(false), highScoreManager(hsm), playerName(name) {
        srand(time(0));
        initScreen();
        
        // Set base speed based on difficulty
        switch(difficulty) {
            case 1: baseSpeed = 180; break; // Easy
            case 2: baseSpeed = 120; break; // Normal
            case 3: baseSpeed = 70;  break; // Hard
            default: baseSpeed = 120; break;
        }
        currentSpeed = baseSpeed;
        
        // Start snake in middle with initial length of 3
        snake = new Snake(consoleWidth / 2, consoleHeight / 2, 3);
    }

    ~GameBoard() { 
        delete snake; 
    }

    int getScore() { return score; }
    int getLevel() { return level; }
    int getSpeed() { return currentSpeed; }
    bool isGameOver() { return gameOver; }

    void updateLevel() {
        int newLevel = (score / 10) + 1;
        if (newLevel > level) {
            level = newLevel;
            // Increase speed by 10% each level, but don't go below 30ms
            currentSpeed = max(30, baseSpeed - (level - 1) * (baseSpeed / 10));
        }
    }

    void spawnFood() {
        food.spawn(snake->getBody(), consoleWidth, consoleHeight);
    }

    void drawBorder() {
        setColor(14); // Yellow
        for (int x = 0; x < consoleWidth; x++) {
            gotoxy(x, 0); cout << "═";
            gotoxy(x, consoleHeight - 1); cout << "═";
        }
        for (int y = 0; y < consoleHeight; y++) {
            gotoxy(0, y); cout << "║";
            gotoxy(consoleWidth - 1, y); cout << "║";
        }
        
        // Draw corners
        gotoxy(0, 0); cout << "╔";
        gotoxy(consoleWidth - 1, 0); cout << "╗";
        gotoxy(0, consoleHeight - 1); cout << "╚";
        gotoxy(consoleWidth - 1, consoleHeight - 1); cout << "╝";
        
        setColor(7); // Reset to default
    }

    void displayGameInfo() {
        gotoxy(2, 0);
        setColor(11); // Cyan
        cout << "Player: " << playerName;
        
        gotoxy(consoleWidth / 2 - 8, 0);
        setColor(14); // Yellow
        cout << "Score: " << score;
        
        gotoxy(consoleWidth / 2 + 10, 0);
        setColor(13); // Magenta
        cout << "Level: " << level;
        
        gotoxy(consoleWidth - 20, 0);
        setColor(10); // Green
        cout << "Speed: " << currentSpeed << "ms";
        
        // Show high score
        gotoxy(consoleWidth - 35, 0);
        setColor(6); // Orange/Brown
        cout << "Best: " << highScoreManager->getHighestScore();
    }

    void displayInstructions() {
        gotoxy(2, consoleHeight - 1);
        setColor(8); // Gray
        cout << "Controls: WASD or Arrow Keys | P: Pause";
    }

    void showLevelUpMessage() {
        int centerX = consoleWidth / 2 - 10;
        int centerY = consoleHeight / 2;
        MessageBeep(MB_OK);
        gotoxy(centerX, centerY - 1);
        setColor(14); // Yellow
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
        
        // Flash the message
        for (int i = 0; i < 3; i++) {
            Sleep(300);
            gotoxy(centerX, centerY);
            setColor(12); // Red
            cout << "║      LEVEL UP!       ║";
            Sleep(300);
            gotoxy(centerX, centerY);
            setColor(14); // Yellow
            cout << "║      LEVEL UP!       ║";
        }
        
        // Clear the message
        for (int y = centerY - 1; y <= centerY + 2; y++) {
            gotoxy(centerX, y);
            cout << "                      ";
        }
    }

    void draw() {
        system("cls");
        drawBorder();
        displayGameInfo();
        displayInstructions();
        
        snake->draw();
        food.draw();
    }

    bool update() {
        if (gameOver) return false;
        
        // Clear the tail position before moving
        snake->clearTail();
        
        // Move snake and check if food was eaten
        bool foodEaten = snake->move(food);
        
        if (foodEaten) {
            score++;
            MessageBeep(MB_ICONASTERISK);
            int oldLevel = level;
            updateLevel();
            
            // Show level up message if level changed
            if (level > oldLevel) {
                draw(); // Redraw the game first
                showLevelUpMessage();
                draw(); // Redraw again to restore game state
            }
            
            spawnFood();
        }
        
        // Check collisions
        if (snake->checkSelfCollision() || snake->checkBoundaryCollision(consoleWidth, consoleHeight)) {
            gameOver = true;
            return false;
        }
        
        return true;
    }

    void getInput() {
        if (_kbhit()) {
            int key = _getch();

            // Handle arrow keys
            if (key == 0 || key == 224) {
                key = _getch();
                switch (key) {
                    case 72: snake->changeDirection(DIR_UP); break;
                    case 80: snake->changeDirection(DIR_DOWN); break;
                    case 75: snake->changeDirection(DIR_LEFT); break;
                    case 77: snake->changeDirection(DIR_RIGHT); break;
                }
            } else {
                // Handle WASD keys
                switch (tolower(key)) {
                    case 'w': snake->changeDirection(DIR_UP); break;
                    case 'a': snake->changeDirection(DIR_LEFT); break;
                    case 's': snake->changeDirection(DIR_DOWN); break;
                    case 'd': snake->changeDirection(DIR_RIGHT); break;
                    case 'p': // Pause functionality
                        gotoxy(consoleWidth / 2 - 5, consoleHeight / 2);
                        setColor(14);
                        cout << "PAUSED";
                        while (_getch() != 'p') {} // Wait for 'p' to resume
                        break;
                }
            }
        }
    }
    
    void displayGameOver() {
        system("cls");
        MessageBeep(MB_ICONHAND);
        setColor(12); // Red
        cout << "\n\n";
        cout << "   ╔══════════════════════════════╗\n";
        cout << "   ║         GAME OVER!           ║\n";
        cout << "   ╚══════════════════════════════╝\n\n";
        
        setColor(7); // White
        cout << "   Player: " << playerName << endl;
        cout << "   Final Score: " << score << endl;
        cout << "   Level Reached: " << level << endl;
        cout << "   High Score: " << highScoreManager->getHighestScore() << endl;
        
        int highest = highScoreManager->getHighestScore();
        if (score > highest && score > 0) {
            highScoreManager->addHighScore(playerName, score);
            setColor(14); // Yellow
            cout << "\n   🎉 NEW HIGH SCORE! 🎉\n";
            cout << "   Congratulations, " << playerName << "!\n";
        }
        else if (score == highest && score > 0) {
            setColor(11); // Cyan
            cout << "\n   ⭐ You matched the high score! ⭐\n";
        }

        
        // Display performance message based on level
        cout << "\n";
        setColor(11); // Cyan
        if (level >= 10) {
            cout << "   🏆 LEGENDARY! You're a Snake Master!\n";
        } else if (level >= 7) {
            cout << "   ⭐ EXCELLENT! Great skills!\n";
        } else if (level >= 4) {
            cout << "   👍 GOOD JOB! Keep practicing!\n";
        } else {
            cout << "   💪 Nice try! You'll do better next time!\n";
        }
        
        setColor(10); // Green
        cout << "\n   Press any key to continue...";
        _getch();
    }
};

class GameManager {
private:
    string playerName;
    HighScoreManager highScoreManager;

public:
    GameManager() {}
    
    void showMainMenu() {
        system("cls");
        setColor(11); // Cyan
        cout << "╔══════════════════════════════════════╗\n";
        cout << "║           🐍 SNAKE GAME 🐍           ║\n";
        cout << "║                                      ║\n";
        cout << "║          Classic Arcade Fun!         ║\n";
        cout << "╚══════════════════════════════════════╝\n\n";
        
        setColor(7); // White
        cout << "1. Start New Game\n";
        cout << "2. View High Scores\n";
        cout << "3. Instructions\n";
        cout << "4. Level Progression Info\n";
        cout << "5. Exit\n\n";
        cout << "Enter your choice (1-5): ";
    }
    
    void showLevelProgressionInfo() {
        system("cls");
        setColor(14); // Yellow
        cout << "╔══════════════════════════════════════╗\n";
        cout << "║         LEVEL PROGRESSION           ║\n";
        cout << "╚══════════════════════════════════════╝\n\n";
        
        setColor(7); // White
        cout << "🎯 HOW LEVELS WORK:\n";
        cout << "   • Start at Level 1\n";
        cout << "   • Advance to next level every 10 points\n";
        cout << "   • Speed increases with each level\n";
        cout << "   • Challenge yourself to reach higher levels!\n\n";
        
        cout << "⚡ SPEED PROGRESSION:\n";
        cout << "   • Easy:   180ms → 162ms → 144ms ...\n";
        cout << "   • Normal: 120ms → 108ms → 96ms  ...\n";
        cout << "   • Hard:    70ms →  63ms →  56ms ...\n\n";
        
        cout << "🏆 LEVEL MILESTONES:\n";
        cout << "   • Level 5:  Intermediate Snake Charmer\n";
        cout << "   • Level 10: Advanced Snake Master\n";
        cout << "   • Level 15: Legendary Snake God\n\n";
        
        setColor(10); // Green
        cout << "Press any key to return to menu...";
        _getch();
    }
    
    void showHighScores() {
        system("cls");
        setColor(14); // Yellow
        cout << "╔══════════════════════════════════════╗\n";
        cout << "║            HIGH SCORES               ║\n";
        cout << "╚══════════════════════════════════════╝\n\n";
        
        vector<HighScore> scores = highScoreManager.getHighScores();
        
        if (scores.empty()) {
            setColor(8); // Gray
            cout << "    No high scores yet!\n";
            cout << "    Play a game to set a record!\n";
        } else {
            setColor(11); // Cyan
            cout << " Rank  Player           Score    Level  Date\n";
            cout << " ────────────────────────────────────────────\n";
            
            for (int i = 0; i < scores.size() && i < 10; i++) {
                setColor(7); // White
                
                // Highlight top 3 scores with different colors
                if (i == 0) setColor(14); // Gold for 1st
                else if (i == 1) setColor(8); // Silver for 2nd  
                else if (i == 2) setColor(6); // Bronze for 3rd
                
                cout << "  ";
                // Rank with medal emojis for top 3
                if (i == 0) cout << "🥇 ";
                else if (i == 1) cout << "🥈 ";
                else if (i == 2) cout << "🥉 ";
                else cout << (i + 1) << ".  ";
                
                // Player name (truncate if too long)
                string name = scores[i].playerName;
                if (name.length() > 10) {
                    name = name.substr(0, 10) + "..";
                }
                cout << setw(12) << left << name;
                
                // Score and calculated level
                int level = (scores[i].score / 10) + 1;
                cout << setw(8) << right << scores[i].score;
                cout << setw(8) << right << level;
                
                // Date
                cout << "  " << scores[i].date << endl;
            }
        }
        
        setColor(10); // Green
        cout << "\nPress any key to return to menu...";
        _getch();
    }
    
    void showInstructions() {
        system("cls");
        setColor(14); // Yellow
        cout << "╔══════════════════════════════════════╗\n";
        cout << "║            INSTRUCTIONS              ║\n";
        cout << "╚══════════════════════════════════════╝\n\n";
        
        setColor(7); // White
        cout << "🎯 OBJECTIVE:\n";
        cout << "   • Guide the snake to eat food (●)\n";
        cout << "   • Grow longer without hitting walls or yourself\n";
        cout << "   • Reach higher levels by scoring points\n\n";
        
        cout << "🎮 CONTROLS:\n";
        cout << "   • W or ↑ - Move Up\n";
        cout << "   • A or ← - Move Left\n";
        cout << "   • S or ↓ - Move Down\n";
        cout << "   • D or → - Move Right\n";
        cout << "   • P - Pause Game\n\n";
        
        cout << "⚡ GAME RULES:\n";
        cout << "   • Each food gives +1 point\n";
        cout << "   • Snake grows longer when eating\n";
        cout << "   • Game ends if you hit walls or yourself\n";
        cout << "   • Level up every 10 points (speed increases)\n\n";
        
        cout << "🏆 SCORING:\n";
        cout << "   • Try to beat the high score!\n";
        cout << "   • Top 10 scores are saved\n";
        cout << "   • Higher levels = higher prestige!\n\n";
        
        setColor(10); // Green
        cout << "Press any key to return to menu...";
        _getch();
    }
    
    void startGame() {
        cout << "\nEnter your name: ";
        cin >> playerName;
        
        // If player name is empty, set default
        if (playerName.empty()) {
            playerName = "Player";
        }
        
        int modeChoice;
        cout << "\n🎮 SELECT DIFFICULTY:\n";
        cout << "1. Easy (Slow) - Good for beginners\n";
        cout << "2. Normal (Medium) - Balanced gameplay\n";
        cout << "3. Hard (Fast) - For snake masters!\n";
        cout << "Enter choice (1-3): ";
        cin >> modeChoice;
        
        if (modeChoice < 1 || modeChoice > 3) {
            cout << "Invalid choice! Defaulting to Normal mode.\n";
            modeChoice = 2;
        }
        
        GameBoard* board = new GameBoard(&highScoreManager, modeChoice, playerName);
        board->spawnFood();
        
        // Show starting level info
        system("cls");
        cout << "\n\n   Starting at Level 1!\n";
        cout << "   Reach Level 10 to become a Snake Master!\n";
        cout << "   Current High Score: " << highScoreManager.getHighestScore() << "\n";
        cout << "   Good luck, " << playerName << "!\n\n";
        cout << "   Press any key to start...";
        _getch();
        
        while (board->update()) {
            board->getInput();
            board->draw();
            Sleep(board->getSpeed());
        }
        
        board->displayGameOver();
        delete board;
    }
    
    void run() {
        hideCursor();
        
        while (true) {
            showMainMenu();
            
            char choice;
            cin >> choice;
            
            switch (choice) {
                case '1':
                    startGame();
                    break;
                case '2':
                    showHighScores();
                    break;
                case '3':
                    showInstructions();
                    break;
                case '4':
                    showLevelProgressionInfo();
                    break;
                case '5':
                    cout << "\nThanks for playing! Goodbye 👋\n";
                    return;
                default:
                    cout << "Invalid choice! Press any key to continue...";
                    _getch();
            }
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    GameManager game;
    game.run();
    
    return 0;
}
#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

#define MAX_LENGTH 1000

const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

// 🟢 Set console text color
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

// 🟢 Initialize console screen dimensions
void initScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

// 🟢 Move cursor
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

struct Point {
    int xCoord, yCoord;
    Point() {}
    Point(int x, int y) : xCoord(x), yCoord(y) {}
};

class Snake {
    int length;
    char direction;

public:
    Point body[MAX_LENGTH];
    Snake(int x, int y) {
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    int getLength() { return length; }
    char getDirection() { return direction; }

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

    bool move(Point food) {
        Point lastTail = body[length - 1];

        for (int i = length - 1; i > 0; i--)
            body[i] = body[i - 1];

        switch (direction) {
            case DIR_UP:    body[0].yCoord--; break;
            case DIR_DOWN:  body[0].yCoord++; break;
            case DIR_LEFT:  body[0].xCoord--; break;
            case DIR_RIGHT: body[0].xCoord++; break;
        }

        // Self collision
        for (int i = 1; i < length; i++)
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord)
                return false;

        // Eat food → grow
        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord) {
            if (length < MAX_LENGTH)
                body[length++] = lastTail;
        } else {
            gotoxy(lastTail.xCoord, lastTail.yCoord);
            cout << " ";
        }

        return true;
    }
};

class Board {
    Snake* snake;
    Point food;
    int score;

public:
    Board() {
        srand(time(0));
        initScreen();
        snake = new Snake(consoleWidth / 2, consoleHeight / 2);
        score = 0;
        spawnFood();
    }

    ~Board() { delete snake; }

    int getScore() { return score; }

    void spawnFood() {
        food = Point(rand() % (consoleWidth - 4) + 2, rand() % (consoleHeight - 5) + 2);
    }

    void drawBorder() {
        setColor(14); // Yellow
        for (int x = 0; x < consoleWidth; x++) {
            gotoxy(x, 0); cout << "-";
            gotoxy(x, consoleHeight - 1); cout << "|";
        }
        for (int y = 0; y < consoleHeight; y++) {
            gotoxy(0, y); cout << "-";
            gotoxy(consoleWidth - 1, y); cout << "|";
        }
        setColor(7);
    }

    void displayCurrentScore(const string& playerName) {
        gotoxy(2, 0);
        setColor(7);
        cout << "Player: " << playerName;
        gotoxy(consoleWidth / 2 - 8, 0);
        cout << "Score: " << score;
    }

    // 🖼️ Draw everything
    void draw(const string& playerName) {
        // Move cursor to top-left without clearing screen
        COORD cursorPos = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPos);

        drawBorder();

        // 🟢 Draw green snake (consistent width)
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            setColor(10); // bright green
            cout << "■"; // perfect single-width block
        }

        // 🍎 Draw apple (food)
        gotoxy(food.xCoord, food.yCoord);
        setColor(12);
        cout << "🍎";

        setColor(7);
        displayCurrentScore(playerName);
    }


    bool update() {
        bool alive = snake->move(food);
        if (!alive) return false;

        Point head = snake->body[0];
        if (head.xCoord <= 0 || head.xCoord >= consoleWidth - 1 ||
            head.yCoord <= 0 || head.yCoord >= consoleHeight - 1)
            return false;

        if (head.xCoord == food.xCoord && head.yCoord == food.yCoord) {
            score++;
            spawnFood();
        }

        return true;
    }

    void getInput() {
        if (_kbhit()) {
            int key = _getch();
            switch (tolower(key)) {
                case 'w': snake->changeDirection(DIR_UP); break;
                case 'a': snake->changeDirection(DIR_LEFT); break;
                case 's': snake->changeDirection(DIR_DOWN); break;
                case 'd': snake->changeDirection(DIR_RIGHT); break;
            }
        }
    }
};

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    hideCursor();

    string playerName;
    int modeChoice;
    bool playAgain = true;

    while (playAgain) {
        system("cls");
        setColor(11);
        cout << "============================\n";
        cout << "         🐍 SNAKE GAME \n";
        cout << "============================\n";
        setColor(7);

        cout << "\nEnter your name: ";
        cin >> playerName;

        cout << "\nSelect Game Mode:\n";
        cout << "1. Easy (Slow)\n";
        cout << "2. Normal (Medium)\n";
        cout << "3. Fast (Hard)\n";
        cout << "Enter choice (1-3): ";
        cin >> modeChoice;

        int SPEED = (modeChoice == 1) ? 180 : (modeChoice == 2) ? 120 : 70;

        // 🧹 Clear screen before starting game (fix for overlap)
        system("cls");

        Board* board = new Board();

        while (board->update()) {
            board->getInput();
            board->draw(playerName);
            Sleep(SPEED);
        }

        system("cls");
        setColor(12);
        cout << "\n\n💀 GAME OVER!\n";
        setColor(7);
        cout << "Player: " << playerName << endl;
        cout << "Final Score: " << board->getScore() << endl;

        delete board;

        char again;
        cout << "\nDo you want to play again? (y/n): ";
        cin >> again;
        playAgain = (again == 'y' || again == 'Y');
    }

    cout << "\nThanks for playing! Goodbye 👋\n";
    return 0;
}

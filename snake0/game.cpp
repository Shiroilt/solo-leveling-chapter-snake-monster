#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;

#define MAX_LENGTH 1000

// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
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
    Snake(int x, int y)
    {
        length = 1;
        body[0] = Point(x, y);
        direction = DIR_RIGHT;
    }

    int getLength() { return length; }

    void changeDirection(char newDirection)
    {
        if (newDirection == DIR_UP && direction != DIR_DOWN)
            direction = newDirection;
        else if (newDirection == DIR_DOWN && direction != DIR_UP)
            direction = newDirection;
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT)
            direction = newDirection;
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT)
            direction = newDirection;
    }

    bool move(Point food)
    {
        // Move body
        for (int i = length - 1; i > 0; i--)
            body[i] = body[i - 1];

        switch (direction) {
        case DIR_UP:    body[0].yCoord--; break;
        case DIR_DOWN:  body[0].yCoord++; break;
        case DIR_LEFT:  body[0].xCoord--; break;
        case DIR_RIGHT: body[0].xCoord++; break;
        }

        // Check self-collision
        for (int i = 1; i < length; i++)
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord)
                return false;

        // Eat food → increase length
        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord)
            body[length++] = Point(body[length - 1].xCoord, body[length - 1].yCoord);

        return true;
    }
};

class Board {
    Snake* snake;
    const char SNAKE_BODY = 'O';
    const char FOOD = 'o';
    Point food;
    int score;

public:
    Board()
    {
        srand(time(0));
        initScreen();
        snake = new Snake(consoleWidth / 2, consoleHeight / 2);
        score = 0;
        spawnFood();
    }

    ~Board() { delete snake; }

    int getScore() { return score; }

    void spawnFood()
    {
        food = Point(rand() % (consoleWidth - 2) + 1, rand() % (consoleHeight - 3) + 2);
    }

    void gotoxy(int x, int y)
    {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void drawBorder()
    {
        for (int x = 0; x < consoleWidth; x++) {
            gotoxy(x, 0);
            cout << "#";
            gotoxy(x, consoleHeight - 1);
            cout << "#";
        }
        for (int y = 0; y < consoleHeight; y++) {
            gotoxy(0, y);
            cout << "#";
            gotoxy(consoleWidth - 1, y);
            cout << "#";
        }
    }

    void displayCurrentScore(string playerName)
    {
        gotoxy(2, 0);
        cout << "Player: " << playerName;
        gotoxy(consoleWidth / 2 - 8, 0);
        cout << "Score: " << score;
    }

    void draw(string playerName)
    {
        system("cls");
        drawBorder();

        // Draw snake
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            cout << SNAKE_BODY;
        }

        // Draw food
        gotoxy(food.xCoord, food.yCoord);
        cout << FOOD;

        // Score display
        displayCurrentScore(playerName);
    }

    bool update()
    {
        bool isAlive = snake->move(food);
        if (!isAlive)
            return false;

        // Out of bounds → game over
        Point head = snake->body[0];
        if (head.xCoord <= 0 || head.xCoord >= consoleWidth - 1 ||
            head.yCoord <= 0 || head.yCoord >= consoleHeight - 1)
            return false;

        // Eat food
        if (head.xCoord == food.xCoord && head.yCoord == food.yCoord) {
            score++;
            spawnFood();
        }

        return true;
    }

    void getInput()
    {
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

int main()
{
    string playerName;
    int modeChoice;
    bool playAgain = true;

    while (playAgain) {
        system("cls");
        cout << "============================\n";
        cout << "         SNAKE GAME \n";
        cout << "============================\n";
        cout << "\nEnter your name: ";
        cin >> playerName;

        cout << "\nSelect Game Mode:\n";
        cout << "1. Easy (Slow)\n";
        cout << "2. Normal (Medium)\n";
        cout << "3. Fast (Hard)\n";
        cout << "Enter choice (1-3): ";
        cin >> modeChoice;

        int SPEED;
        if (modeChoice == 1)
            SPEED = 180;
        else if (modeChoice == 2)
            SPEED = 120;
        else
            SPEED = 70;

        Board* board = new Board();

        while (board->update()) {
            board->getInput();
            board->draw(playerName);
            Sleep(SPEED);
        }

        system("cls");
        cout << "\n\nGAME OVER!\n";
        cout << "Player: " << playerName << endl;
        cout << "Final Score: " << board->getScore() << endl;

        delete board;

        char again;
        cout << "\nDo you want to play again? (y/n): ";
        cin >> again;
        playAgain = (again == 'y' || again == 'Y');
    }

    cout << "\nThanks for playing! Goodbye \n";
    return 0;
}

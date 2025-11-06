#include "Snake.h"
#include <iostream>
using namespace std;
Snake::Snake(int x, int y, int initialLength) {
    for (int i = 0; i < initialLength; i++) {
        body.push_back(Point(x - i, y));
    }
    direction = DIR_RIGHT;
    length = initialLength;
}

const vector<Point>& Snake::getBody() { return body; }
int Snake::getLength() { return length; }
char Snake::getDirection() { return direction; }
Point Snake::getHead() { return body[0]; }

void Snake::changeDirection(char newDirection) {
    if (newDirection == DIR_UP && direction != DIR_DOWN)
        direction = newDirection;
    else if (newDirection == DIR_DOWN && direction != DIR_UP)
        direction = newDirection;
    else if (newDirection == DIR_LEFT && direction != DIR_RIGHT)
        direction = newDirection;
    else if (newDirection == DIR_RIGHT && direction != DIR_LEFT)
        direction = newDirection;
}

bool Snake::move(Food& food) {
    Point newHead = body[0];
    Point oldTail = body.back();
    
    for (int i = body.size() - 1; i > 0; i--) {
        body[i] = body[i - 1];
    }
    
    switch (direction) {
        case DIR_UP:    newHead.yCoord--; break;
        case DIR_DOWN:  newHead.yCoord++; break;
        case DIR_LEFT:  newHead.xCoord--; break;
        case DIR_RIGHT: newHead.xCoord++; break;
    }
    
    body[0] = newHead;
    
    if (food.isActive() && newHead == food.getPosition()) {
        body.push_back(oldTail);
        length++;
        food.deactivate();
        return true;
    }
    
    return false;
}

bool Snake::checkSelfCollision() {
    Point head = body[0];
    for (int i = 1; i < body.size(); i++) {
        if (head == body[i]) {
            return true;
        }
    }
    return false;
}

bool Snake::checkBoundaryCollision(int maxX, int maxY) {
    Point head = body[0];
    return (head.xCoord <= 0 || head.xCoord >= maxX - 1 ||
            head.yCoord <= 0 || head.yCoord >= maxY - 1);
}

void Snake::draw() {
    gotoxy(body[0].xCoord, body[0].yCoord);
    setColor(10);
    cout << "■";
    
    for (int i = 1; i < body.size(); i++) {
        gotoxy(body[i].xCoord, body[i].yCoord);
        setColor(2);
        cout << "□";
    }
}

void Snake::clearTail() {
    if (!body.empty()) {
        gotoxy(body.back().xCoord, body.back().yCoord);
        cout << " ";
    }
}
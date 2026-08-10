#ifndef SNAKE_H
#define SNAKE_H

#include "Common.h"
#include "Food.h"
#include <vector>

class Snake {
private:
    vector<Point> body;
    char direction;
    int length;

public:
    Snake(int x, int y, int initialLength = 3, char initialDirection = DIR_RIGHT);
    const vector<Point>& getBody();
    int getLength();
    char getDirection();
    Point getHead();
    void changeDirection(char newDirection);
    bool move(Food& food);
    bool checkSelfCollision();
    bool checkOtherCollision(const vector<Point>& otherBody);
    bool checkBoundaryCollision(int maxX, int maxY);
    void draw(int headColor = 10, int bodyColor = 2);
    void clearTail();
};

#endif
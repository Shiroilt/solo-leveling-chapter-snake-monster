#ifndef FOOD_H
#define FOOD_H

#include "Common.h"
#include <vector>

class Food {
private:
    Point position;
    bool active;

public:
    Food();
    Point getPosition();
    bool isActive();
    void spawn(const vector<Point>& snakeBody, int maxX, int maxY);
    void deactivate();
    void draw();
};

#endif
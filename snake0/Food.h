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
    // Takes a list of all snake bodies so food never spawns on any snake
    void spawn(const vector<const vector<Point>*>& snakeBodies, int maxX, int maxY);
    void deactivate();
    void draw();
};

#endif
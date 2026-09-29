#include <cassert>
#include <iostream>
#include <vector>
#include "../snake0/Food.h"

void runFoodTests() {
    Food food;

    // Initially inactive
    assert(food.isActive() == false);

    // Create a small snake body
    std::vector<Point> snakeBody = {
        Point(5, 5),
        Point(5, 6),
        Point(5, 7)
    };

    // Spawn food
    food.spawn(snakeBody, 20, 20);

    // Should now be active
    assert(food.isActive() == true);

    Point p = food.getPosition();

    // Food should be inside the board
    assert(p.xCoord >= 1);
    assert(p.xCoord < 19);
    assert(p.yCoord >= 1);
    assert(p.yCoord < 19);

    // Food should not spawn on the snake
    for (const auto& segment : snakeBody) {
        assert(!(segment == p));
    }

    // Deactivate
    food.deactivate();
    assert(food.isActive() == false);

    std::cout << "Food tests passed\\n";
}
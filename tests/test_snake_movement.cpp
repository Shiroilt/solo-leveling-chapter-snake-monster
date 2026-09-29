#include <cassert>
#include <iostream>
#include "../snake0/Snake.h"
#include "../snake0/Food.h"

void runSnakeMovementTests() {
    Food food;

    // Test moving right
    {
        Snake snake(10, 10, 3);

        Point before = snake.getHead();
        snake.move(food);
        Point after = snake.getHead();

        assert(after.xCoord == before.xCoord + 1);
        assert(after.yCoord == before.yCoord);
    }

    // Test moving up
    {
        Snake snake(10, 10, 3);

        snake.changeDirection(DIR_UP);

        Point before = snake.getHead();
        snake.move(food);
        Point after = snake.getHead();

        assert(after.xCoord == before.xCoord);
        assert(after.yCoord == before.yCoord - 1);
    }

    // Test moving down
    {
        Snake snake(10, 10, 3);

        snake.changeDirection(DIR_DOWN);

        Point before = snake.getHead();
        snake.move(food);
        Point after = snake.getHead();

        assert(after.xCoord == before.xCoord);
        assert(after.yCoord == before.yCoord + 1);
    }

    // Test reverse direction prevention
    {
        Snake snake(10, 10, 3);

        snake.changeDirection(DIR_LEFT);

        // Should remain RIGHT
        assert(snake.getDirection() == DIR_RIGHT);
    }

    std::cout << "Snake movement tests passed\\n";
}
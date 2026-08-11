#include <cassert>
#include <iostream>
#include "../snake0/Snake.h"

void runSnakeInitializationTests() {
    Snake snake(10, 10, 3);

    assert(snake.getLength() == 3);
    assert(snake.getDirection() == DIR_RIGHT);

    Point head = snake.getHead();
    assert(head.xCoord == 10);
    assert(head.yCoord == 10);

    const std::vector<Point>& body = snake.getBody();
    assert(body.size() == 3);

    std::cout << "Snake initialization tests passed\\n";
}
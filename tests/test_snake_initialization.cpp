#include <iostream>
#include <cassert>
#include "../snake0/Snake.h"

void test_initial_length() {
    Snake snake(10, 10, 3);

    assert(snake.getLength() == 3);

    std::cout << "PASS: Initial snake length is correct\\n";
}

void test_initial_direction() {
    Snake snake(10, 10, 3);

    assert(snake.getDirection() == DIR_RIGHT);

    std::cout << "PASS: Initial snake direction is RIGHT\\n";
}

void test_initial_head_position() {
    Snake snake(10, 10, 3);

    Point head = snake.getHead();

    assert(head.x == 10);
    assert(head.y == 10);

    std::cout << "PASS: Initial head position is correct\\n";
}

int main() {
    std::cout << "Running Snake Initialization Tests...\\n\\n";

    test_initial_length();
    test_initial_direction();
    test_initial_head_position();

    std::cout << "\\nAll initialization tests passed successfully!\\n";

    return 0;
}

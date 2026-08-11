#include <iostream>
#include <cassert>
#include "../snake0/Snake.h"

void test_move_right() {
    Snake snake(10, 10, 3);

    Point before = snake.getHead();
    snake.move();
    Point after = snake.getHead();

    assert(after.x == before.x + 1);
    assert(after.y == before.y);

    std::cout << "PASS: Snake moves right correctly\\n";
}

void test_move_up() {
    Snake snake(10, 10, 3);

    snake.changeDirection(DIR_UP);

    Point before = snake.getHead();
    snake.move();
    Point after = snake.getHead();

    assert(after.x == before.x);
    assert(after.y == before.y - 1);

    std::cout << "PASS: Snake moves up correctly\\n";
}

void test_move_down() {
    Snake snake(10, 10, 3);

    snake.changeDirection(DIR_DOWN);

    Point before = snake.getHead();
    snake.move();
    Point after = snake.getHead();

    assert(after.x == before.x);
    assert(after.y == before.y + 1);

    std::cout << "PASS: Snake moves down correctly\\n";
}

void test_no_reverse_direction() {
    Snake snake(10, 10, 3);

    // Initial direction is RIGHT
    snake.changeDirection(DIR_LEFT);

    assert(snake.getDirection() == DIR_RIGHT);

    std::cout << "PASS: Reverse direction is prevented\\n";
}

int main() {
    std::cout << "Running Snake Movement Tests...\\n\\n";

    test_move_right();
    test_move_up();
    test_move_down();
    test_no_reverse_direction();

    std::cout << "\\nAll movement tests passed successfully!\\n";

    return 0;
}

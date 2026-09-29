#include <iostream>
#include <cassert>
#include "../snake0/Snake.h"

void test_wall_collision_right() {
    // Create snake near the right wall
    Snake snake(19, 10, 3);

    snake.move();

    assert(snake.hasCollided());

    std::cout << "PASS: Right wall collision detected\\n";
}

void test_wall_collision_left() {
    Snake snake(0, 10, 3);

    snake.changeDirection(DIR_LEFT);
    snake.move();

    assert(snake.hasCollided());

    std::cout << "PASS: Left wall collision detected\\n";
}

void test_wall_collision_top() {
    Snake snake(10, 0, 3);

    snake.changeDirection(DIR_UP);
    snake.move();

    assert(snake.hasCollided());

    std::cout << "PASS: Top wall collision detected\\n";
}

void test_wall_collision_bottom() {
    Snake snake(10, 19, 3);

    snake.changeDirection(DIR_DOWN);
    snake.move();

    assert(snake.hasCollided());

    std::cout << "PASS: Bottom wall collision detected\\n";
}

int main() {
    std::cout << "Running Snake Collision Tests...\\n\\n";

    test_wall_collision_right();
    test_wall_collision_left();
    test_wall_collision_top();
    test_wall_collision_bottom();

    std::cout << "\\nAll collision tests passed successfully!\\n";

    return 0;
}

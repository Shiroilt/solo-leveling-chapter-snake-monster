#include <iostream>
#include <cassert>
#include "../snake0/Snake.h"
#include "../snake0/Food.h"

void test_snake_grows_after_eating() {
    Snake snake(10, 10, 3);
    Food food;

    // Place food directly in front of the snake
    Point head = snake.getHead();
    food.setPosition({head.x + 1, head.y});

    int initialLength = snake.getLength();

    snake.move();

    // If your game uses a separate eat/check function,
    // replace this with the correct method call.
    if (snake.getHead().x == food.getPosition().x &&
        snake.getHead().y == food.getPosition().y) {
        snake.grow();
    }

    assert(snake.getLength() == initialLength + 1);

    std::cout << "PASS: Snake grows after eating food\\n";
}

void test_multiple_food_growth() {
    Snake snake(10, 10, 3);

    snake.grow();
    snake.grow();

    assert(snake.getLength() == 5);

    std::cout << "PASS: Snake grows correctly multiple times\\n";
}

int main() {
    std::cout << "Running Snake Growth Tests...\\n\\n";

    test_snake_grows_after_eating();
    test_multiple_food_growth();

    std::cout << "\\nAll growth tests passed successfully!\\n";

    return 0;
}

#include <iostream>
#include <cassert>
#include "../snake0/Snake.h"
#include "../snake0/Food.h"
#include "../snake0/GameBoard.h"

void test_snake_eats_food_and_scores() {
    Snake snake(10, 10, 3);
    Food food;
    GameBoard board;

    // Place food directly in front of the snake
    Point head = snake.getHead();
    food.setPosition({head.x + 1, head.y});

    int initialLength = snake.getLength();
    int initialScore = board.getScore();

    // Snake moves onto the food
    snake.move();

    if (snake.getHead().x == food.getPosition().x &&
        snake.getHead().y == food.getPosition().y) {
        snake.grow();
        board.addScore(10);
        food.deactivate();
    }

    assert(snake.getLength() == initialLength + 1);
    assert(board.getScore() == initialScore + 10);
    assert(food.isActive() == false);

    std::cout << "PASS: Snake, Food, and GameBoard integrate correctly\\n";
}

void test_multiple_food_collection() {
    Snake snake(5, 5, 3);
    GameBoard board;

    snake.grow();
    board.addScore(10);

    snake.grow();
    board.addScore(10);

    assert(snake.getLength() == 5);
    assert(board.getScore() == 20);

    std::cout << "PASS: Multiple food collections update game state correctly\\n";
}

int main() {
    std::cout << "Running Integration Tests...\\n\\n";

    test_snake_eats_food_and_scores();
    test_multiple_food_collection();

    std::cout << "\\nAll integration tests passed successfully!\\n";

    return 0;
}

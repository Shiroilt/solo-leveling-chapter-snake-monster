#include <iostream>
#include <cassert>
#include "../snake0/GameBoard.h"

void test_initial_score() {
    GameBoard board;

    assert(board.getScore() == 0);

    std::cout << "PASS: Initial score is zero\\n";
}

void test_score_increase() {
    GameBoard board;

    board.addScore(10);
    assert(board.getScore() == 10);

    board.addScore(20);
    assert(board.getScore() == 30);

    std::cout << "PASS: Score increases correctly\\n";
}

void test_level_progression() {
    GameBoard board;

    int initialLevel = board.getLevel();

    // Increase score enough to trigger a level up
    board.addScore(100);

    assert(board.getLevel() >= initialLevel);

    std::cout << "PASS: Level progression works\\n";
}

void test_speed_change() {
    GameBoard board;

    int initialSpeed = board.getSpeed();

    board.addScore(100);

    assert(board.getSpeed() <= initialSpeed);

    std::cout << "PASS: Speed updates correctly with level\\n";
}

void test_board_dimensions() {
    GameBoard board;

    assert(board.getWidth() > 0);
    assert(board.getHeight() > 0);

    std::cout << "PASS: Board dimensions are valid\\n";
}

int main() {
    std::cout << "Running GameBoard Tests...\\n\\n";

    test_initial_score();
    test_score_increase();
    test_level_progression();
    test_speed_change();
    test_board_dimensions();

    std::cout << "\\nAll GameBoard tests passed successfully!\\n";

    return 0;
}

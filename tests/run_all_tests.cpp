#include <iostream>

// Forward declarations
void runSnakeInitializationTests();
void runSnakeMovementTests();
void runFoodTests();
void runHighScoreTests();

int main() {
    std::cout << "====================================\\n";
    std::cout << "   SOLO LEVELING C++ TEST SUITE\\n";
    std::cout << "====================================\\n\\n";

    runSnakeInitializationTests();
    runSnakeMovementTests();
    runFoodTests();
    runHighScoreTests();

    std::cout << "\\n====================================\\n";
    std::cout << "All tests passed successfully!\\n";
    std::cout << "====================================\\n";

    return 0;
}
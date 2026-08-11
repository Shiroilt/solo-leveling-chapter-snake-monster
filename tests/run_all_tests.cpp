#include <iostream>
#include <exception>

// Forward declarations of test suites
void runSnakeInitializationTests();
void runSnakeMovementTests();
void runSnakeGrowthTests();
void runSnakeCollisionTests();
void runFoodTests();
void runHighScoreTests();
void runGameBoardTests();
void runIntegrationTests();

int main() {
    std::cout << "====================================\\n";
    std::cout << "     SOLO LEVELING SNAKE TESTS\\n";
    std::cout << "====================================\\n\\n";

    int passed = 0;
    int failed = 0;

    auto runTest = [&](const std::string& name, void (*testFunc)()) {
        std::cout << "Running " << name << "...\\n";
        try {
            testFunc();
            std::cout << "[PASS] " << name << "\\n\\n";
            passed++;
        } catch (const std::exception& e) {
            std::cout << "[FAIL] " << name << ": " << e.what() << "\\n\\n";
            failed++;
        } catch (...) {
            std::cout << "[FAIL] " << name << ": Unknown error\\n\\n";
            failed++;
        }
    };

    runTest("Snake Initialization", runSnakeInitializationTests);
    runTest("Snake Movement", runSnakeMovementTests);
    runTest("Snake Growth", runSnakeGrowthTests);
    runTest("Snake Collision", runSnakeCollisionTests);
    runTest("Food", runFoodTests);
    runTest("High Score", runHighScoreTests);
    runTest("GameBoard", runGameBoardTests);
    runTest("Integration", runIntegrationTests);

    std::cout << "====================================\\n";
    std::cout << "Test Summary\\n";
    std::cout << "====================================\\n";
    std::cout << "Passed: " << passed << "\\n";
    std::cout << "Failed: " << failed << "\\n";

    if (failed == 0) {
        std::cout << "\\nAll tests passed successfully!\\n";
        return 0;
    } else {
        std::cout << "\\nSome tests failed!\\n";
        return 1;
    }
}

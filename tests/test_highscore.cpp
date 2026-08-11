#include <iostream>
#include <cassert>
#include <cstdio>
#include "../snake0/HighScoreManager.h"

const std::string TEST_FILE = "test_highscore.dat";

void cleanup() {
    std::remove(TEST_FILE.c_str());
}

void test_save_highscore() {
    cleanup();

    HighScoreManager manager(TEST_FILE);

    manager.saveScore("Player1", 150);

    int highest = manager.getHighestScore();

    assert(highest == 150);

    std::cout << "PASS: High score saved correctly\\n";
}

void test_load_highscore() {
    cleanup();

    {
        HighScoreManager manager(TEST_FILE);
        manager.saveScore("Player1", 200);
    }

    HighScoreManager manager(TEST_FILE);

    int highest = manager.getHighestScore();

    assert(highest == 200);

    std::cout << "PASS: High score loaded correctly\\n";
}

void test_multiple_scores() {
    cleanup();

    HighScoreManager manager(TEST_FILE);

    manager.saveScore("A", 100);
    manager.saveScore("B", 250);
    manager.saveScore("C", 180);

    int highest = manager.getHighestScore();

    assert(highest == 250);

    std::cout << "PASS: Highest score selected correctly\\n";
}

int main() {
    std::cout << "Running High Score Tests...\\n\\n";

    test_save_highscore();
    test_load_highscore();
    test_multiple_scores();

    cleanup();

    std::cout << "\\nAll high score tests passed successfully!\\n";

    return 0;
}

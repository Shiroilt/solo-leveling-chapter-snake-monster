#include <cassert>
#include <iostream>
#include "../snake0/HighScoreManager.h"

void runHighScoreTests() {
    HighScoreManager manager;

    manager.loadHighScores();

    int before = manager.getHighestScore();

    // Add a score that should become the highest score
    manager.addHighScore("CI_TEST", 99999);
    manager.saveHighScores();

    // Reload from file
    manager.loadHighScores();

    int highest = manager.getHighestScore();

    // Verify persistence through the public API
    assert(highest == 99999);

    // Verify high score qualification logic
    assert(manager.isHighScore(99999));
    assert(manager.qualifiesAsHighScore(99999));

    std::cout << "High score tests passed\\n";
}
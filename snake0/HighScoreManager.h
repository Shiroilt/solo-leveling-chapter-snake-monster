#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>

using namespace std;

struct HighScore {
    string playerName;
    int score;
    string date;
    
    HighScore() : playerName(""), score(0), date("") {} 
    HighScore(string name, int sc, string dt) : playerName(name), score(sc), date(dt) {}
};

class HighScoreManager {
private:
    vector<HighScore> highScores;
    const int MAX_SCORES = 3;

public:
    HighScoreManager();
    void loadHighScores();
    void saveHighScores();
    void addHighScore(const string& playerName, int score);
    vector<HighScore> getHighScores();
    int getHighestScore();
    bool isHighScore(int score);
    bool qualifiesAsHighScore(int score);
};

#endif
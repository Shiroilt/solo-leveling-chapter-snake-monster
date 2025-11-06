#include "HighScoreManager.h"
#include <iostream>
#define HIGHSCORE_FILE "highscores.txt"
using namespace std;
HighScoreManager::HighScoreManager() {
    loadHighScores();
}

void HighScoreManager::loadHighScores() {
    highScores.clear();
    ifstream file(HIGHSCORE_FILE);
    if (!file.is_open()) {
        return;
    }
    string name, date;
    int score;
    while (file >> name >> score >> date) {
        replace(name.begin(), name.end(), '_', ' ');
        highScores.push_back(HighScore(name, score, date));
    }
    file.close();
    sort(highScores.begin(), highScores.end(), [](const HighScore &a, const HighScore &b){
        return a.score > b.score;
    });
}

void HighScoreManager::saveHighScores() {
    ofstream file(HIGHSCORE_FILE);
    if (file.is_open()) {
        for (const auto& hs : highScores) {
            string name = hs.playerName;
            replace(name.begin(), name.end(), ' ', '_');
            file << name << " " << hs.score << " " << hs.date << endl;
        }
        file.close();
    }
}

void HighScoreManager::addHighScore(const string& playerName, int score) {
    time_t now = time(0);
    tm* localtm = localtime(&now);
    char dateStr[11];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", localtm);

    highScores.push_back(HighScore(playerName, score, dateStr));
    sort(highScores.begin(), highScores.end(), [](const HighScore &a, const HighScore &b){
        return a.score > b.score;
    });

    if ((int)highScores.size() > MAX_SCORES) {
        highScores.resize(MAX_SCORES);
    }
    saveHighScores();
}

vector<HighScore> HighScoreManager::getHighScores() {
    return highScores;
}

int HighScoreManager::getHighestScore() {
    if (highScores.empty()) return 0;
    return highScores[0].score;
}

bool HighScoreManager::isHighScore(int score) {
    if (highScores.size() < MAX_SCORES) return true;
    return score > highScores.back().score;
}

bool HighScoreManager::qualifiesAsHighScore(int score) {
    if (highScores.empty()) return true;
    if (highScores.size() < MAX_SCORES) return true;
    return score >= highScores.back().score;
}
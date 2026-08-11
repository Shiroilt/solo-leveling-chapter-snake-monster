#include "GameManager.h"
#include <iostream>
using namespace std;
GameManager::GameManager() {}

void GameManager::showMainMenu() {
    //system("cls");
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    setColor(11);
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║           🐍 SNAKE GAME 🐍          ║\n";
    cout << "║                                      ║\n";
    cout << "║           Classic Arcade Fun!        ║\n";
    cout << "╚══════════════════════════════════════╝\n\n";
    
    setColor(7);
    cout << "1. Start New Game\n";
    cout << "2. View High Scores\n";
    cout << "3. Instructions\n";
    cout << "4. Level Progression Info\n";
    cout << "5. Exit\n\n";
    cout << "Enter your choice (1-5): ";
}

void GameManager::showLevelProgressionInfo() {
    //system("cls");
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    setColor(14);
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║         LEVEL PROGRESSION             ║\n";
    cout << "╚══════════════════════════════════════╝\n\n";
    
    setColor(7);
    cout << "🎯 HOW LEVELS WORK:\n";
    cout << "   • Start at Level 1\n";
    cout << "   • Advance to next level every 10 points\n";
    cout << "   • Speed increases with each level\n";
    cout << "   • Challenge yourself to reach higher levels!\n\n";
    
    cout << "⚡ SPEED PROGRESSION:\n";
    cout << "   • Easy:   180ms → 162ms → 144ms ...\n";
    cout << "   • Normal: 120ms → 108ms → 96ms  ...\n";
    cout << "   • Hard:    70ms →  63ms →  56ms ...\n\n";
    
    cout << "🏆 LEVEL MILESTONES:\n";
    cout << "   • Level 5:  Intermediate Snake Charmer\n";
    cout << "   • Level 10: Advanced Snake Master\n";
    cout << "   • Level 15: Legendary Snake God\n\n";
    
    setColor(10);
    cout << "Press any key to return to menu...";
    _getch();
}

void GameManager::showHighScores() {
    //system("cls");
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    setColor(14);
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║            HIGH SCORES               ║\n";
    cout << "╚══════════════════════════════════════╝\n\n";
    
    vector<HighScore> scores = highScoreManager.getHighScores();
    
    if (scores.empty()) {
        setColor(8);
        cout << "    No high scores yet!\n";
        cout << "    Play a game to set a record!\n";
    } else {
        setColor(11);
        cout << " Rank  Player           Score    Level  Date\n";
        cout << " ────────────────────────────────────────────\n";
        
        for (int i = 0; i < scores.size() && i < 10; i++) {
            setColor(7);
            
            if (i == 0) setColor(14);
            else if (i == 1) setColor(8); 
            else if (i == 2) setColor(6);
            
            cout << "  ";
            if (i == 0) cout << "🥇 ";
            else if (i == 1) cout << "🥈 ";
            else if (i == 2) cout << "🥉 ";
            else cout << (i + 1) << ".  ";
            
            string name = scores[i].playerName;
            if (name.length() > 10) {
                name = name.substr(0, 10) + "..";
            }
            cout << setw(12) << left << name;
            
            int level = (scores[i].score / 10) + 1;
            cout << setw(8) << right << scores[i].score;
            cout << setw(8) << right << level;
            
            cout << "  " << scores[i].date << endl;
        }
    }
    
    setColor(10);
    cout << "\nPress any key to return to menu...";
    _getch();
}

void GameManager::showInstructions() {
   // system("cls");
   COORD coord = {0, 0};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); 
   setColor(14);
    cout << "╔══════════════════════════════════════╗\n";
    cout << "║            INSTRUCTIONS              ║\n";
    cout << "╚══════════════════════════════════════╝\n\n";
    
    setColor(7);
    cout << "🎯 OBJECTIVE:\n";
    cout << "   • Guide the snake to eat food (●)\n";
    cout << "   • Grow longer without hitting walls or yourself\n";
    cout << "   • Reach higher levels by scoring points\n\n";
    
    cout << "🎮 CONTROLS:\n";
    cout << "   • W or ↑ - Move Up\n";
    cout << "   • A or ← - Move Left\n";
    cout << "   • S or ↓ - Move Down\n";
    cout << "   • D or → - Move Right\n";
    cout << "   • P - Pause Game\n\n";
    
    cout << "⚡ GAME RULES:\n";
    cout << "   • Each food gives +1 point\n";
    cout << "   • Snake grows longer when eating\n";
    cout << "   • Game ends if you hit walls or yourself\n";
    cout << "   • Level up every 10 points (speed increases)\n\n";
    
    cout << "🏆 SCORING:\n";
    cout << "   • Try to beat the high score!\n";
    cout << "   • Top 10 scores are saved\n";
    cout << "   • Higher levels = higher prestige!\n\n";
    
    setColor(10);
    cout << "Press any key to return to menu...";
    _getch();
}

void GameManager::startGame() {
    cout << "\nEnter your name: ";
    cin >> playerName;
    
    if (playerName.empty()) {
        playerName = "Player";
    }
    
    int modeChoice;
    cout << "\n🎮 SELECT DIFFICULTY:\n";
    cout << "1. Easy (Slow) - Good for beginners\n";
    cout << "2. Normal (Medium) - Balanced gameplay\n";
    cout << "3. Hard (Fast) - For snake masters!\n";
    cout << "Enter choice (1-3): ";
    cin >> modeChoice;
    
    if (modeChoice < 1 || modeChoice > 3) {
        cout << "Invalid choice! Defaulting to Normal mode.\n";
        modeChoice = 2;
    }
    
    GameBoard* board = new GameBoard(&highScoreManager, modeChoice, playerName);
    board->spawnFood();
    
    // system("cls");
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << "\n\n   Starting at Level 1!\n";
    cout << "   Reach Level 10 to become a Snake Master!\n";
    cout << "   Current High Score: " << highScoreManager.getHighestScore() << "\n";
    cout << "   Good luck, " << playerName << "!\n\n";
    cout << "   Press any key to start...";
    _getch();
    system("cls");
    
    while (board->update()) {
        board->getInput();
        board->draw();
        Sleep(board->getSpeed());
    }
    
    board->displayGameOver();
    delete board;
}

void GameManager::run() {
    hideCursor();
    
    while (true) {
        showMainMenu();
        
        char choice;
        cin >> choice;
        
        switch (choice) {
            case '1':
                startGame();
                break;
            case '2':
                showHighScores();
                break;
            case '3':
                showInstructions();
                break;
            case '4':
                showLevelProgressionInfo();
                break;
            case '5':
                cout << "\nThanks for playing! Goodbye 👋\n";
                return;
            default:
                cout << "Invalid choice! Press any key to continue...";
                _getch();
        }
    }
}
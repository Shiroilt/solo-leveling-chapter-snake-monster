# 🐍 Snake Game in C++

<div align="center">

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)
![License](https://img.shields.io/badge/License-MIT-green.svg)
![Version](https://img.shields.io/badge/Version-1.0.0-orange.svg)

**A classic Snake game implementation in C++ with modern features and modular architecture**

![Gameplay](https://img.shields.io/badge/Gameplay-Classic_Snake-brightgreen)
![Status](https://img.shields.io/badge/Status-Completed-success)

</div>

## 📖 Introduction

Welcome to my first major C++ project! This Snake Game represents my journey into programming and showcases my understanding of object-oriented programming, file handling, game development concepts, and code organization. As a beginner programmer, I've put significant effort into creating a feature-rich, well-structured game that demonstrates good programming practices.

## 🎮 Features

### 🕹️ Core Gameplay
- **Classic Snake Mechanics**: Guide the snake to eat food and grow longer
- **Multiple Difficulty Levels**: Easy, Normal, and Hard modes
- **Progressive Speed**: Game speeds up as you advance through levels
- **Collision Detection**: Wall and self-collision detection
- **Smooth Controls**: Responsive keyboard controls with both WASD and Arrow key support

### 🏆 Scoring & Progression
- **Level System**: Advance through levels every 10 points
- **High Score Tracking**: Persistent high score storage
- **Performance Metrics**: Detailed end-game statistics
- **Achievement Messages**: Encouraging feedback based on performance

### 💾 Data Management
- **File-based Storage**: High scores saved to `highscores.txt`
- **Player Profiles**: Track scores with player names
- **Date Tracking**: Record when high scores were achieved

### 🎨 User Interface
- **Colorful Display**: Vibrant console colors and symbols
- **Visual Feedback**: Animated level-up messages
- **Comprehensive Menus**: Intuitive navigation system
- **Real-time Stats**: Live score, level, and speed display

## 🏗️ Project Structure
snake_game/
├── main.cpp # Program entry point

├── Common.h # Shared constants and utilities

├── Common.cpp # Utility function implementations

├── HighScoreManager.h # High score management interface

├── HighScoreManager.cpp # High score logic implementation

├── Food.h # Food class interface

├── Food.cpp # Food behavior implementation

├── Snake.h # Snake class interface

├── Snake.cpp # Snake movement and rendering

├── GameBoard.h # Game board management interface

├── GameBoard.cpp # Game state and rendering logic

├── GameManager.h # Main game flow interface

├── GameManager.cpp # Menu and game flow implementation

├── highscores.txt # High score data storage

└── README.md # Project documentation


## 🚀 Getting Started

### Prerequisites
- **Windows OS** (uses Windows API for console manipulation)
- **C++ Compiler** (GCC, MinGW, or Visual Studio)
- **C++17** compatible compiler

### Installation & Compilation

#### Method 1: Manual Compilation
```bash
# Using g++
g++ -o snake_game main.cpp Common.cpp HighScoreManager.cpp Food.cpp Snake.cpp GameBoard.cpp GameManager.cpp

# Run the game
./snake_game.exe

Method 2: Visual Studio
Create a new C++ Console Project

Add all .cpp and .h files to the project

Build and Run



🎯 How to Play
Start the Game: Run the executable

Enter Your Name: Create your player profile

Choose Difficulty:

🟢 Easy: Slow speed, great for beginners

🟡 Normal: Balanced gameplay

🔴 Hard: Fast-paced challenge for experts

Controls:

W or ↑ - Move Up

A or ← - Move Left

S or ↓ - Move Down

D or → - Move Right

P - Pause Game

Objective: Eat food (●) to grow and score points without hitting walls or yourself!

🛠️ Technical Implementation
🔧 Key Components
GameManager Class
Handles main menu navigation

Manages game flow and transitions

Displays instructions and high scores

GameBoard Class
Manages game state and rendering

Handles collision detection

Controls game speed and level progression

Snake Class
Implements snake movement logic

Manages body segments and growth

Handles direction changes and collision

Food Class
Generates food at valid positions

Manages food spawning and consumption

HighScoreManager Class
Handles file I/O for high scores

Manages score sorting and validation

Provides score persistence

💡 Programming Concepts Demonstrated
Object-Oriented Programming: Clean class hierarchy and encapsulation

File Handling: Persistent data storage and retrieval

Memory Management: Proper resource allocation and deallocation

Modular Design: Separated concerns and reusable components

User Input Handling: Responsive keyboard controls

Console Manipulation: Cross-platform compatible UI rendering

🎮 Game Screenshots
Main Menu
╔══════════════════════════════════════╗
║           🐍 SNAKE GAME 🐍           ║
║                                      ║
║          Classic Arcade Fun!         ║
╚══════════════════════════════════════╝

1. Start New Game
2. View High Scores
3. Instructions
4. Level Progression Info
5. Exit

High Scores Display

╔══════════════════════════════════════╗
║            HIGH SCORES               ║
╚══════════════════════════════════════╝

 Rank  Player           Score    Level  Date
 ────────────────────────────────────────────
  🥇   Player1           45       5      2024-01-15
  🥈   SnakeMaster       32       4      2024-01-14
  🥉   Beginner          18       2      2024-01-13

  🤝 Collaboration Welcome!
As a beginner programmer, I'm excited to collaborate and learn from others! This project is open for:

🎯 Areas for Improvement
Code Review: Suggestions for better practices

Feature Additions: New gameplay elements

Performance Optimization: More efficient algorithms

Cross-Platform Support: Linux/Mac compatibility

Code Refactoring: Better organization and patterns


---------------------------------------------------------------------------------------------------------------------------
👥 How to Contribute
Fork the repository

Create a feature branch (git checkout -b feature/AmazingFeature)

Commit your changes (git commit -m 'Add some AmazingFeature')

Push to the branch (git push origin feature/AmazingFeature)

Open a Pull Request

🐛 Reporting Issues
Found a bug or have a suggestion? Open an issue with:

Detailed description of the problem

Steps to reproduce

Expected vs actual behavior

Screenshots if applicable



📊 Project Stats
Lines of Code: 1,200+

Classes: 6 main classes

Files: 14+ source files

Features: 15+ gameplay features

Development Time: 2+ weeks of dedicated learning

🎓 Learning Outcomes
Through this project, I've gained practical experience in:

C++ Syntax and Semantics: Deep understanding of C++ features

OOP Principles: Encapsulation, inheritance, polymorphism

Game Development: Game loops, collision detection, state management

Software Architecture: Modular design and separation of concerns

Debugging Skills: Identifying and fixing complex issues

Version Control: Proper Git usage and commit practices

🔮 Future Enhancements
Power-ups and special food types

Different snake skins and themes

Sound effects and background music

Multiplayer mode

Save/load game functionality

Graphical interface using SFML or SDL

📜 License
This project is licensed under the MIT License - see the LICENSE file for details.

🙏 Acknowledgments
Inspired by classic Nokia Snake game

Thanks to the C++ community for excellent learning resources

Special thanks to all who provided code reviews and suggestions

Built with passion for learning programming fundamentals

<div align="center">
"Every expert was once a beginner. This game represents my first steps into the world of programming."
⭐ If you like this project, please give it a star! ⭐


Happy Coding! 🎯




</div>
📞 Contact
Developer: Shashwat Soni

Project: Snake Game C++

Experience Level: Beginner Programmer

Goal: Learn and collaborate with the programming community

This README was generated with love and attention to detail. Feel free to customize it further!

# Makefile for Solo Leveling Snake Game CI Testing

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I./snake0

# Project source files

SRC = 
snake0/Snake.cpp 
snake0/Food.cpp 
snake0/GameBoard.cpp 
snake0/HighScoreManager.cpp 
snake0/Common.cpp

# Test source files

TEST_SRC = 
tests/run_all_tests.cpp 
tests/test_snake_initialization.cpp 
tests/test_snake_movement.cpp 
tests/test_snake_growth.cpp 
tests/test_snake_collision.cpp 
tests/test_food.cpp 
tests/test_highscore.cpp 
tests/test_gameboard.cpp 
tests/test_integration.cpp

TARGET = run_tests

all: test

$(TARGET): $(SRC) $(TEST_SRC)
$(CXX) $(CXXFLAGS) $(SRC) $(TEST_SRC) -o $(TARGET)

test: $(TARGET)
./$(TARGET)

clean:
rm -f $(TARGET) test_highscore.dat

.PHONY: all test clean
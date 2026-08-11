#include "Food.h"
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;
Food::Food() : active(false) {}

Point Food::getPosition() { return position; }
bool Food::isActive() { return active; }

void Food::spawn(const vector<Point>& snake1Body, const vector<Point>& snake2Body, int maxX, int maxY) {
    vector<Point> validPositions;
    
    for (int x = 2; x < maxX - 2; x++) {
        for (int y = 2; y < maxY - 2; y++) {
            Point candidate(x, y);
            bool occupied = false;
            
            for (const Point& segment : snake1Body) {
                if (candidate == segment) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                for (const Point& segment : snake2Body) {
                    if (candidate == segment) {
                        occupied = true;
                        break;
                    }
                }
            }
            
            if (!occupied) {
                validPositions.push_back(candidate);
            }
        }
    }
    
    if (!validPositions.empty()) {
        position = validPositions[rand() % validPositions.size()];
        active = true;
    }
}

void Food::deactivate() {
    active = false;
}

void Food::draw() {
    if (active) {
        gotoxy(position.xCoord, position.yCoord);
        setColor(12);
        cout << "●";
    }
}
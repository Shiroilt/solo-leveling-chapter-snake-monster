#ifndef COMMON_H
#define COMMON_H

#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    inline void Sleep(unsigned int ms) {
        usleep(ms * 1000);
    }
#endif

#include <iostream>
#include <vector>
#include <string>

using namespace std;

const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

extern int consoleWidth, consoleHeight;

struct Point {
    int xCoord, yCoord;
    Point() : xCoord(0), yCoord(0) {}
    Point(int x, int y) : xCoord(x), yCoord(y) {}

    bool operator==(const Point& other) const {
        return xCoord == other.xCoord && yCoord == other.yCoord;
    }
};

// Utility functions
void setColor(int color);
void hideCursor();
void initScreen();
void gotoxy(int x, int y);

#endif
#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <iostream>
#pragma comment(lib, "Winmm.lib")

#include "Common.h"
#include "GameManager.h"

using namespace std;

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    
    GameManager game;
    game.run();
    
    return 0;
}
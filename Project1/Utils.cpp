#pragma once
#include "Utils.h"
#include "Game.h"  // for Game::WIDTH / HEIGHT
#include <windows.h>
#include <iostream>
#include <map>

 bool g_colorMode = true;

 void setGlobalColorMode(bool enable) {
     g_colorMode = enable;
 }

 bool getGlobalColorMode() {
     return g_colorMode;
 }

 void gotoxy(int x, int y) {
     std::cout.flush();
     COORD coord;
     coord.X = x;
     coord.Y = y;
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
 }

 void hideCursor() {
     HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_CURSOR_INFO cursorInfo;
     GetConsoleCursorInfo(hConsole, &cursorInfo);
     cursorInfo.bVisible = FALSE;
     SetConsoleCursorInfo(hConsole, &cursorInfo);
 }

 void resetColor() {
     if (!g_colorMode) return;

     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
         FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
 }

 void setColorByName(const std::string& name) {
     if (!g_colorMode) return;

     static const std::map<std::string, int> colorMap = {
         {"black", 0},
         {"blue", FOREGROUND_BLUE},
         {"green", FOREGROUND_GREEN},
         {"red", FOREGROUND_RED},
         {"cyan", FOREGROUND_GREEN | FOREGROUND_BLUE},
         {"magenta", FOREGROUND_RED | FOREGROUND_BLUE},
         {"yellow", FOREGROUND_RED | FOREGROUND_GREEN},
         {"white", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE},
         {"grey", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE},
         {"bright blue", FOREGROUND_BLUE | FOREGROUND_INTENSITY},
         {"bright green", FOREGROUND_GREEN | FOREGROUND_INTENSITY},
         {"bright red", FOREGROUND_RED | FOREGROUND_INTENSITY},
         {"bright cyan", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
         {"bright magenta", FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
         {"bright yellow", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY},
         {"bright white", FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY},
     };

     auto it = colorMap.find(name);
     if (it != colorMap.end()) {
         SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), it->second);
     }
     else {
         std::cerr << "Unknown color: " << name << std::endl;
     }
 }

 void wrapCoordinates(int& x, int& y) {
     if (x < 0) x = Game::WIDTH - 1;
     else if (x >= Game::WIDTH) x = 0;

     if (y < 0) y = Game::HEIGHT - 1;
     else if (y >= Game::HEIGHT) y = 0;
 }

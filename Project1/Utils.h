#pragma once
#include <windows.h>
#include <string>
#include <iostream>
#include <map>

extern bool g_colorMode;

void gotoxy(int x, int y);

void hideCursor();

void resetColor();

void setColorByName(const std::string& name);

void wrapCoordinates(int& x, int& y);

void setGlobalColorMode(bool enable);
bool getGlobalColorMode();

namespace Direction {
    enum Type { U, UR, R, DR, D, DL, L, UL };
}

#pragma once

#include <windows.h>
#include <string>
#include <iostream>
#include <map>

void gotoxy(int x, int y);

void hideCursor();

void resetColor();

void setColorByName(const std::string& name);

namespace Direction {
    enum Type { U, UR, R, DR, D, DL, L, UL };
}
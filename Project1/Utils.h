#pragma once

#include <windows.h>

// Moves the cursor to position (x, y) in the console
void gotoxy(int x, int y);

// Hides the cursor (optional, looks nicer)
void hideCursor();

// Optional: Set text color
void setColor(int color);

// Optional: Reset text color
void resetColor();

namespace Direction {
    enum Type { U, UR, R, DR, D, DL, L, UL };
}
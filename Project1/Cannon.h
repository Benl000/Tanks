#pragma once
#include "Utils.h"
#include <vector>

class Tank;  // Forward declare Tank

class Cannon
{
private:
    int x, y;
    enum Condition { FIXED, BROKEN };
    Condition currCondition = FIXED;
    char symbol = '|';
    Tank* tank;

public:
    Cannon(Tank* t);
    int getX() const;
    int getY() const;
    void update();
    void render();
    std::vector<int> nextXY(Direction::Type d);
private:
    void setCannonLocation();
    void setDirectionSymbol();
};

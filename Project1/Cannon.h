#pragma once
#include "Utils.h"
#include <vector>

class Tank;  // Forward declare Tank

class Cannon
{
public:
    enum Condition { FIXED, BROKEN };
private:
    int x, y;
    Condition currCondition = FIXED;
    char symbol = '|';
    Tank* tank;

public:
    Cannon(Tank* t);
    int getX() const;
    int getY() const;
    void update();
    void render();
    void setCondition(Condition c);
    Condition getCondition();
    std::vector<int> nextXY(Direction::Type d);
private:
    void setCannonLocation();
    void setDirectionSymbol();
};

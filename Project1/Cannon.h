#pragma once
#include "Utils.h"

class Tank;  // Forward declare Tank

class Cannon
{
private:
    int x, y,prevX, prevY;
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
private:
    void setCannonLocation();
    void setDirectionSymbol();

};

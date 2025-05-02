#pragma once
#include <vector>
#include "Tank.h"
#include <string>
using namespace std;

class Player
{
    struct ControlKeys {
        char leftForward;
        char leftBackward;
        char rightForward;
        char rightBackward;
        char stopBoth;
    };

private:
    ControlKeys controls;
    vector<Tank> tanks;
    string color = "blue";
    int activeTankIndex = 0; // Index of currently controlled tank

public:
    void addTank(const Tank& tank);
    Tank* getActiveTank();
    void renderAllTanks();
    void removeTank(int index);
    void setControls(ControlKeys keys);
    void handleInput(char key);
    void setColor(const string& c);
    string getColor();
};

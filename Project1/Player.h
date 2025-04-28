#pragma once
#include <vector>
#include "Tank.h"
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
    int activeTankIndex = 0; // Index of currently controlled tank

public:
    void addTank(const Tank& tank);
    Tank* getActiveTank();
    void renderAllTanks();
    void switchToNextTank();
    void removeTank(int index);
    bool hasTanks() const;
    void setControls(ControlKeys keys);
    void handleInput(char key);

};

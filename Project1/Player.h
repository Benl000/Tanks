#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Tank.h"
using std::unique_ptr;
using std::string;

class Player {
    struct ControlKeys {
        char leftForward;
        char leftBackward;
        char rightForward;
        char rightBackward;
        char stopBoth;
        char shoot;
    };

private:
    ControlKeys controls;
    vector<unique_ptr<Tank>> tanks;  // ? now using unique_ptr
    string color = "blue";
    int activeTankIndex = 0;
    int score = 0;

public:
    void addTank(unique_ptr<Tank> tank);
    Tank* getActiveTank();
    vector<unique_ptr<Tank>>& getTanks();
    void renderAllTanks();
    void removeTank(int index);
    void setControls(ControlKeys keys);
    void setColor(const string& c);
    void updateScore(int change);
    string getColor();
    void handleInput(char key, std::vector<Shell>& gameShells);

};

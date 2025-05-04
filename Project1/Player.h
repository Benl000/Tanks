#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Tank.h"

class Player {
    struct ControlKeys {
        char leftForward;
        char leftBackward;
        char rightForward;
        char rightBackward;
        char stopBoth;
    };

private:
    ControlKeys controls;
    std::vector<std::unique_ptr<Tank>> tanks;  // ? now using unique_ptr
    std::string color = "blue";
    int activeTankIndex = 0;

public:
    void addTank(std::unique_ptr<Tank> tank);
    Tank* getActiveTank();
    std::vector<std::unique_ptr<Tank>>& getTanks();
    void renderAllTanks();
    void removeTank(int index);
    void setControls(ControlKeys keys);
    void handleInput(char key);
    void setColor(const std::string& c);
    std::string getColor();
};

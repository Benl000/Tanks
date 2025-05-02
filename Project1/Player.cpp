#include "Player.h"
#include "Utils.h"

using namespace std;

void Player::addTank(const Tank& tank) {
    tanks.push_back(tank);
}

Tank* Player::getActiveTank() {
    if (tanks.empty()) return nullptr;
    return &tanks[activeTankIndex];
}

void Player::renderAllTanks()
{
    setColorByName(color);
    for (Tank& tank : tanks) {
        tank.render();
    }
    resetColor();
}

void Player::removeTank(int index) {
    if (index >= 0 && index < tanks.size()) {
        tanks.erase(tanks.begin() + index);
        if (activeTankIndex >= tanks.size()) {
            activeTankIndex = 0; // Reset if needed
        }
    }
}

void Player::setControls(ControlKeys keys) {
    controls = keys;
}

void Player::handleInput(char key) {
    Tank* tank = getActiveTank();
    if (!tank)
        return;

    if (key == controls.leftForward) {
        tank->setLeftTrack(tank->getLeftTrack() == Tank::FORWARD ? Tank::STOPPED : Tank::FORWARD);
    }
    else if (key == controls.leftBackward) {
        tank->setLeftTrack(tank->getLeftTrack() == Tank::BACKWARD ? Tank::STOPPED : Tank::BACKWARD);
    }
    else if (key == controls.rightForward) {
        tank->setRightTrack(tank->getRightTrack() == Tank::FORWARD ? Tank::STOPPED : Tank::FORWARD);
    }
    else if (key == controls.rightBackward) {
        tank->setRightTrack(tank->getRightTrack() == Tank::BACKWARD ? Tank::STOPPED : Tank::BACKWARD);
    }
    else if (key == controls.stopBoth) {
        tank->setLeftTrack(Tank::STOPPED);
        tank->setRightTrack(Tank::STOPPED);
    }
}

void Player::setPlayerColor(const string& c)
{
    color = c;
}

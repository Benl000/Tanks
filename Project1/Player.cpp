#include "Player.h"

void Player::addTank(const Tank& tank) {
    tanks.push_back(tank);
}

Tank* Player::getActiveTank() {
    if (tanks.empty()) return nullptr;
    return &tanks[activeTankIndex];
}

void Player::renderAllTanks()
{
    for (Tank& tank : tanks) {
        tank.render();
    }
}

void Player::switchToNextTank() {
    if (!tanks.empty()) {
        activeTankIndex = (activeTankIndex + 1) % tanks.size();
    }
}

void Player::removeTank(int index) {
    if (index >= 0 && index < tanks.size()) {
        tanks.erase(tanks.begin() + index);
        if (activeTankIndex >= tanks.size()) {
            activeTankIndex = 0; // Reset if needed
        }
    }
}

bool Player::hasTanks() const {
    return !tanks.empty();
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


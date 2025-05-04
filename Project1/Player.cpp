#include "Player.h"
#include <iostream>
using namespace std;

void Player::addTank(std::unique_ptr<Tank> tank) {
    tanks.push_back(std::move(tank));
}

Tank* Player::getActiveTank() {
    if (tanks.empty()) return nullptr;
    return tanks[activeTankIndex].get();
}

std::vector<std::unique_ptr<Tank>>& Player::getTanks() {
    return tanks;
}

void Player::renderAllTanks() {
    for (auto& tank : tanks) {
        tank->render();
    }
}

void Player::removeTank(int index) {
    if (index >= 0 && index < tanks.size()) {
        tanks.erase(tanks.begin() + index);
        if (activeTankIndex >= tanks.size())
            activeTankIndex = 0;
    }
}

void Player::setControls(ControlKeys keys) {
    controls = keys;
}

void Player::setColor(const std::string& c) {
    color = c;
}

std::string Player::getColor() {
    return color;
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

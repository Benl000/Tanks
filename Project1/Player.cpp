#include "Player.h"
#include <iostream>
#include "Shell.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::unique_ptr;

void Player::addTank(unique_ptr<Tank> tank) {
    tanks.push_back(move(tank));
}

bool Player::hasTanks() {
    if (tanks.empty()) return false;
    return true;
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

Player::ControlKeys& Player::getControls()
{
    return controls;
}


void Player::setColor(const std::string& c) {
    color = c;
}

void Player::updateScore(int change)
{
    score += change;
}

std::string Player::getColor() {
    return color;
}

int Player::getScore()
{
    return score;
}

void Player::handleInput(char key, std::vector<Shell>& gameShells, int playerID) {
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
    else if (key == controls.shoot) {
        tank->shoot(gameShells, playerID);
    }
    else if (key == controls.switchActiveTank) {
        activeTankIndex = (activeTankIndex + 1) % tanks.size() ;
    }
}

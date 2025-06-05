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
    if (tanks.empty() || activeTankIndex < 0 || activeTankIndex >= tanks.size())
        return nullptr;
    return tanks[activeTankIndex].get();
}


int Player::getActiveTankIndex() const {
    return activeTankIndex;
}

void Player::setActiveTankIndex(int index) {
    if (index >= 0 && index < tanks.size())
        activeTankIndex = index;
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
    tanks.erase(tanks.begin() + index);
    if (tanks.empty()) {
        activeTankIndex = -1;
    }
    else {
        activeTankIndex %= tanks.size();
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

void Player::handleInput(char key, std::vector<Shell>& gameShells, int playerID, int currentGameTime, GameRecorder& recorder) {
    Tank* tank = getActiveTank();
    int tankIndex = getActiveTankIndex();
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
        tank->shoot(gameShells, playerID, tankIndex, currentGameTime, &recorder);
    }
    else if (key == controls.switchActiveTank) {
        switchToNextTank();
    }
}

void Player::switchToNextTank() {
    if (tanks.size() <= 1)
        return;

    tanks[activeTankIndex]->setActive(false);
    tanks[activeTankIndex]->render(); // Redraw as non-active

    activeTankIndex = (activeTankIndex + 1) % tanks.size();

    tanks[activeTankIndex]->setActive(true);
    tanks[activeTankIndex]->render(); // Redraw as active
}

#include "Cannon.h"
#include "Tank.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Cannon::Cannon(Tank* t) : tank(t) {
    update();
}

int Cannon::getX() const { return x; }
int Cannon::getY() const { return y; }

void Cannon::update() {
    setCannonLocation();
    setDirectionSymbol();
}


void Cannon::render() {
    // Draw new
    gotoxy(x, y);
    cout << symbol;
}

void Cannon::setCannonLocation() {
    int tx = tank->getX();
    int ty = tank->getY();

    prevX = x;
    prevY = y;

    switch (tank->getDirection()) {
    case Direction::U:  x = tx;     y = ty - 1; break;
    case Direction::UR: x = tx + 1; y = ty - 1; break;
    case Direction::R:  x = tx + 1; y = ty;     break;
    case Direction::DR: x = tx + 1; y = ty + 1; break;
    case Direction::D:  x = tx;     y = ty + 1; break;
    case Direction::DL: x = tx - 1; y = ty + 1; break;
    case Direction::L:  x = tx - 1; y = ty;     break;
    case Direction::UL: x = tx - 1; y = ty - 1; break;
    }
    wrapCoordinates(x, y);
}


void Cannon::setDirectionSymbol() {
    // Correct: match cannon symbol according to direction
    switch (tank->getDirection()) {
    case Direction::U:
    case Direction::D:
        symbol = '|';
        break;
    case Direction::L:
    case Direction::R:
        symbol = '-';
        break;
    case Direction::UL:
    case Direction::DR:
        symbol = '\\';
        break;
    case Direction::UR:
    case Direction::DL:
        symbol = '/';
        break;
    }
}


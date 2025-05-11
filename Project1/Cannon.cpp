#include "Cannon.h"
#include "Tank.h"
#include "Utils.h"
#include <vector>
#include <iostream>
using std::cout;
using std::endl;
using std::string;
using std::vector;

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
    gotoxy(x, y);
    cout << symbol;
}
void Cannon::setCondition(Condition c)
{
    currCondition = c;
}

Cannon::Condition Cannon::getCondition()
{
    return currCondition;
}

vector<int> Cannon::nextXY(Direction::Type d)  {
    int tx = tank->getX();
    int ty = tank->getY();
    Direction::Type dir = d;

    switch (dir) {
    case Direction::U:  --ty; break;
    case Direction::UR: ++tx; --ty; break;
    case Direction::R:  ++tx;     break;
    case Direction::DR: ++tx;++ty ; break;
    case Direction::D: ++ty; break;
    case Direction::DL: --tx; ++ty; break;
    case Direction::L:  --tx;   break;
    case Direction::UL: --tx; --ty; break;
    }
    std::vector<int> newPosition;
    newPosition.push_back(tx); // First element is the new X
    newPosition.push_back(ty); // Second element is the new Y
    return newPosition;
}

void Cannon::setCannonLocation() {
    int tx = tank->getX();
    int ty = tank->getY();
    Direction::Type dir = tank->getDirection();

    switch (dir) {
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
    Direction::Type dir = tank->getDirection();

    switch (dir) {
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



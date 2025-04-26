#include "Cannon.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Cannon::Cannon(Tank* t) : tank(t) {
    update();
}
int Cannon::getX() const { return x; }

int Cannon::getY() const { return y; }

void Cannon::update()
{
	getCannonLocation(x, y); 
	getDirectionSymbol();
}

void Cannon::getCannonLocation(int& x, int& y)
{
    int tx = tank->getX();
    int ty = tank->getY();

    // Move cannon position one unit in the direction the tank is facing
    switch (tank->getDirection()) {
    case Direction::U:        ty--; break;
    case Direction::D:      ty++; break;
    case Direction::L:     tx--; break;
    case Direction::R:     tx++; break;
    case Direction::UL:   tx--; ty--; break;
    case Direction::UR:  tx++; ty--; break;
    case Direction::DL: tx--; ty++; break;
    case Direction::DR: tx++; ty++; break;
    }

    // Output the computed cannon coordinates
    x = tx;
    y = ty;
}

void Cannon::getDirectionSymbol()
{
    switch (tank->getDirection()) {
    case Direction::U:
    case Direction::D:
        symbol = '|'; break;
    case Direction::L:
    case Direction::R:
        symbol = '-'; break;
    case Direction::UL:
    case Direction::DR:
        symbol = '\\'; break;
    case Direction::UR:
    case Direction::DL:
        symbol = '/'; break;
    }
}

void Cannon::render() {
    update();
    gotoxy(x, y);
    cout << symbol;  // Restore mine if cannon was over it
}

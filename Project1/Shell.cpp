#include "Shell.h"
#include "Utils.h"
#include <iostream>
using std::cout;
using std::endl;
using std::string;

Shell::Shell(int x, int y, Direction::Type d, int shooterID) : x(x), y(y), dir(d), shooterID(shooterID) {}

int Shell::getX() const { return x; }

int Shell::getY() const { return y; }

void Shell::move()
{
    // Move shell in the direction
    switch (dir) {
    case Direction::U: y -= 1; break;
    case Direction::D: y += 1; break;
    case Direction::L: x -= 1; break;
    case Direction::R: x += 1; break;
    case Direction::UL: x -= 1; y -= 1; break;
    case Direction::UR: x += 1; y -= 1; break;
    case Direction::DL: x -= 1; y += 1; break;
    case Direction::DR: x += 1; y += 1; break;
    }

    // Wrap coordinates using the utility function
    wrapCoordinates(x, y);
}


void Shell::render() const
{
	gotoxy(x, y);
	cout << symbol;
}

char Shell::getSymbol() const
{
	return symbol;
}

void Shell::setprevStatus(bool b)
{
    prevEmpty = b;
}

bool Shell::isPrevEmpty()
{
    return prevEmpty;
}

Direction::Type Shell::getDirection() const {
	return dir;
}

int Shell::getShooterID() const { return shooterID; }
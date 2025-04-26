#include "Tank.h"
#include "Utils.h"

#include <iostream>
using namespace std;

Tank::Tank(int x, int y, Direction::Type direction) : x(x), y(y), direction(direction), cannon(this) {}

int Tank::getX() const { return x; }

int Tank::getY() const { return y; }

void Tank::render() const
{
	gotoxy(x, y);
	cout << symbol;

	cannon.render();
}

Direction::Type Tank::getDirection() const
{
	return direction;
}

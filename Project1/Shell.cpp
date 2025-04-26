#include "Shell.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Shell::Shell(int x, int y, Direction::Type d) : x(x), y(y), dir(d) {}

int Shell::getX() const { return x; }

int Shell::getY() const { return y; }

void Shell::render() const
{
	gotoxy(x, y);
	cout << symbol;
}

void Shell::setOverMine(bool b)
{
	isOverMine = b;
}

bool Shell::getOverMine()
{
	return isOverMine;
}

Direction::Type Shell::getDirection() {
	return dir;
}


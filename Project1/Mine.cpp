#include "Mine.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Mine::Mine(int x, int y) : x(x), y(y) {}

int Mine::getX() const {
    return x;
}

int Mine::getY() const {
    return y;
}

char Mine::getSymbol() const
{
	return symbol;
}

void Mine::render() const
{
    setColorByName(color);
    gotoxy(x, y);
	cout << symbol;
    resetColor();
	
}
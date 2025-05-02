#include "Wall.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Wall::Wall(int x, int y) : x(x), y(y), currCondition(FIXED) {}

int Wall::getX() const
{
    return x;
}

int Wall::getY() const 
{
    return y;
}

void Wall::render() const
{
	gotoxy(x, y);
	setWallColor();
	cout << getSymbol();
	resetColor();
}

void Wall::setWallColor() const
{
	switch (currCondition)
	{
	case Wall::FIXED:
		setColorByName(fixedColor);
		break;
	case Wall::BROKEN:
		setColorByName(brokenColor);
		break;
	default:
		setColorByName(fixedColor);
		break;
	}
}

char Wall::getSymbol() const
{
	switch (currCondition)
	{
	case Wall::FIXED:
		return FixedSymbol;
		break;
	case Wall::BROKEN:
		return brokenSymbol;
		break;
	default:
		return FixedSymbol;
		break;
	}
}

void Wall::setCondition()
{
	switch (currCondition)
	{
	case Wall::FIXED:
		currCondition = BROKEN;
		break;
	case Wall::BROKEN:
		currCondition = GONE;
		break;
	default:
		currCondition = BROKEN;
		break;
	}
}

bool Wall::gotShoot() {
	setCondition();
	render();
	return currCondition == GONE;
}


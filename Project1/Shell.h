#pragma once
#include "Utils.h"

class Shell
{
private:
	int x, y;
	Direction::Type dir;
	char symbol = '*';
	bool prevEmpty = false;

public:
	Shell(int x, int y, Direction::Type d);
	int getX() const;
	int getY() const;
	void move();
	void render() const;
	void setOverMine(bool b);
	char getSymbol() const;
	void setprevStatus(bool b);
	bool isPrevEmpty();

	Direction::Type getDirection();
};

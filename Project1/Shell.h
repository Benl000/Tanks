#pragma once
#include "Utils.h"

class Shell
{
private:
	int x, y;
	Direction::Type dir;
	char symbol = '*';
	bool isOverMine = false;
public:
	Shell(int x, int y, Direction::Type d);
	int getX() const;
	int getY() const;
	void render() const;
	void setOverMine(bool b);
	char getSymbol() const;
	bool getOverMine();
	Direction::Type getDirection();
};

#pragma once
#include "Utils.h"

class Shell
{
private:
	int x, y;
	Direction::Type dir;
	char symbol = '*';
	bool prevEmpty = false;
	int shooterID;

public:
	Shell(int x, int y, Direction::Type d, int shooterID);
	int getX() const;
	int getY() const;
	void move();
	void render() const;
	char getSymbol() const;
	void setprevStatus(bool b);
	bool isPrevEmpty();
	int getShooterID() const;
	Direction::Type getDirection();
};

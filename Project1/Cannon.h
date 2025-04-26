#pragma once
#include "Tank.h"
class Cannon
{
private:
	int x, y;
	enum Condition {FIXED,BROKEN};
	Condition currCondition = FIXED;
	char symbol = '|';
	Tank* tank;
public:
	Cannon(Tank* t);
	int getX() const;
	int getY() const;
	void update();
	void getCannonLocation(int& x, int& y);
	void getDirectionSymbol();
	void render();
	
};


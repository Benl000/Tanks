#pragma once
#include <string>
using std::string;

class Wall
{
private:
	int x, y;
	enum conditions {FIXED,BROKEN,GONE};
	conditions currCondition = FIXED;
	char FixedSymbol = '#';
	char brokenSymbol = '=';
	string fixedColor = "white";
	string brokenColor = "grey";

public:
	Wall(int x, int y);
	int getX() const;
	int getY() const;
	void render() const;
	void setColor() const;
	char getSymbol() const;
	void setCondition();
	bool gotShoot() ;
	bool isBroken() const;
};


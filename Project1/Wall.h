#pragma once
#include <string>
	using namespace std;

class Wall
{
private:
	int x, y;
	enum conditions {FIXED,BROKEN,GONE};
	conditions currCondition = FIXED;
	char FixedSymbol = '#';
	char brokenSymbol = '=';
	string fixedColor = "white";
	string brokenColor = "white";

public:
	Wall(int x, int y);
	int getX() const;
	int getY() const;
	void render() const;
	void setWallColor() const;
	char getSymbol() const;
	void setCondition();
	bool gotShoot() ;
};


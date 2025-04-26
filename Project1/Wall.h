#pragma once
class Wall
{
private:
	int x, y;
	enum conditions {FIXED,BROKEN,GONE};
	conditions currCondition = FIXED;
	char FixedSymbol = '#';
	char brokenSymbol = '=';
public:
	Wall(int x, int y);
	int getX() const;
	int getY() const;
	void render() const;
	char getSymbol() const;
	void setCondition();
	bool gotShoot() ;
};


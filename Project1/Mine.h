#pragma once

#include <string>
using std::string;

class Mine
{
private:
	int x, y;
	char symbol = '@';
	string color = "yellow";
public:
	Mine(int x, int y);
	int getX() const;
	int getY() const;
	char getSymbol() const;
	void render() const;
};


#pragma once
class Mine
{
private:
	int x, y;
	char symbol = '@';
public:
	Mine(int x, int y);
	int getX() const;
	int getY() const;
	char getSymbol() const;
	void render() const;
};


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
	void render() const;
};


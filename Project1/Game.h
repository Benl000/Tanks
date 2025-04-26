#pragma once
#include "Board.h"
class Game
{
private:
	enum Status {RUN,STOP,PAUSE};
	Status mode;
	int const speed = 500;
	Board board;
public:
	void run();
	void init();
	void cycle();
	void handleInput();
	void setMode(Status s);
	void mainMenu();
};


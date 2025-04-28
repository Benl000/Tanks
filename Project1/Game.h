#pragma once
#include "Board.h"

class Game
{
private:
    enum Status { ON, OFF, PLAY, PAUSE };
    Status mode;
    int const speed = 500;
    Board board;

public:
    void run();
    void init();
    void gameLoop();
    void cycle();
    void handleInput();
    void handlePause();
    void setMode(Status s);
    void mainMenu();
    void printMainMenu();
    void printInstructions();
};

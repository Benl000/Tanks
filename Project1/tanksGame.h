#pragma once
#include "Game.h"

class tanksGame
{
private:
    enum Status { ON, OFF, PLAY, PAUSE };
    Status mode = OFF;
    int const speed = 500;
    Game game;

public:
    void run();
    void init();
    void gameLoop();
    void handleInput();
    void handlePause();
    void setMode(Status s);
    void mainMenu();
    void printMainMenu();
    void printInstructions();
    void printSettings();
};

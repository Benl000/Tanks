#pragma once
#include "Game.h"

class tanksGame
{
private:
    enum Status { ON, OFF, PLAY, PAUSE };
    enum Map { RANDOM, FILE };
    Status mode = OFF;
    Map source;
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
    void printMapSelection();
    void setMap(Map choice);
};

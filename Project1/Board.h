#pragma once
#include <vector>
#include "Tank.h"
#include "Wall.h"
#include "Mine.h"
#include "Shell.h"

using namespace std;

class Board
{
private:
    enum Elements { EMPTY, TANK, CANNON, MINE, SHELL, WALL };
    static const int WIDTH = 80;
    static const int HEIGHT = 25;
    Elements layout[HEIGHT][WIDTH];

    int tankCount = 2;
    int wallClusterCount = 10;
    int mineCount = 10;

    // Game Elements
    vector<Tank> tanks;  //+ cannons
    vector<Wall> walls;
    vector<Mine> mines;
    vector<Shell> shells;

public:
    Board();
    void init();
    void getBoardSize(int& w, int& h);
    void placeTanks();
    void placeWalls();
    void placeMines();
    Elements getElement(int x, int y);
    void renderAll();
    void renderCell(int x, int y);
    void updateLayoutCell(int x, int y, Elements e);
    void cellGotShoot(int x, int y, Shell& shell);
};
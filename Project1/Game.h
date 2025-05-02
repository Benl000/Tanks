#pragma once
#include <vector>
#include "Tank.h"
#include "Wall.h"
#include "Mine.h"
#include "Shell.h"
#include "Player.h"

using namespace std;
class Game
{
private:
	enum Elements { EMPTY, TANK, CANNON, MINE, SHELL, WALL };
	static const int WIDTH = 80;
	static const int HEIGHT = 25;
	Elements board[HEIGHT][WIDTH];

	int playersCount = 2;
	int tankCount = 2;
	int wallClusterCount = 50;
	int mineCount = 6;

	// Game Elements
	vector<Player> players;
	vector<Wall> walls;
	vector<Mine> mines;
	vector<Shell> shells;

public:
	Game();
	void init();
	int getPlayersAmount();
	void getGameSize(int& w, int& h);
	void initPlayers();
	void initWalls();
	void initMines();
	Elements getElement(int x, int y);
	void renderAll();
	void renderCell(int x, int y);
	void renderChanges();
	void updateLayoutCell(int x, int y, Elements e);
	void cellGotShoot(int x, int y, Shell& shell);
	void moveTanks();
	Player& getPlayer(int index) { return players[index]; }
};
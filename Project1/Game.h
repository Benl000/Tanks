#pragma once
#include <vector>
#include "Tank.h"
#include "Wall.h"
#include "Mine.h"
#include "Shell.h"
#include "Player.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;


class Game
{
public:
	static const int WIDTH = 80;
	static const int HEIGHT = 24;
private:
	enum Elements { EMPTY, TANK, CANNON, MINE, SHELL, WALL };
	enum Score { TANK_ON_MINE = -100, HIT_CANNON =50 , HIT_TANK=100, SELF_HIT_CANNON=-50, SELF_HIT_TANK=-100 };


	Elements board[HEIGHT][WIDTH];

	int playersCount = 2;
	int tankCount = 1;
	int wallClusterCount = 100;
	int mineCount = 6;

	int currentScreenIndex;
	bool isColored = true;

	// Game Elements
	vector<Player> players;
	vector<Wall> walls;
	vector<Mine> mines;
	vector<Shell> shells;
	vector<string> screenFiles;

public:
	bool loadScreenFromFile(const string& filename);
	void initPlayers();
	void initWalls();
	void initMines();
	void applyScreenData(const vector<string>& screenData);
	vector<int> findLegendPosition(const vector<string>& screenData) const;
	vector<int> findValidCannonPosition(int tankX, int tankY) const;
	void initRandom();
	void initFromFile();
	int getPlayersAmount();
	int getTanksPerPlayer();
	void setTanksPerPlayer();
	void getGameSize(int& w, int& h);
	bool getColorMode();
	void setColorMode();
	void initShells();
	void initPlayersData();
	Elements getElement(int x, int y);
	vector<Shell>& getShells();
	void renderAll();
	void renderCell(int x, int y);
	void renderScore();
	void updateLayoutCell(int x, int y, Elements e);
	void cellGotShoot(int x, int y, Shell& shell);
	void checkHit(int x, int y, Shell& shell);
	void handleTankHit(Tank* tank, int playerIndex, Shell& shell);
	void handleCannonHit(Tank* tank, int playerIndex, Shell& shell);
	void moveTanks();
	bool canTankMove(Tank* tank, int moveType);
	Player& getPlayer(int index) { return players[index]; }
	bool isCellBlocked(int x, int y);
	void removeMine(Mine* mineToRemove); 
	void removeShell(Shell* shellToRemove); 
	void removeTank(Player& playerTank, Tank* tankToRemove);
	void clearTank(Tank* tank);
	void updateTank(Tank* tank, Player& player);
	void updateShells();
	bool checkGameOver();
	void renderEndGameScreen(int playerIndex);
	Direction::Type getDirectionFromXY(int tankX, int tankY, int CannonX, int cannonY);

};
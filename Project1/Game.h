#pragma once
#include <vector>
#include "Tank.h"
#include "Wall.h"
#include "Mine.h"
#include "Shell.h"
#include "Player.h"
#include "GameRecorder.h" 
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
	enum PlayersMode { HvH = 1, HvC= 2, CvC = 3 };
	enum Score { TANK_ON_MINE = -100, HIT_CANNON =50 , HIT_TANK=100, SELF_HIT_CANNON=-50, SELF_HIT_TANK=-100 };


	Elements board[HEIGHT][WIDTH];
	PlayersMode playersStatus = HvH;

	int playersCount = 2;
	int tankCount = 1;
	int maxTankPerPlayer = 3;
	int wallClusterCount = 4;
	int mineCount = 6;

	int currentScreenIndex;
	bool isColored = true;

	

	// Game Elements
	vector<Wall> walls;
	vector<Mine> mines;
	vector<Shell> shells;
	vector<string> screenFiles;

public:
	vector<Player> players;
	unsigned int gameSeed;
	bool loadScreenFromFile(const string& filename);
	void initPlayers();
	void initWalls();
	void initMines();
	void applyScreenData(const vector<string>& screenData);
	void applyLoadScreenData(const std::string& filename);
	vector<int> findLegendPosition(const vector<string>& screenData) const;
	vector<int> findValidCannonPosition(int tankX, int tankY) const;
	void initRandom();
	std::string initFromFile();
	int getPlayersAmount();
	int getTanksPerPlayer();
	void setTanksPerPlayer();
	void setPlayersMode();
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
	void cellGotShoot(int x, int y, Shell& shell, GameRecorder& recorder, int currentGameTime);
	void checkHit(int x, int y, Shell& shell, GameRecorder& recorder, int currentGameTime);
	void handleTankHit(Tank* tank, int tankIndex, int playerIndex, Shell& shell, GameRecorder& recorder, int currentGameTime);
	void handleCannonHit(Tank* tank, int playerIndex, Shell& shell);
	void moveTanks(GameRecorder &recorder, int currentGameTime);
	bool canTankMove(Tank* tank, int moveType);
	int getPlayerStatus();
	void setClusterSize();
	int getClusterSize();
	Player& getPlayer(int index) { return players[index]; }
	bool isCellBlocked(int x, int y);
	void removeMine(Mine* mineToRemove); 
	void removeShell(Shell* shellToRemove, bool isBroken);
	void handleComputerTurn(Player& player, int playerID, int currentGameTime, GameRecorder& recorder);
	bool shouldSwitchTank(Player& player, Tank* tank);
	bool tryShootOrRotate(int playerID, Tank* tank, Player& player, int activeIndex, int currentGameTime, GameRecorder& recorder);
	bool tryDodgeShells(int playerID, Tank* tank, Player& player, int activeIndex, int currentGameTime, GameRecorder& recorder);
	void handlePatrol(int playerID, Tank* tank, Player& player, int activeIndex, int currentGameTime, GameRecorder& recorder);
	void removeTank(Player& playerTank, Tank* tankToRemove);
	void clearTank(Tank* tank);
	void updateTank(Tank* tank, Player& player,int playerIndex,int TankIndex, GameRecorder& recorder, int currentGameTime);
	void updateShells(GameRecorder& recorder, int currentGameTime);
	int checkGameOver();
	void renderEndGameScreen(int playerIndex);
	Direction::Type getDirectionFromXY(int tankX, int tankY, int CannonX, int cannonY);

};
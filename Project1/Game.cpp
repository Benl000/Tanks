#include "Game.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Game::Game() {
    init();
}

///////////////////////////
/// Game Initialization ///
///////////////////////////

void Game::init()
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            board[y][x] = EMPTY;
        }
    }

    initPlayers();
    initWalls();
    initMines();
}

void Game::initPlayers() {
    players.clear();
    players.resize(playersCount);

    players[0].setControls({ 'q', 'a', 'e', 'd', 's' });
    players[1].setControls({ 'u', 'j', 'o', 'l', 'k' });

    players[0].setColor("blue");
    players[1].setColor("red");

    players[0].addTank(make_unique<Tank>(2, 2, Direction::U, players[0].getColor()));
    players[1].addTank(make_unique<Tank>(77, 21, Direction::U, players[1].getColor()));

    // Mark tanks and cannons on the board
    for (int i = 0; i < playersCount; ++i) {
        for (auto& tank : players[i].getTanks()) {
            updateLayoutCell(tank->getX(), tank->getY(), TANK);
            updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), CANNON);
        }
    }
}


void Game::initWalls() {
    walls.clear();

    for (int i = 0; i < wallClusterCount; ++i) {
        int type = rand() % 3; // 0 = horizontal, 1 = vertical, 2 = block
        int x = rand() % (WIDTH - 5);
        int y = rand() % (HEIGHT - 5);

        switch (type) {
        case 0:
            for (int j = 0; j < 4; ++j) {
                int newX = x + j;
                if (newX < WIDTH && board[y][newX] == EMPTY) {
                    board[y][newX] = WALL;
                    walls.push_back(Wall(newX, y));
                }
            }
            break;
        case 1:
            for (int j = 0; j < 4; ++j) {
                int newY = y + j;
                if (newY < HEIGHT && board[newY][x] == EMPTY) {
                    board[newY][x] = WALL;
                    walls.push_back(Wall(x, newY));
                }
            }
            break;
        case 2:
            for (int dy = 0; dy < 2; ++dy) {
                for (int dx = 0; dx < 3; ++dx) {
                    int newX = x + dx;
                    int newY = y + dy;
                    if (newX < WIDTH && newY < HEIGHT && board[newY][newX] == EMPTY) {
                        board[newY][newX] = WALL;
                        walls.push_back(Wall(newX, newY));
                    }
                }
            }
            break;
        }
    }
}

void Game::initMines() {
    mines.clear();

    int currMineCount = 0;
    while (currMineCount < mineCount) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;

        if (board[y][x] == EMPTY) {
            board[y][x] = MINE;
            mines.push_back(Mine(x, y));
            currMineCount++;
        }
    }
}

/////////////////////
/// Game Settings ///
/////////////////////

void Game::setTanksPerPlayer()
{
    const int MAX_TANKS = 2;
    tankCount = (tankCount % MAX_TANKS) + 1;
}

int Game::getPlayersAmount()
{
    return playersCount;
}

int Game::getTanksPerPlayer()
{
    return tankCount;
}

void Game::getGameSize(int& w, int& h)
{
    w = Game::WIDTH;
    h = Game::HEIGHT;
}

bool Game::getColorMode()
{
    return isColored;
}

void Game::setColorMode()
{
    isColored = !isColored;
    setGlobalColorMode(isColored);
}

/////////////////////////////
/// Game Render and Board ///
/////////////////////////////

void Game::renderCell(int x, int y)
{
    gotoxy(x, y);
    switch (board[y][x]) {
    case EMPTY: cout << ' '; break;
    case WALL:
        for (Wall& wall : walls) {
            if (wall.getX() == x && wall.getY() == y) {
                wall.render();
                break;
            }
        }
        break;
    case MINE:
        for (Mine& mine : mines) {
            if (mine.getX() == x && mine.getY() == y) {
                mine.render();
                break;
            }
        }
        break;
    case SHELL:
        for (Shell& shell : shells) {
            if (shell.getX() == x && shell.getY() == y) {
                shell.render();
                break;
            }
        }
        break;
    case TANK:
        for (int i = 0; i < playersCount; ++i) {
            for (auto& tank : players[i].getTanks()) {
                if (tank->getX() == x && tank->getY() == y) {
                    tank->render();
                    return;
                }
            }
        }
        break;
    case CANNON:
        for (int i = 0; i < playersCount; ++i) {
            for (auto& tank : players[i].getTanks()) {
                if (tank->getCannon().getX() == x && tank->getCannon().getY() == y) {
                    tank->render();
                    return;
                }
            }
        }
        break;
    }
}

Game::Elements Game::getElement(int x, int y)
{
    return board[y][x];
}

void Game::renderAll() {
    system("cls");

    // Walls
    for (Wall& wall : walls) {
        wall.render();
    }

    // Mines
    for (Mine& mine : mines) {
        mine.render();
    }

    // Tanks (from Players)
    for (int i = 0; i < playersCount; ++i) {
        players[i].renderAllTanks();
    }

    // Shells
    for (Shell& shell : shells) {
        shell.render();
    }
}

void Game::updateLayoutCell(int x, int y, Elements e)
{
    board[y][x] = e;
}

///////////////////////////
/// Game Play Manegment ///
///////////////////////////

void Game::cellGotShoot(int x, int y, Shell& shell)
{
    // Not implemented yet (for shooting feature)
}

void Game::moveTanks() {
    for (int i = 0; i < playersCount; ++i) {
        Player& player = players[i];
        Tank* tank = player.getActiveTank();
        if (!tank || tank->isStopped())
            continue;

        int moveType = tank->getMovementType();
        if (moveType == 0)
            continue;

        if (canTankMove(tank, moveType))
            continue;

        // Clear old positions
        clearTank(tank);

        // Move tank and cannon
        tank->move();

        // Update new positions
        updateTank(tank);
    }
}

bool Game::canTankMove(Tank* tank, int moveType) {
    bool canMove = false;

    if (moveType == 1 || moveType == -1) { // Moving forward or backward
        vector<int> newPosition = tank->calculateMovementOffset(moveType);
        if (moveType == -1) { // backward;
            if (isCellBlocked(newPosition[0], newPosition[1]))
            canMove = true;
        }
        else { // forward
            if (isCellBlocked(newPosition[2], newPosition[3]))
            canMove = true;

        }
    }
    else if (moveType == 2) { // Rotate regular clockwise
        auto pos = tank->getCannon().nextXY(tank->rotate(Tank::DOUBLE, Tank::CLOCKWISE,false));
        if (isCellBlocked(pos[0], pos[1]))
        canMove = true;

    }
    else if (moveType == 3) { // Rotate regular counter-clockwise
        auto pos = tank->getCannon().nextXY(tank->rotate(Tank::DOUBLE, Tank::COUNTER_CLOCKWISE,false));
        if (isCellBlocked(pos[0], pos[1]))
        canMove = true;

    }
    else if (moveType == 4) { // Rotate double clockwise
        auto pos = tank->getCannon().nextXY(tank->rotate(Tank::REGULAR, Tank::CLOCKWISE,false));
        if (isCellBlocked(pos[0], pos[1]))
        canMove = true;

    }
    else { // Rotate double counter-clockwise
        auto pos = tank->getCannon().nextXY(tank->rotate(Tank::REGULAR, Tank::COUNTER_CLOCKWISE,false));
        if (isCellBlocked(pos[0], pos[1]))
        canMove = true;

    }
    if (canMove) {
        tank->setBothTracks(Tank::STOPPED);
    }
    return canMove;
}


void Game::clearTank(Tank* tank) {
    updateLayoutCell(tank->getX(), tank->getY(), EMPTY);
    renderCell(tank->getX(), tank->getY());


    // If the cannon was over a mine, re-render the mine
    bool isCannonOverMine = false;
    for (Mine& mine : mines) {
        if (mine.getX() == tank->getCannon().getX() && mine.getY() == tank->getCannon().getY()) {
            isCannonOverMine = true;
            break;
        }
    }

    if (isCannonOverMine) {
        renderCell(tank->getCannon().getX(), tank->getCannon().getY());
    }
    else {
        updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), EMPTY);
        renderCell(tank->getCannon().getX(), tank->getCannon().getY());
    }
}

void Game::updateTank(Tank* tank) {
    updateLayoutCell(tank->getX(), tank->getY(), TANK);

    bool isCannonOverMine = false;
    for (Mine& mine : mines) {
        if (mine.getX() == tank->getCannon().getX() && mine.getY() == tank->getCannon().getY()) {
            isCannonOverMine = true;
            break;
        }
    }
    updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), CANNON);
    renderCell(tank->getCannon().getX(), tank->getCannon().getY());
    if (isCannonOverMine) {
        updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), MINE);
    }
}

void Game::renderChanges() {
    // Redraw moving objects only

    // Move Shells
    for (Shell& shell : shells) {
        // erase previous position
        gotoxy(shell.getX(), shell.getY());
        cout << ' ';

        // update shell position (if you have such logic)
        // shell.move(); // (depends if you have shell movement)

        // draw new position
        shell.render();
    }

    // Move Tanks (and their cannons)
    for (int i = 0; i < playersCount; ++i) {
        Player& player = players[i];
        player.renderAllTanks();  // render active tank
    }
}

bool Game::isCellBlocked(int x, int y)
{
    Elements el = getElement(x, y);
	if (el==WALL || el==TANK ||el==CANNON)
		return true;
	return false;
}



#include "Game.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Game::Game() {
    init();
}

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

int Game::getPlayersAmount()
{
    return playersCount;
}

void Game::getGameSize(int& w, int& h)
{
    w = Game::WIDTH;
    h = Game::HEIGHT;
}

void Game::initPlayers() {
    players.clear();

    players.resize(playersCount);

    players[0].setControls({ 'q', 'a', 'e', 'd', 's' });  // Player 1 keys
    players[1].setControls({ 'u', 'j', 'o', 'l', 'k' });  // Player 2 keys


    players[0].setColor("blue");  // Player 1 color
    players[1].setColor("red");  // Player 2 color

    players[0].addTank(Tank(2, 2, Direction::U));
    players[1].addTank(Tank(77, 21, Direction::U));
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

void Game::renderCell(int x, int y)
{
    gotoxy(x, y);
    switch (board[y][x]) {
    case EMPTY: cout << ' '; break;
    case WALL:
        for (Wall& wall : walls) {
            if (wall.getX() == x && wall.getY() == y) {
                cout << wall.getSymbol();
                break;
            }
        }
        break;
    case MINE:
        for (Mine& mine : mines) {
            if (mine.getX() == x && mine.getY() == y) {
                cout << mine.getSymbol();
                break;
            }
        }
        break;
    case SHELL:
        for (Shell& shell : shells) {
            if (shell.getX() == x && shell.getY() == y) {
                cout << shell.getSymbol();
                break;
            }
        }
        break;
    case TANK:
    case CANNON:
        for (int i = 0; i < playersCount; ++i) {
            Tank* activeTank = players[i].getActiveTank();
            if (activeTank && activeTank->getX() == x && activeTank->getY() == y) {
                activeTank->render();
                break;
            }
        }
        break;
    }
}

void Game::updateLayoutCell(int x, int y, Elements e)
{
    board[y][x] = e;
}

void Game::cellGotShoot(int x, int y, Shell& shell)
{
    // Not implemented yet (for shooting feature)
}

void Game::moveTanks() {
    for (int i = 0; i < playersCount; ++i) {
        Tank* tank = players[i].getActiveTank();
        if (tank) {
            tank->move();  // move each tank
        }
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



#include "Board.h"
#include "Utils.h"
#include <iostream>

Board::Board() {
    generateBoard();
}

void Board::init()
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            layout[y][x] = EMPTY;
        }
    }

    placeTanks();
    placeWalls();
    placeMines();

}

void Board::getBoardSize(int& w, int& h)
{
    w = Board::WIDTH;
    h = Board::HEIGHT;
}

void Board::placeTanks() {
    tanks.push_back(Tank(2, 2,Direction::Type::U));
    tanks.push_back(Tank(77, 21,Direction::Type::U)));

    for (const Tank& tank : tanks) {
        int x = tank.getX();
        int y = tank.getY();
        if (layout[y][x] == EMPTY) {
            updateLayoutCell(x,y,TANK);
        }
        //update cannon on layout
    }
}

void Board::placeWalls() {
    for (int i = 0; i < wallClusterCount; ++i) {
        int type = rand() % 3; // 0 = horizontal, 1 = vertical, 2 = block
        int x = rand() % (WIDTH - 5);
        int y = rand() % (HEIGHT - 5);

        switch (type) {
        case 0: // horizontal line of 4 walls
            for (int i = 0; i < 4; ++i) {
                int newX = x + i;
                if (newX < 0 || newX >= WIDTH) continue;
                if (layout[y][newX] == EMPTY) {
                    layout[y][newX] = WALL;
                    walls.push_back(Wall(newX, y));
                }
            }
            break;

        case 1: // vertical line of 4 walls
            for (int i = 0; i < 4; ++i) {
                int newY = y + i;
                if (newY < 0 || newY >= HEIGHT) continue;
                if (layout[newY][x] == EMPTY) {
                    layout[newY][x] = WALL;
                    walls.push_back(Wall(x, newY));
                }
            }
            break;

        case 2: // block (2 rows x 3 columns)
            for (int dy = 0; dy < 2; ++dy) {
                for (int dx = 0; dx < 3; ++dx) {
                    int newX = x + dx;
                    int newY = y + dy;
                    if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT) continue;
                    if (layout[newY][newX] == EMPTY) {
                        layout[newY][newX] = WALL;
                        walls.push_back(Wall(newX, newY));
                    }
                }
            }
            break;
        }
    }
}

void Board::placeMines() {
    int currMineCount = 0;
    while (currMineCount < mineCount) {
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;

        if (layout[y][x] == EMPTY) {
            layout[y][x] = MINE;
            mines.push_back(Mine(x, y));
            currMineCount++;
        }
    }
}

Board::Elements Board::getElement(int x, int y)
{
    return layout[y][x];
}

void Board::renderAll() {
    system("cls");
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            renderCell(x, y);
        }
    }

    for (Tank& tank : tanks) {
        tank.render();
    }

    for (Wall& wall : walls) {
        wall.render();
    }

    for (Mine& mine : mines) {
        mine.render();
    }

    for (Shell& shell : shells) {
        shell.render();
    }
}

void Board::renderCell(int x, int y)
{
    gotoxy(x, y);
    switch (layout[y][x])
    {
    case Board::EMPTY:
        cout << ' ';
        break;
    case Board::TANK:
        for (const auto& tank : tanks) {
            if (tank.getX() == x && tank.getY() == y) {
                tank.render();
                break;
            }
        }
        break;
    case Board::CANNON:
        for (const auto& tank : tanks) {
            if (tank.cannon.getX() == x && tank.cannon.getY() == y) {
                tank.render();
                break;
            }
        }
        break;
    case Board::MINE:
        for (const auto& mine : mines) {
            if (mine.getX() == x && mine.getY() == y) {
                mine.render();
                break;
            }
        }
        break;
    case Board::SHELL:
        for (const auto& shell : shells) {
            if (shell.getX() == x && shell.getY() == y) {
                shell.render();
                break;
            }
        }
        break;
    case Board::WALL:
        for (const Wall& wall : walls) {
            if (wall.getX() == x && wall.getY() == y) {
                cout << wall.getSymbol();
                break;
            }
        }
        break;
    default:
        break;
    }
}

void Board::updateLayoutCell(int x, int y, Elements e)
{
    layout[y][x] = e;
}

void Board::cellGotShoot(int x, int y, Shell& shell)
{
    int prevX = x;
    int prevY = y;

    switch (shell.getDirection()) {
    case Direction::U:
        prevY--;
        break;
    case Direction::UR:
        prevX--;
        prevY--;
        break;
    case Direction::R:
        prevX--;
        break;
    case Direction::DR:
        prevX--;
        prevY++;
        break;
    case Direction::D:
        prevY++;
        break;
    case Direction::DL:
        prevX++;
        prevY++;
        break;
    case Direction::L:
        prevX++;
        break;
    case Direction::UL:
        prevX++;
        prevY--;
        break;
    }

    renderCell(prevX, prevY);

    // Handle interaction with the current cell
    switch (layout[y][x]) {
    case Board::EMPTY:
        break;
    case Board::TANK:
        break;
    case Board::CANNON:
        break;
    case Board::MINE:
        break;
    case Board::SHELL:
        break;
    case Board::WALL:
        for (auto it = walls.begin(); it != walls.end(); ) {
            if (it->getX() == x && it->getY() == y) {
                if (it->gotShoot()) {
                    updateLayoutCell(x,y,EMPTY);
                    renderCell(x, y);

                    // Find and remove the shell from shells vector
                    for (auto sIt = shells.begin(); sIt != shells.end(); ++sIt) {
                        if (&(*sIt) == &shell) {
                            shells.erase(sIt);
                            break;
                        }
                    }

                    it = walls.erase(it);  // Remove wall
                }
                else {
                    ++it;
                }
                return;
            }
            else {
                ++it;
            }
        }
        break;
    default:
        break;
    }
}

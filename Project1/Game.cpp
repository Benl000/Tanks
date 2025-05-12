#include "Game.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <random>

using std::cout;
using std::endl;
using std::string;

using std::make_unique;


/////////////////////////////////////
/// Game Initialization from FILE ///
/////////////////////////////////////

void Game::initFromFile() {
    system("cls");
    screenFiles.clear();
    currentScreenIndex = 0;

    // Console dimensions
    int screenWidth = 80; // Assuming 80-character width console
    int centerX = screenWidth / 2;

    // Find screen files
    string pattern = "tanks-game*.screen";
    for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        string fileName = entry.path().filename().string();
        if (entry.is_regular_file() && fileName.find("tanks-game") == 0 && fileName.find(".screen") == fileName.length() - 7) {
            screenFiles.push_back(fileName);
        }
    }
    std::sort(screenFiles.begin(), screenFiles.end());

    // If no screen files found
    if (screenFiles.empty()) {
        setColorByName("bright red");
        gotoxy(centerX - 20, 10);
        cout << "========================================";
        gotoxy(centerX - 20, 11);
        cout << "     Error: No screen files found.     ";
        gotoxy(centerX - 20, 12);
        cout << "        Cannot start a new game.       ";
        gotoxy(centerX - 20, 13);
        cout << "========================================";
        resetColor();
        return;
    }

    // Display available screen files
    setColorByName("bright yellow");
    gotoxy(centerX - 20, 6);
    cout << "========================================";
    gotoxy(centerX - 20, 7);
    cout << "        Available Screen Files         ";
    gotoxy(centerX - 20, 8);
    cout << "========================================";
    resetColor();

    // List files in the center
    int listStartY = 10;
    for (size_t i = 0; i < screenFiles.size(); ++i) {
        setColorByName("bright cyan");
        gotoxy(centerX - 15, listStartY + i);
        cout << "[" << i + 1 << "] " << screenFiles[i];
    }

    // User choice input
    setColorByName("bright magenta");
    gotoxy(centerX - 20, listStartY + screenFiles.size() + 2);
    cout << "Choose a screen to play and press enter: ";
    resetColor();

    // Move cursor for clean input
    gotoxy(centerX - 15, listStartY + screenFiles.size() + 4);
    cout << "Your choice: ";
    int choice;
    gotoxy(centerX, listStartY + screenFiles.size() + 4); // Cursor placed after "Your choice: "
    std::cin >> choice;

    // Process choice
    if (choice >= 1 && choice <= screenFiles.size()) {
        std::string selectedFile = screenFiles[choice - 1];

        setColorByName("bright green");
        gotoxy(centerX - 15, listStartY + screenFiles.size() + 6);
        cout << "Loading screen: " << selectedFile << "...";
        resetColor();

        if (!loadScreenFromFile(selectedFile)) {
            setColorByName("bright red");
            gotoxy(centerX - 15, listStartY + screenFiles.size() + 8);
            cout << "Error loading file: " << selectedFile << ". Exiting game.";
            resetColor();
            return;
        }
    }
    else {
        setColorByName("bright red");
        gotoxy(centerX - 15, listStartY + screenFiles.size() + 6);
        cout << "Invalid choice. Exiting game.";
        resetColor();
        return;
    }

    initShells();
}

vector<int> Game::findLegendPosition(const vector<string>& screenData) const {
    for (size_t y = 0; y < screenData.size(); ++y) {
        size_t x = screenData[y].find('L');
        if (x != string::npos) {
            return { static_cast<int>(x), static_cast<int>(y) };
        }
    }
    return { -1, -1 };
}

vector<int> Game::findValidCannonPosition(int tankX, int tankY) const {
    vector<vector<int>> potentialPositions = {
        {tankX + 1, tankY}, {tankX - 1, tankY}, {tankX, tankY + 1}, {tankX, tankY - 1},
        { tankX + 1, tankY + 1 }, {tankX - 1, tankY - 1}, {tankX - 1, tankY + 1}, {tankX + 1, tankY - 1}
    };
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(potentialPositions.begin(), potentialPositions.end(), gen); // Randomize search

    for (const auto& pos : potentialPositions) {
        if (pos[0] >= 0 && pos[0] < WIDTH && pos[1] >= 0 && pos[1] < HEIGHT && board[pos[1]][pos[0]] == EMPTY) {
            return pos;
        }
    }
    return { -1, -1 }; // Indicate no valid position found
}

Direction::Type Game::getDirectionFromXY(int tankX, int tankY, int CannonX, int cannonY) {
    if (tankX == CannonX && tankY < cannonY) {
        return Direction::U;
    }
    else if (tankX < CannonX && tankY < cannonY) {
        return Direction::UR;
    }
    else if (tankX < CannonX && tankY == cannonY) {
        return Direction::R;
    }
    else if (tankX < CannonX && tankY > cannonY) {
        return Direction::DR;
    }
    else if (tankX == CannonX && tankY > cannonY) {
        return Direction::D;
    }
    else if (tankX > CannonX && tankY > cannonY) {
        return Direction::DL;
    }
    else if (tankX > CannonX && tankY == cannonY) {
        return Direction::L;
    }
    else if (tankX > CannonX && tankY < cannonY) {
        return Direction::UL;
    }
    else {
        return Direction::U; // Default direction if no match found
    }
}

void Game::applyScreenData(const std::vector<std::string>& screenData) {
    // Clear existing game state
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            board[y][x] = EMPTY;
        }
    }
    

    walls.clear();
    mines.clear();
    initPlayersData();

    // Iterate through the screen data
    for (size_t y = 0; y < screenData.size() && y < HEIGHT; ++y) {
        for (size_t x = 0; x < screenData[y].length() && x < WIDTH; ++x) {
            char c = screenData[y][x];
            switch (c) {
            case '#':
                board[y][x] = WALL;
                walls.push_back(Wall(static_cast<int>(x), static_cast<int>(y)));
                break;
            case '1': {
                board[y][x] = TANK;
                vector<int> cannonPos = findValidCannonPosition(x, y);
                Direction::Type d = getDirectionFromXY(x, y, cannonPos[0], cannonPos[1]);
                board[cannonPos[1]][cannonPos[0]] = CANNON;
                players[0].addTank(make_unique<Tank>(static_cast<int>(x), static_cast<int>(y), d, players[0].getColor()));
                break;
            }
            case '2': {
                board[y][x] = TANK;
                vector<int> cannonPos = findValidCannonPosition(x, y);
                Direction::Type d = getDirectionFromXY(x, y, cannonPos[0], cannonPos[1]);
                board[cannonPos[1]][cannonPos[0]] = CANNON;
                players[1].addTank(make_unique<Tank>(static_cast<int>(x), static_cast<int>(y), d, players[1].getColor()));
                break;
            }
            case '@':
                board[y][x] = MINE;
                mines.push_back(Mine(static_cast<int>(x), static_cast<int>(y)));
                break;
            case 'L':
                // Handle legend position if needed, though the instructions say it's just an indicator
                break;
            case ' ': 
                board[y][x] = EMPTY;
                break;
            case '\n':
                // Ignore empty spaces and newlines
                break;
            default:
                // You might want to handle unknown characters or log a warning
                break;
            }
        }
    }
}

bool Game::loadScreenFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening screen file: " << filename << std::endl;
        return false;
    }
    std::vector<std::string> screenData;
    std::string line;
    while (std::getline(file, line)) {
        screenData.push_back(line);
    }
    file.close();

    // Ensure the loaded screen fits within the game board dimensions (you might need to adjust this)
    if (screenData.size() > HEIGHT) {
        screenData.resize(HEIGHT);
    }
    for (auto& row : screenData) {
        if (row.length() > WIDTH) {
            row.resize(WIDTH);
        }
    }

    applyScreenData(screenData);
    return true;
}

///////////////////////////////////////
/// Game Initialization from RANDOM ///
///////////////////////////////////////

void Game::initRandom()
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            board[y][x] = EMPTY;
        }
    }

    initPlayers();
    initWalls();
    initMines();
    initShells();
}

void Game::initPlayers() {

    initPlayersData();

    players[0].addTank(make_unique<Tank>(2, 2, Direction::U, players[0].getColor()));
    players[1].addTank(make_unique<Tank>(77, 21, Direction::U, players[1].getColor()));

    if (tankCount == 2) {

        players[0].addTank(make_unique<Tank>(28, 2, Direction::U, players[0].getColor()));
        players[1].addTank(make_unique<Tank>(50, 21, Direction::U, players[1].getColor()));
    }
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

void Game::initShells() {
    shells.clear();

}

/////////////////////
/// Game Settings ///
/////////////////////

void Game::initPlayersData() {
    players.clear();
    players.resize(playersCount);

    players[0].setControls({ 'q', 'a', 'e', 'd', 's','w','z' });
    players[1].setControls({ 'u', 'j', 'o', 'l', 'k','i','m' });

    players[0].setColor("blue");
    players[1].setColor("red");
}

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

vector<Shell>& Game::getShells()
{
    return shells;
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
        for (auto& tank : players[i].getTanks()) {
            // Set the active flag based on the active tank
            tank->setActive(tank.get() == players[i].getActiveTank());
            tank->render();
        }
    }

    // Shells
    for (Shell& shell : shells) {
        shell.render();
    }

    renderScore();
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
    switch (getElement(x, y))
    {
    case EMPTY:
        updateLayoutCell(x, y, SHELL);
        renderCell(x, y);
        break;
    case WALL:
        for (Wall& wall : walls) {
            if (wall.getX() == x && wall.getY() == y) {
                if (wall.gotShoot()) {
                    updateLayoutCell(x, y, EMPTY);
                }
                renderCell(x, y);
                removeShell(&shell);
                return;
            }
        }
        break;
    case TANK:
    case CANNON:
        checkHit(x,y,shell);
        break;
    case MINE:
        updateLayoutCell(x, y, SHELL);
        renderCell(x, y);
        updateLayoutCell(x, y, MINE);
        shell.setprevStatus(false);
        break;
    }
}

void Game::checkHit(int x, int y, Shell& shell) {
    for (int i = 0; i < playersCount; ++i) {
        for (auto& tank : players[i].getTanks()) {
            if (tank->getX() == x && tank->getY() == y) {
                handleTankHit(tank.get(), i, shell);
                return; // Tank hit, no need to continue
            }
            else if (tank->getCannon().getX() == x && tank->getCannon().getY() == y) {
                handleCannonHit(tank.get(), i, shell);
                return; // Cannon hit, no need to continue
            }
        }
    }
}

void Game::handleTankHit(Tank* tank, int playerIndex, Shell& shell) {
    if (playerIndex == shell.getShooterID()) {
        players[playerIndex].updateScore(SELF_HIT_TANK);
    }
    else {
        players[shell.getShooterID()].updateScore(HIT_TANK);
    }

    renderScore();
    removeShell(&shell);
    removeTank(players[playerIndex], tank);
    checkGameOver();
    players[playerIndex].getActiveTank()->setActive(true);

}

void Game::handleCannonHit(Tank* tank, int playerIndex, Shell& shell) {
    if (playerIndex == shell.getShooterID()) {
        players[playerIndex].updateScore(SELF_HIT_CANNON);
    }
    else {
        players[shell.getShooterID()].updateScore(HIT_CANNON);
    }

    renderScore();
    removeShell(&shell);

    // Fully destroy the cannon
    tank->getCannon().setCondition(Cannon::Condition::BROKEN);
    updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), EMPTY);
    renderCell(tank->getCannon().getX(), tank->getCannon().getY());
}

void Game::moveTanks() {
    for (int i = 0; i < playersCount; ++i) {
        Player& player = players[i];

        for (auto& tank : player.getTanks()) {
            if (tank) {
                tank->reduceCoolDown();

                if (tank->isStopped())
                    continue;

                int moveType = tank->getMovementType();
                if (moveType == 0)
                    continue;

                if (canTankMove(tank.get(), moveType))
                    continue;

                // Clear old positions
                clearTank(tank.get());

                // Move tank and cannon
                tank->move();

                // Update new positions
                updateTank(tank.get(), player);
            }
        }
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

    if (!isCannonOverMine) {
        updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), EMPTY);
    }

    renderCell(tank->getCannon().getX(), tank->getCannon().getY());

}

void Game::updateTank(Tank* tank,Player& player) {
    updateLayoutCell(tank->getX(), tank->getY(), TANK);

    bool isTankOverMine = false;
    bool isCannonOverMine = false;

    for (Mine& mine : mines) {
        if (mine.getX() == tank->getCannon().getX() && mine.getY() == tank->getCannon().getY()) {
            isCannonOverMine = true;
            break;
        }
        if (mine.getX() == tank->getX() && mine.getY() == tank->getY()) {
            isTankOverMine = true;
            removeMine(&mine);
            break;
        }
    }
    updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), CANNON);
    renderCell(tank->getCannon().getX(), tank->getCannon().getY());
    if (isCannonOverMine) {
        updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), MINE);
    }
    if (isTankOverMine) {
        
        removeTank(player, tank);
        player.updateScore(TANK_ON_MINE);
       checkGameOver();
       player.getActiveTank()->setActive(true);

    }
}

void Game::updateShells()
{
    for (auto& shell : shells) {
        if (shell.isPrevEmpty()) {
            updateLayoutCell(shell.getX(), shell.getY(), EMPTY);   
        }
        renderCell(shell.getX(), shell.getY());
        shell.setprevStatus(true);
        shell.move();
        cellGotShoot(shell.getX(), shell.getY(), shell);
    }
}

bool Game::checkGameOver()
{
    for (int i = 0; i < players.size(); ++i) {
        if (!players[i].hasTanks()) {
            renderEndGameScreen(1 - i); // Renders the winning player (0 -> 1, 1 -> 0)
            return true;
        }
    }
    return false;
}

void Game::renderEndGameScreen(int playerIndex)
{
    // Calculate message positions
    int centerX = WIDTH / 2;
    int centerY = HEIGHT / 2;
    string message = "Game Over!";
    string playerMessage = "Player " + std::to_string(playerIndex + 1) + " Won!";

    // Clear the entire area of the end game screen (rectangle)
    setColorByName("bright white");
    for (int y = centerY - 4; y <= centerY + 4; ++y) {
        gotoxy(centerX - 15, y);
        cout << string(31, ' ');  // Clear 31 characters wide
    }

    // Draw border
    for (int i = centerX - 15; i <= centerX + 15; ++i) {
        gotoxy(i, centerY - 4);
        cout << "#";
        gotoxy(i, centerY + 4);
        cout << "#";
    }
    for (int i = centerY - 4; i <= centerY + 4; ++i) {
        gotoxy(centerX - 15, i);
        cout << "#";
        gotoxy(centerX + 15, i);
        cout << "#";
    }

    // Display the "Game Over" message
    gotoxy(centerX - static_cast<int>(message.size()) / 2, centerY - 2);
    setColorByName("bright yellow");
    cout << message;

    // Display the winning player message in their color
    gotoxy(centerX - static_cast<int>(playerMessage.size()) / 2, centerY);
    setColorByName(players[playerIndex].getColor());
    cout << playerMessage;

    // Display the "Press any key" message
    gotoxy(centerX - 13, centerY + 2);
    setColorByName("bright cyan");
    cout << "Press any key to continue...";

    resetColor();
}





void Game::renderScore()
{
    // Clear the score line
    gotoxy(0, HEIGHT);
    cout << string(WIDTH, ' '); // Clear the line

    // Player 1 - Left Side
    gotoxy(0, HEIGHT);
    setColorByName(players[0].getColor());
    cout << "Player 1: ";
    resetColor();
    cout << std::setw(4) << std::setfill('0') << players[0].getScore();

    // Player 2 - Right Side
    int rightPosition = WIDTH - 14;
    gotoxy(rightPosition, HEIGHT);
    setColorByName(players[1].getColor());
    cout << "Player 2: ";
    resetColor();
    cout << std::setw(4) << std::setfill('0') << players[1].getScore();
}

bool Game::isCellBlocked(int x, int y)
{
    Elements el = getElement(x, y);
	if (el==WALL || el==TANK ||el==CANNON)
		return true;
	return false;
}

void Game::removeMine( Mine* mineToRemove) {
    if (!mineToRemove) return; 

    auto it = find_if(mines.begin(), mines.end(), [&](const Mine& mine) {
        return &mine == mineToRemove;
        });

    if (it != mines.end()) {
        mines.erase(it);
    }
}

void Game::removeTank(Player& playerTank, Tank* tankToRemove) {
    if (!tankToRemove) return;

    auto& tanks = playerTank.getTanks();
    auto it = std::find_if(tanks.begin(), tanks.end(), [&](const std::unique_ptr<Tank>& tank) {
        updateLayoutCell(tank->getX(), tank->getY(), EMPTY);
        updateLayoutCell(tank->getCannon().getX(), tank->getCannon().getY(), EMPTY);
        renderCell(tank->getX(), tank->getY());
        renderCell(tank->getCannon().getX(), tank->getCannon().getY());
        return tank.get() == tankToRemove; // Compare using the raw pointer
        });

    if (it != tanks.end()) {
        tanks.erase(it);
    }
}

void Game::removeShell(Shell* shellToRemove) {
    if (!shellToRemove) return;

    auto it = find_if(shells.begin(), shells.end(), [&](const Shell& shell) {
        
        return &shell == shellToRemove;
        });

    if (it != shells.end()) {
        updateLayoutCell(it->getX(), it->getY(), EMPTY);
        shells.erase(it);
    }
}
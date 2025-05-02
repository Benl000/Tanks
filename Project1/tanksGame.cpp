#include "tanksGame.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void tanksGame::run()
{
    setMode(ON);
    mainMenu();
}

void tanksGame::init()
{
    setMode(PLAY);
    game.init();
    game.renderAll();
    gameLoop();
}

void tanksGame::gameLoop()
{
    while (mode == PLAY) {

        handleInput();

        if (mode == PAUSE) {
            handlePause(); // Manage pause screen
        }
        else {
            // updateShells(); // Move and process shells

            Sleep(speed / 2);

            game.moveTanks();
            // updateShells(); // Move shells again
        }

        Sleep(speed / 2);

    }
}

void tanksGame::handleInput()
{
    while (_kbhit()) {
        char key = _getch();

        for (int i = 0; i < game.getPlayersAmount(); ++i) {
            game.getPlayer(i).handleInput(key);
        }
    }
}

void tanksGame::handlePause()
{
    // Will be added later if needed
}

void tanksGame::setMode(Status s)
{
    mode = s;
}

void tanksGame::mainMenu()
{
    printMainMenu();

    while (mode == ON) {
        char choice = _getch();
        switch (choice) {
        case '1':
            init();
            break;
        case '8':
            printInstructions();
            printMainMenu();
            break;

        case '9':
            setMode(OFF);
            break;
        default:
            _getch();
            break;
        }
    }
}

void tanksGame::printMainMenu() {
    system("cls");
    cout << "=== Tank Battle ===" << endl;
    cout << "(1) Start a new game" << endl;
    cout << "(8) Instructions" << endl;
    cout << "(9) Exit" << endl;
}

void tanksGame::printInstructions()
{
    system("cls");
    cout << "=== Instructions ===" << endl;
    cout << "Player 1 Controls (Tank 1):" << endl;
    cout << "  q - Left track forward/stop" << endl;
    cout << "  a - Left track backward/stop" << endl;
    cout << "  e - Right track forward/stop" << endl;
    cout << "  d - Right track backward/stop" << endl;
    cout << "  s - Stop both tracks" << endl;
    cout << "Player 2 Controls (Tank 2):" << endl;
    cout << "  u - Left track forward/stop" << endl;
    cout << "  j - Left track backward/stop" << endl;
    cout << "  o - Right track forward/stop" << endl;
    cout << "  l - Right track backward/stop" << endl;
    cout << "  k - Stop both tracks" << endl;

    cout << "\nPress ESC to go back to the menu..." << endl;

    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 27) // 27 is ASCII code for ESC
                break;     // Exit the loop and return to mainMenu
        }
    }
}


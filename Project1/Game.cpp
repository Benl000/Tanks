#include "Game.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
using namespace std;

void Game::run()
{
    setMode(ON);
    mainMenu();
}

void Game::init()
{
    setMode(PLAY);
    board.init();
    board.renderAll();
    gameLoop();
}

void Game::gameLoop()
{
    while (mode == PLAY) {

        handleInput();

        if (mode == PAUSE) {
            handlePause(); // Manage pause screen
        }
        else {
           // updateShells(); // Move and process shells

            Sleep(speed / 2);

           // updateGame();   // Move tanks and check for collisions //board.moveTanks();  board.renderChanges();
           // updateShells(); // Move shells again
        }

        Sleep(speed / 2);
        
    }
}

void Game::handleInput()
{
    if (_kbhit()) {
        char key = _getch();

        for (int i = 0; i < board.getPlayersAmount(); ++i) {
            board.getPlayer(i).handleInput(key);
        }
    }
}

void Game::handlePause()
{
    // Will be added later if needed
}

void Game::setMode(Status s)
{
    mode = s;
}

void Game::mainMenu()
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

void Game::printMainMenu() {
    system("cls");
    cout << "=== Tank Battle ===" << endl;
    cout << "(1) Start a new game" << endl;
    cout << "(8) Instructions" << endl;
    cout << "(9) Exit" << endl;
}

void Game::printInstructions()
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


#include "tanksGame.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
using std::cout;
using std::endl;
using std::string;

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
            game.updateShells();

            Sleep(speed / 2);

            game.moveTanks();
            game.updateShells();
        }

        Sleep(speed / 2);

    }
}

void tanksGame::handleInput()
{
    while (_kbhit()) {
        char key = tolower(_getch());

        for (int i = 0; i < game.getPlayersAmount(); ++i) {
            game.getPlayer(i).handleInput(key, game.getShells(),i);

        }
        if (key == 27) mode = PAUSE;
    }
}

void tanksGame::handlePause()
{
    system("cls");
    cout << "Game paused," << endl;
    cout << "press ESC again to continue" << endl;
    cout << "or X to go back to the main menu" << endl;

    while (mode==PAUSE) {
        if (_kbhit()) {
            char key = tolower(_getch());
            switch (key) {
            case 27:
                mode=PLAY;
                game.renderAll();
                break;
            case 'x':
                mode = ON;
                mainMenu();
                break;
            }
        }
        Sleep(50);
    }
}

void tanksGame::setMode(Status s)
{
    mode = s;
}

void tanksGame::mainMenu()
{
    printMainMenu();

    while (mode == ON) {
        char choice = tolower(_getch());
        switch (choice) {
        case '1':
            init();
            break;
        case '7':
            printSettings();
            printMainMenu();
            break;
        case '8':
            printInstructions();
            printMainMenu();
            break;
        case '9':
            setMode(OFF);
            break;
        default:
            choice = tolower(_getch());
            break;
        }
    }
}

void tanksGame::printMainMenu() {
    system("cls");
    cout << "=== Tank Battle ===" << endl;
    cout << "(1) Start a new game" << endl;
    cout << "(7) Settings" << endl;
    cout << "(8) Instructions" << endl;
    cout << "(9) Exit" << endl;
}

void tanksGame::printInstructions()
{
    system("cls");
    cout << "=== Instructions ===" << endl;

    for (int i = 0; i < game.getPlayersAmount(); i++) {
        Player& p = game.getPlayer(i);
        Player::ControlKeys& keys = p.getControls();
        cout << "Player " << i + 1 << " Controls:" << endl;
        cout << "  " << keys.leftForward << " - Left track forward/stop" << endl;
        cout << "  " << keys.leftBackward << " - Left track backward/stop" << endl;
        cout << "  " << keys.rightForward << " - Right track forward/stop" << endl;
        cout << "  " << keys.rightBackward << " - Right track backward/stop" << endl;
        cout << "  " << keys.stopBoth << " - Stop both tracks" << endl;
        cout << "  " << keys.shoot << " - Shoot" << endl;
        cout << "  " << keys.switchActiveTank << " - Switch Active Tank" << endl;
        cout << endl;
    }
    cout << "\nPress ESC to go back to the menu..." << endl;

    while (true) {
        if (_kbhit()) {
            char key = tolower(_getch());
            if (key == 27) // 27 is ASCII code for ESC
                break;     // Exit the loop and return to mainMenu
        }
    }
}

void tanksGame::printSettings()
{
    while (true) {
        system("cls");
        cout << "=== Settings ===" << endl;
        cout << "To change, enter the setting number." << endl << endl;
        cout << "(1) Number of tanks per player: " << game.getTanksPerPlayer() << endl;
        cout << "(2) Colored game: " << (game.getColorMode() ? "ON" : "OFF") << endl;
        cout << "(ESC) Back to main menu" << endl;

        char key = tolower(_getch());
        switch (key) {
        case 27: // ESC
            return; // Exit the function safely
        case '1':
            game.setTanksPerPlayer();
            break;
        case '2':
            game.setColorMode();
            break;
        default:
            break;
        }
    }
}


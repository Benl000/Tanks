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
    switch (source) {
    case RANDOM:
        game.initRandom();
        break;
    case FILE:
        game.initFromFile();
        break;
    }
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
        if (game.checkGameOver())
        {
            break;
        }

    }
    mode = ON;
    _getch(); // wait for the player to continue afte game over
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

void tanksGame::printPauseScreen() {
    // Clear the area for the pause screen (without full cls)
    for (int y = 10; y <= 15; ++y) {
        gotoxy(20, y);
        cout << string(40, ' ');  // Clear 40 characters width
    }

    // Render the pause screen in a visually appealing layout
    setColorByName("bright yellow");
    gotoxy(30, 11);
    cout << "== Game Paused ==";

    setColorByName("bright cyan");
    gotoxy(28, 13);
    cout << "Press ESC to continue";

    setColorByName("bright magenta");
    gotoxy(28, 14);
    cout << "Press X to return to the Main Menu";

    resetColor();
}

void tanksGame::handlePause()
{
    printPauseScreen();

    // Handle pause interaction
    while (mode == PAUSE) {
        if (_kbhit()) {
            char key = tolower(_getch());
            switch (key) {
            case 27: // ESC key
                mode = PLAY;
                game.renderAll();
                return;
            case 'x':
                mode = ON;
                mainMenu();
                return;
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
    while (mode == ON) {
        printMainMenu();
        char choice = tolower(_getch());
        switch (choice) {
        case '1':
            printMapSelection();
            break;
        case '7':
            printSettings();
            break;  // No need for printMainMenu() here
        case '8':
            printInstructions();
            break;  // No need for printMainMenu() here
        case '9':
            setMode(OFF);
            break;
        default:
            break;
        }
    }
}

void tanksGame::printMainMenu() {
   
    system("cls");

    // Render the main menu with a stylish ice-cream theme
    setColorByName("bright yellow");
    gotoxy(30, 8);
    cout << "===== Tank Battle =====";

    setColorByName("bright cyan");
    gotoxy(30, 10);
    cout << "(1) Start a New Game";

    setColorByName("bright magenta");
    gotoxy(30, 11);
    cout << "(7) Settings";

    setColorByName("bright green");
    gotoxy(30, 12);
    cout << "(8) Instructions";

    setColorByName("bright red");
    gotoxy(30, 13);
    cout << "(9) Exit";

    resetColor();
}


void tanksGame::printInstructions()
{
    game.initPlayersData();

    system("cls");

    // Calculate the centered X position
    int centerX = (Game::WIDTH / 2);
    int startY = 4;

    // Render the instructions title with a colorful border
    setColorByName("bright yellow");
    gotoxy(centerX - 18, startY-2);
    cout << "   ======== Instructions =========";

    for (int i = 0; i < game.getPlayersAmount(); i++) {
        Player& p = game.getPlayer(i);
        Player::ControlKeys& keys = p.getControls();

        // Player title with player color
        setColorByName("bright cyan");
        gotoxy(centerX - 15, startY);
        cout << "Player " << i + 1 << " Controls:";

        // Player controls in aligned format
        setColorByName("bright magenta");
        gotoxy(centerX - 15, startY + 1);
        cout << " " << keys.leftForward << " - Left track forward/stop";
        gotoxy(centerX - 15, startY + 2);
        cout << " " << keys.leftBackward << " - Left track backward/stop";
        gotoxy(centerX - 15, startY + 3);
        cout << " " << keys.rightForward << " - Right track forward/stop";
        gotoxy(centerX - 15, startY + 4);
        cout << " " << keys.rightBackward << " - Right track backward/stop";
        gotoxy(centerX - 15, startY + 5);
        cout << " " << keys.stopBoth << " - Stop both tracks";
        gotoxy(centerX - 15, startY + 6);
        cout << " " << keys.shoot << " - Shoot";
        gotoxy(centerX - 15, startY + 7);
        cout << " " << keys.switchActiveTank << " - Switch Active Tank";

        // Adjust starting position for the next player
        startY += 9; // 8 lines for each player + 1 space
    }

    // Render the exit instruction with a stylish frame
    setColorByName("bright yellow");
    gotoxy(centerX - 18, startY + 1);
    cout << "  Press ESC to go back to the menu";
    resetColor();

    // Input handling
    while (true) {
        if (_kbhit()) {
            char key = tolower(_getch());
            if (key == 27) { // ESC
                system("cls");
                break;
            }
        }
    }
}




void tanksGame::printSettings()
{
    while (true) {
        system("cls");

        // Render the settings title
        setColorByName("bright yellow");
        gotoxy(25, 8);
        cout << "===== Settings =====";

        setColorByName("bright cyan");
        gotoxy(25, 10);
        cout << "(1) Number of tanks per player: " << game.getTanksPerPlayer();

        setColorByName("bright magenta");
        gotoxy(25, 11);
        cout << "(2) Colored game: " << (game.getColorMode() ? "ON" : "OFF");

        setColorByName("bright red");
        gotoxy(25, 12);
        cout << "(ESC) Back to main menu";

        resetColor();

        char key = tolower(_getch());
        switch (key) {
        case 27: // ESC
            return;
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


void tanksGame::printMapSelection() {
    system("cls");
    // Calculate the centered X position
    int centerX = 40; // Assuming a console width of 80

    // Render the map selection title
    setColorByName("bright yellow");
    gotoxy(centerX - 10, 8);
    cout << "===== Choose Map =====";

    // Map options with clear alignment
    setColorByName("bright cyan");
    gotoxy(centerX - 10, 10);
    cout << "(1) Map from file";

    setColorByName("bright magenta");
    gotoxy(centerX - 10, 11);
    cout << "(2) Random";

    setColorByName("bright red");
    gotoxy(centerX - 10, 12);
    cout << "(ESC) Back to main menu";

    resetColor();

    // Handle input for map selection
    char key = tolower(_getch());
    switch (key) {
    case 27: // ESC
        return;
    case '1':
        setMap(FILE);
        init();
        break;
    case '2':
        setMap(RANDOM);
        init();
        break;
    default:
        printMapSelection(); // Re-render the menu on invalid input
        break;
    }
}


void tanksGame::setMap(Map choice)
{
    source = choice;
}

#pragma once
#include "Game.h" 
#include "GameRecorder.h"
#include "GameLoader.h"

// Define GameMode enum here or in a common header
enum class GameMode {
    NORMAL,
    LOAD,
    SAVE,
    SILENT_LOAD
};

class tanksGame
{
private:
    enum Status { ON, OFF, PLAY, PAUSE };
    enum Map { RANDOM, FILE };
    Status mode = OFF;
    Map source;
    int const speed = 500;
    Game game;
    GameMode currentRunMode; // New member to store the current game mode
    std::string currentScreenBaseName;
    GameRecorder recorder;
    GameLoader loader;

public:
    // Update run to accept GameMode
    void run(GameMode runMode); // Modified signature
    void init();
    void gameLoop();
    void handleInput(int currentGameTime);
    void printPauseScreen();
    void handlePause();
    void setMode(Status s);
    void mainMenu();
    void printMainMenu();
    void printInstructions();
    void printSettings();
    void printMapSelection();
    void setMap(Map choice);

    // Add a way to retrieve the current game mode for other parts of the game
    GameMode getRunMode() const { return currentRunMode; }
};
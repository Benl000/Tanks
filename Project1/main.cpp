#include "tanksGame.h"
#include <string> // Add for std::string
#include <iostream> // For potential error messages

int main(int argc, char* argv[]) {
    hideCursor();

    GameMode currentMode = GameMode::NORMAL;
    bool isSilent = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i]; 
        if (arg == "-load") {
            currentMode = GameMode::LOAD;
        }
        else if (arg == "-save") {
            currentMode = GameMode::SAVE;
        }
        else if (arg == "-silent") {
            isSilent = true;
        }
        else {
            // Optional: Handle unknown arguments
            std::cerr << "Warning: Unknown command-line argument: " << arg << std::endl;
        }
    }

    if (isSilent) {
        currentMode = GameMode::SILENT_LOAD;
    }
    // -silent is ignored if present with -save, no change needed here.

    tanksGame gameInstance; 
    gameInstance.run(currentMode); // Pass the determined mode to run()

    return 0; // It's good practice to explicitly return 0
}
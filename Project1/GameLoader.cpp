// GameLoader.cpp
#include "GameLoader.h"
#include "Game.h"      // Required to interact with the Game object
#include "Player.h"    // Required to interact with Player objects
#include "Tank.h"      // Required to interact with Tank objects
#include <iostream>    // For error messages and debugging
#include <sstream>     // For std::istringstream to parse lines
#include <algorithm>   // For std::sort (if needed for result comparison, though handled in tanksGame)

// Constructor: Initializes members.
GameLoader::GameLoader() : loadedSeed(0) {
    stepsByTime.clear();
    expectedResults.clear();
}

// Helper to parse a single line from the .steps file.
StepEvent GameLoader::parseStepLine(const std::string& line) {
    StepEvent event;
    std::istringstream iss(line);
    std::string typeStr;
    iss >> typeStr;
    event.type = typeStr;

    if (typeStr == "SEED") {
        iss >> event.gameTime >> event.value; // gameTime should be 0 for SEED
    }
    else if (typeStr == "MOVE") {
        iss >> event.gameTime >> event.playerID >> event.tankID >> event.value; // value is direction
    }
    else if (typeStr == "FIRE" || typeStr == "MINE") {
        iss >> event.gameTime >> event.playerID >> event.tankID;
        event.value = 0; // Not used for FIRE/MINE
    }
    // Add more event types if your .steps format expands

    return event;
}

// Helper to parse a single line from the .result file.
ResultEvent GameLoader::parseResultLine(const std::string& line) {
    ResultEvent event;
    std::istringstream iss(line);
    std::string typeStr;
    iss >> typeStr;
    event.type = typeStr;

    if (typeStr == "HIT") {
        iss >> event.gameTime >> event.hitType >> event.hitID >> event.posX >> event.posY;
    }
    else if (typeStr == "DEAD") {
        // The format for DEAD is "DEAD <GAME_TIME> <PLAYER_ID> <TANK_ID>"
        // So we read playerID into hitID and tankID into posX, as per ResultEvent struct
        iss >> event.gameTime >> event.hitID >> event.posX; // hitID = playerID, posX = tankID
    }
    else if (typeStr == "SCORE") {
        event.gameTime = -1; // Not relevant for SCORE
        iss >> event.player1Score >> event.player2Score;
    }
    // Add more result types if your .result format expands

    return event;
}

// Loads all game data (steps and expected results) for a given screen.
bool GameLoader::loadScreenData(const std::string& screenBaseName) {
    stepsByTime.clear();
    expectedResults.clear();
    loadedSeed = 0; // Reset seed

    std::string stepsFilename = screenBaseName + ".steps";
    std::string resultsFilename = screenBaseName + ".result";

    // Load steps file
    std::ifstream stepsFile(stepsFilename);
    if (!stepsFile.is_open()) {
        std::cerr << "Error: Could not open steps file for reading: " << stepsFilename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(stepsFile, line)) {
        if (line.empty() || line[0] == '#') { // Skip empty lines and comments
            continue;
        }
        StepEvent event = parseStepLine(line);
        if (event.type == "SEED") {
            loadedSeed = static_cast<unsigned int>(event.value);
        }
        stepsByTime[event.gameTime].push_back(event);
    }
    stepsFile.close();

    // Load results file
    std::ifstream resultsFile(resultsFilename);
    if (!resultsFile.is_open()) {
        std::cerr << "Error: Could not open results file for reading: " << resultsFilename << std::endl;
        return false;
    }

    while (std::getline(resultsFile, line)) {
        if (line.empty() || line[0] == '#') { // Skip empty lines and comments
            continue;
        }
        expectedResults.push_back(parseResultLine(line));
    }
    resultsFile.close();

    return true;
}

// Applies all recorded steps that are scheduled for the given gameTime.
// This method interacts directly with your Game object.
//void GameLoader::applyStepsForCurrentTime(int gameTime, Game& game) {
//    // Check if there are any events for the current gameTime
//    auto it = stepsByTime.find(gameTime);
//    if (it != stepsByTime.end()) {
//        const std::vector<StepEvent>& events = it->second;
//        for (const auto& step : events) {
//            // Note: SEED events are handled during loadScreenData, not here.
//            if (step.type == "MOVE") {
//                if (step.playerID >= 0 && step.playerID < game.getPlayersAmount()) {
//                    Player& player = game.getPlayer(step.playerID);
//                    // Use getTanks() and index directly, assuming tankID is the vector index.
//                    // IMPORTANT: This assumes tankID corresponds to the *current* index in the vector.
//                    // If tanks are removed, indices might shift, and this could point to the wrong tank.
//                    if (step.tankID >= 0 && step.tankID < player.getTanks().size()) {
//                        Tank* tank = player.getTanks()[step.tankID].get(); // Get raw pointer from unique_ptr
//                        if (tank) {
//                            tank->direction = static_cast<Direction::Type>(step.value);
//                        }
//                    }
//                    else {
//                        std::cerr << "Warning: Tank index " << step.tankID << " out of bounds for player " << step.playerID << " at gameTime " << gameTime << std::endl;
//                    }
//                }
//                else {
//                    std::cerr << "Warning: Invalid playerID " << step.playerID << " for MOVE event at gameTime " << gameTime << std::endl;
//                }
//            }
//            else if (step.type == "FIRE") {
//                if (step.playerID >= 0 && step.playerID < game.getPlayersAmount()) {
//                    Player& player = game.getPlayer(step.playerID);
//                    // Assuming Player has a method to make their active tank fire
//                    player.fireActiveTank(game.getShells(), step.playerID); // Pass shells and playerID
//                }
//                else {
//                    std::cerr << "Warning: Invalid playerID " << step.playerID << " for FIRE event at gameTime " << gameTime << std::endl;
//                }
//            }
//            
//        }
//    }
//}


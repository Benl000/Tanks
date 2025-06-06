// GameLoader.cpp
#include "GameLoader.h"
#include "Game.h"      // Required to interact with the Game object
#include "Player.h"    // Required to interact with Player objects
#include "Tank.h"      // Required to interact with Tank objects
#include "Utils.h"     // NEW: Required for Direction::Type
#include "GameRecorder.h" // Include GameRecorder.h as GameLoader now passes it

#include <iostream>    // For error messages and debugging
#include <sstream>     // For std::istringstream to parse lines
#include <algorithm>   // For std::sort

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

    // Initialize fields to safe defaults
    event.gameTime = 0;
    event.playerID = -1;
    event.tankID = -1;
    event.directionValue = 0;
    event.isForward = false;
    event.shellX = 0;
    event.shellY = 0;
    event.leftTrack = 0;
	event.rightTrack = 0;

    if (typeStr == "SEED") {
        iss >> event.gameTime >> event.directionValue; // gameTime should be 0 for SEED, value is the seed
    }
    else if (typeStr == "MOVE") {
        // Format: MOVE <GAME_TIME> <PLAYER_ID> <TANK_ID> <FORWARD_BOOL> <DIRECTION>
        int forwardBoolInt; // Read boolean as int first
        iss >> event.gameTime >> event.playerID >> event.tankID >> forwardBoolInt >> event.directionValue;
        event.isForward = (forwardBoolInt != 0); // Convert int to bool
    }
    else if (typeStr == "ROTATE") {
        // Format: ROTATE <GAME_TIME> <PLAYER_ID> <TANK_ID> <DIRECTION>
        iss >> event.gameTime >> event.playerID >> event.tankID >> event.leftTrack >> event.rightTrack >> event.directionValue;
    }
    else if (typeStr == "FIRE") {
        // Format: FIRE <GAME_TIME> <PLAYER_ID> <TANK_ID> <DIRECTION> (No shellX, shellY in recent recordFire)
        // If your StepEvent.shellX/Y were for *reading* old format, now they aren't used for FIRE input.
        // Assuming your .steps file now only has <DIRECTION> for FIRE.
        iss >> event.gameTime >> event.playerID >> event.tankID >> event.directionValue; // Read direction for FIRE
    }
    else if (typeStr == "STOP") {
        // Format: STOP <GAME_TIME> <PLAYER_ID> <TANK_ID> <DIRECTION> (No shellX, shellY in recent recordFire)
        // If your StepEvent.shellX/Y were for *reading* old format, now they aren't used for FIRE input.
        // Assuming your .steps file now only has <DIRECTION> for FIRE.
        iss >> event.gameTime >> event.playerID >> event.tankID >> event.directionValue; // Read direction for FIRE
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

    // Initialize fields to safe defaults
    event.gameTime = 0;
    event.hitType = "";
    event.hitID = -1;
    event.posX = 0;
    event.posY = 0;
    event.player1Score = 0;
    event.player2Score = 0;

    if (typeStr == "HIT") {
        iss >> event.gameTime >> event.hitType >> event.hitID >> event.posX >> event.posY;
    }
    else if (typeStr == "DEAD") {
        // Format: DEAD <GAME_TIME> <PLAYER_ID> <TANK_ID>
        // Mapping: hitID = playerID, posX = tankID (as per previous agreement)
        iss >> event.gameTime >> event.hitID >> event.posX;
    }
    else if (typeStr == "SCORE") {
        event.gameTime = -1; // Not relevant for SCORE event
        iss >> event.player1Score >> event.player2Score;
    }
    // Add more result types if your .result format expands

    return event;
}

// Loads all game data (steps and expected results) for a given screen.
bool GameLoader::loadScreenData(const std::string& screenBaseName) {
    stepsByTime.clear();
    expectedResults.clear();
    size_t dotScreenPos = screenBaseName.find(".screen");
    std::string baseName = screenBaseName; // Default to original if not found

    // If ".screen" is found at the end, extract the substring before it
    if (dotScreenPos != std::string::npos && dotScreenPos == screenBaseName.length() - 7) {
        baseName = screenBaseName.substr(0, dotScreenPos);
    }

    std::string stepsFilename = baseName + ".steps";
    std::string resultsFilename = baseName + ".result";
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
            loadedSeed = static_cast<unsigned int>(event.directionValue); // directionValue holds the seed
        }
        stepsByTime[event.gameTime].push_back(event);
    }
    stepsFile.close();

    // --- End of stepsByTime printing section ---


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
void GameLoader::applyStepsForCurrentTime(int gameTime, Game& game) {
    // Check if there are any events for the current gameTime
    auto it = stepsByTime.find(gameTime);
    if (it != stepsByTime.end()) {
        const std::vector<StepEvent>& events = it->second;
        for (const auto& step : events) {
			if (step.type == "SEED") {
				continue;
			}
            else if (step.type == "FIRE") {
                if (step.playerID >= 0 && step.playerID < game.getPlayersAmount()) {
                    Player& player = game.getPlayer(step.playerID);
                    Tank* targetTank = player.getTanks()[step.tankID].get();

                    if (targetTank) {
                        // Make the tank shoot, passing the recorded time and no recorder (as it's replay)
                        targetTank->shoot(game.getShells(), step.playerID, step.tankID, step.gameTime, nullptr);
                    }
                    else {
                        std::cerr << "Warning: Tank index " << step.tankID << " not found for player " << step.playerID << " to fire at gameTime " << gameTime << std::endl;
                    }
                }
                else {
                    std::cerr << "Warning: Invalid playerID " << step.playerID << " for FIRE event at gameTime " << gameTime << std::endl;
                }
            }
            // Note: SEED events are handled during loadScreenData, not here.
            if (step.type == "MOVE") {
                if (step.playerID >= 0 && step.playerID < game.getPlayersAmount()) {
                    Player& player = game.getPlayer(step.playerID);
                    if (step.tankID >= 0 && step.tankID < player.getTanks().size()) {
                        Tank* tank = player.getTanks()[step.tankID].get();
                        if (tank) {
                            // Apply movement direction
                            tank->direction = static_cast<Direction::Type>(step.directionValue);
                            // Assuming cannon.update() also correctly updates cannon's direction based on tank->direction
                            tank->cannon.update();

                            // Apply forward/backward state using setBothTracks
                            if (step.isForward) {
                                tank->setBothTracks(Tank::TrackState::FORWARD);
                            }
                            else {
                                tank->setBothTracks(Tank::TrackState::BACKWARD);
                            }
                        }
                    }
                    else {
                        std::cerr << "Warning: Tank index " << step.tankID << " out of bounds for player " << step.playerID << " at gameTime " << gameTime << std::endl;
                    }
                }
                else {
                    std::cerr << "Warning: Invalid playerID " << step.playerID << " for MOVE event at gameTime " << gameTime << std::endl;
                }
            }
            else if (step.type == "ROTATE") {
                if (step.playerID >= 0 && step.playerID < game.getPlayersAmount()) {
                    Player& player = game.getPlayer(step.playerID);
                    if (step.tankID >= 0 && step.tankID < player.getTanks().size()) {
                        Tank* tank = player.getTanks()[step.tankID].get();
                        if (tank) {
                            //// Apply rotation to tank body and cannon
                            //tank->direction = static_cast<Direction::Type>(step.directionValue);
                            //tank->cannon.update(); // Keep cannon aligned
							if (step.leftTrack == 0) {
								tank->setLeftTrack(Tank::TrackState::STOPPED);
							}
							else if (step.leftTrack == 1) {
								tank->setLeftTrack(Tank::TrackState::FORWARD);
							}
							else if (step.leftTrack == -1) {
								tank->setLeftTrack(Tank::TrackState::BACKWARD);
							}
							if (step.rightTrack == 0) {
								tank->setRightTrack(Tank::TrackState::STOPPED);
							}
							else if (step.rightTrack == 1) {
								tank->setRightTrack(Tank::TrackState::FORWARD);
							}
							else if (step.rightTrack == -1) {
								tank->setRightTrack(Tank::TrackState::BACKWARD);
							}
							

                        }
                    }
                    else {
                        std::cerr << "Warning: Tank index " << step.tankID << " out of bounds for player " << step.playerID << " for ROTATE event at gameTime " << gameTime << std::endl;
                    }
                }
                else {
                    std::cerr << "Warning: Invalid playerID " << step.playerID << " for ROTATE event at gameTime " << gameTime << std::endl;
                }
            }
            
            else if (step.type == "STOP") { // NEW: Handle STOP event
                if (step.playerID >= 0 && step.playerID < game.getPlayersAmount()) {
                    Player& player = game.getPlayer(step.playerID);
                    if (step.tankID >= 0 && step.tankID <= player.getTanks().size()) {
                        Tank* tank = player.getTanks()[step.tankID].get();
                        if (tank) {
                            // When a STOP event is replayed, the tank's tracks should be set to STOPPED.
                            // The direction might be implicitly set to the tank's current direction,
                            // or it might be explicitly defined in the STOP event if it implies final orientation.
                            // Assuming it sets tracks to STOPPED and keeps current direction.
                            tank->setBothTracks(Tank::TrackState::STOPPED);
                            // If the STOP event includes a specific direction to face when stopped:
                            // tank->direction = static_cast<Direction::Type>(step.directionValue);
                            // tank->cannon.update();
                        }
                    }
                    else {
                        std::cerr << "Warning: Tank index " << player.getTanks().size()<< " " << step.tankID << " out of bounds for player " << step.playerID << " for STOP event at gameTime " << gameTime << std::endl;
                    }
                }
                else {

                    std::cerr << "Warning: Invalid playerID " << step.playerID << " " << step.type << " for STOP event at gameTime " << gameTime << std::endl;
                }
            }
            // Add 'MINE', 'HIT', 'DEAD' event types here if you implement them for replay (though HIT/DEAD are usually results)
        }
    }
}

int GameLoader::getMaxGameTime() const {
    if (stepsByTime.empty()) {
        return 0; // or return -1 to indicate "no steps"
    }
    return stepsByTime.rbegin()->first; // rbegin() gives the last (largest) key in the map
}


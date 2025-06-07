#ifndef GAME_LOADER_H
#define GAME_LOADER_H

#include <fstream>   // Required for std::ifstream
#include <string>    // Required for std::string
#include <vector>    // Required for std::vector
#include <map>       // Required for std::map

// Forward declaration of Game class to avoid circular includes
// GameLoader needs to interact with Game, but Game also uses GameLoader structs.
class Game;

// --- Structs to hold parsed game data ---

// Represents a single step event from the .steps file.
struct StepEvent {
    std::string type;    // "SEED", "MOVE", "ROTATE", "FIRE"
    int gameTime;        // The game iteration when this event occurs

    int playerID;        // Common for MOVE, ROTATE, FIRE
    int tankID;          // Common for MOVE, ROTATE, FIRE

    // For SEED: stores the seed value
    // For MOVE/ROTATE: stores the direction value
    int directionValue;

    // NEW: For MOVE: indicates if the movement is forward (1) or not (0)
    // Could also be used for other boolean flags in future.
    bool isForward;

    // For FIRE: stores shellX, shellY (re-using existing fields)
    int shellX;
    int shellY;
    int leftTrack;
	int rightTrack; 

};

// Represents a single result event from the .result file.
// (This struct remains unchanged from previous discussions)
struct ResultEvent {
    std::string type;    // "HIT", "DEAD", "SCORE"
    int gameTime;        // The game iteration when this event occurs (not used for SCORE)

    // Data specific to the event type.
    std::string hitType; // For "HIT": "TANK", "WALL", "CANNON", etc.
    int hitID;           // For "HIT": ID of the hit object, or -1 if not applicable
    int posX, posY;      // For "HIT": Coordinates of the hit
    // For "DEAD": hitID = playerID, posX = tankID (as per GameLoader.cpp parse logic)
    int player1Score;    // For "SCORE": Player 1's final score
    int player2Score;    // For "SCORE": Player 2's final score
};

// GameLoader class definition
class GameLoader {
private:
    std::map<int, std::vector<StepEvent>> stepsByTime;
    std::vector<ResultEvent> expectedResults;
    unsigned int loadedSeed;

    // Helper function to parse a single line from the .steps file into a StepEvent struct.
    StepEvent parseStepLine(const std::string& line);
    // Helper function to parse a single line from the .result file into a ResultEvent struct.
    ResultEvent parseResultLine(const std::string& line);

public:
    // Constructor
    GameLoader();

    // Loads all game data (steps and expected results) for a given screen.
    bool loadScreenData(const std::string& screenBaseName);

    // Returns the random seed loaded from the .steps file.
    unsigned int getLoadedSeed() const { return loadedSeed; }

    // Applies all recorded steps that are scheduled for the given gameTime.
    void applyStepsForCurrentTime(int gameTime, Game& game); // Pass Game object by reference

    // Returns a const reference to the vector of expected result events.
    const std::vector<ResultEvent>& getExpectedResults() const { return expectedResults; }

};

#endif // GAME_LOADER_H
#ifndef GAME_LOADER_H
#define GAME_LOADER_H

#include <fstream>   // Required for std::ifstream
#include <string>    // Required for std::string
#include <vector>    // Required for std::vector
#include <map>       // Required for std::map

// --- Structs to hold parsed game data ---

// Represents a single step event from the .steps file.
struct StepEvent {
    std::string type;    // "SEED", "MOVE", "FIRE", "MINE"
    int gameTime;        // The game iteration when this event occurs

    // Data specific to the event type.
    // We use a union or a combination of fields for flexibility,
    // though a more object-oriented approach might use polymorphism.
    // For simplicity, we'll store all possible fields and use them based on 'type'.
    int playerID;
    int tankID;
    int value;           // Can be seed value, direction, or unused
};

// Represents a single result event from the .result file.
struct ResultEvent {
    std::string type;    // "HIT", "DEAD", "SCORE"
    int gameTime;        // The game iteration when this event occurs (not used for SCORE)

    // Data specific to the event type.
    std::string hitType; // For "HIT": "TANK", "WALL", "CANNON", etc.
    int hitID;           // For "HIT": ID of the hit object, or -1 if not applicable
    int posX, posY;      // For "HIT": Coordinates of the hit
    int player1Score;    // For "SCORE": Player 1's final score
    int player2Score;    // For "SCORE": Player 2's final score
};

// GameLoader class definition
class GameLoader {
private:
    // Stores all loaded step events, organized by their gameTime.
    // Using a map allows quick lookup of events for the current gameTime.
    std::map<int, std::vector<StepEvent>> stepsByTime;

    // Stores all expected result events loaded from the .result file.
    std::vector<ResultEvent> expectedResults;

    unsigned int loadedSeed; // The random seed loaded from the .steps file.

    // Helper function to parse a single line from the .steps file into a StepEvent struct.
    StepEvent parseStepLine(const std::string& line);
    // Helper function to parse a single line from the .result file into a ResultEvent struct.
    ResultEvent parseResultLine(const std::string& line);

public:
    // Constructor
    GameLoader();

    // Loads all game data (steps and expected results) for a given screen.
    // screenBaseName: The base name of the screen (e.g., "tanks-game_01").
    // Returns true on successful loading, false otherwise (e.g., files not found).
    bool loadScreenData(const std::string& screenBaseName);

    // Returns the random seed loaded from the .steps file.
    unsigned int getLoadedSeed() const { return loadedSeed; }

    // Applies all recorded steps that are scheduled for the given gameTime.
    // This method will interact with your game's internal objects (tanks, etc.)
    // to simulate player actions. You'll need to pass references to these objects.
    // Example: applyStepsForCurrentTime(gameTime, game.getTanks(), game.getPlayers());
    //void applyStepsForCurrentTime(int gameTime, Game &game);

    // Returns a const reference to the vector of expected result events.
    // Used by the silent mode for comparison.
    const std::vector<ResultEvent>& getExpectedResults() const { return expectedResults; }
};

#endif // GAME_LOADER_H
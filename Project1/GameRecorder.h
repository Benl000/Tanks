#ifndef GAME_RECORDER_H
#define GAME_RECORDER_H

#include <fstream>   // Required for std::ofstream
#include <string>    // Required for std::string

// GameRecorder class definition
class GameRecorder {
private:
    std::ofstream stepsFile;    // File stream for .steps file
    std::ofstream resultsFile;  // File stream for .results file
    std::string currentScreenBaseName; // Stores the base name (e.g., "tanks-game_01")
    bool isRecordingEnabled;    // NEW: Flag to control if recording is active at all

public:
    // Constructor and Destructor
    GameRecorder();
    ~GameRecorder(); // Ensures files are closed when GameRecorder object is destroyed

    // NEW: Method to enable or disable recording functionality
    void setRecordingEnabled(bool enable);

    // NEW: Method to check if recording is currently enabled and files are open
    bool isRecordingActive() const;

    // Initializes recording for a new screen.
    // Opens the .steps and .results files with the correct naming convention.
    // Writes the initial random seed to the .steps file.
    // Returns true on success, false otherwise (e.g., file open failed).
    // If isRecordingEnabled is false, this method will return true immediately without opening files.
    bool startRecording(const std::string& screenBaseName, unsigned int seed, int gameTime);

    // Records a tank cannon rotation event.
    void recordRotate(int gameTime, int playerID, int tankID, int direction);

    // Records a tank movement event (body and cannon).
    void recordMove(int gameTime, int playerID, int tankID, int direction, bool forward);

    // Records a tank firing a shell event.
    void recordFire(int gameTime, int playerID, int tankID, int direction);

    // Records a shell hitting something event.
    void recordHit(int gameTime, const std::string& hitType, int hitID, int x, int y);

    // Records a tank being destroyed event.
    void recordDead(int gameTime, int playerID, int tankIDm, int how);

    // Records the final scores for both players. This should be the last entry in the results file.
    void recordScores(int player1Score, int player2Score);

    // Stops recording and closes the files.
    void stopRecording();
};

#endif // GAME_RECORDER_H
#include "GameRecorder.h"
#include <iostream> // For error messages
#include <iomanip>  // For std::setw, std::left

// Constructor: Initializes file streams to a closed state and disables recording by default.
GameRecorder::GameRecorder() : stepsFile(), resultsFile(), isRecordingEnabled(false) {
    // Recording is disabled until explicitly enabled by setRecordingEnabled(true)
}

// Destructor: Ensures files are closed when the object is destroyed.
GameRecorder::~GameRecorder() {
    stopRecording(); // Call stopRecording to close any open files
}

// Method to enable or disable recording functionality
void GameRecorder::setRecordingEnabled(bool enable) {
    isRecordingEnabled = enable;
    if (!enable) {
        // If disabling, also ensure files are closed
        stopRecording();
    }
}

void GameRecorder::setIsSilentMode(bool enable) {
    isSilentMode = enable;
    if (!enable) {
        // If disabling, also ensure files are closed
        stopRecording();
    }
}

bool GameRecorder::getIsSilentMode()
{
    return isSilentMode;
}

// Method to check if recording is currently active (enabled AND files are open)
// This remains useful for external checks if you need to know if actual writing can occur.
bool GameRecorder::isRecordingActive() const {
    return isRecordingEnabled && stepsFile.is_open() && resultsFile.is_open();
}

// Starts recording for a new screen.
// If recording is not enabled, it does nothing and returns true.
bool GameRecorder::startRecording(const std::string& screenBaseName, unsigned int seed, int gameTime) {
    if (!isRecordingEnabled) {
        // If recording is disabled, don't open files, just report success.
        return true;
    }

    // If recording is already active (files are open), stop it first
    if (stepsFile.is_open() || resultsFile.is_open()) {
        stopRecording();
    }

    currentScreenBaseName = screenBaseName;
    size_t dotScreenPos = currentScreenBaseName.find(".screen");
    std::string baseName = currentScreenBaseName; // Default to original if not found

    // If ".screen" is found at the end, extract the substring before it
    if (dotScreenPos != std::string::npos && dotScreenPos == currentScreenBaseName.length() - 7) {
        baseName = currentScreenBaseName.substr(0, dotScreenPos);
    }

    if (!getIsSilentMode())
    {
        std::string stepsFilename = baseName + ".steps";

        // Open steps file in truncate mode (overwrites existing file)
        stepsFile.open(stepsFilename, std::ios::out | std::ios::trunc);
        if (!stepsFile.is_open()) {
            std::cerr << "Error: Could not open steps file for writing: " << stepsFilename << std::endl;
            return false; // Indicate failure to open file
        }
    }
    std::string resultsFilename = baseName + ".result";
    // Open results file in truncate mode (overwrites existing file)
    resultsFile.open(resultsFilename, std::ios::out | std::ios::trunc);
    if (!resultsFile.is_open()) {
        std::cerr << "Error: Could not open results file for writing: " << resultsFilename << std::endl;
        stepsFile.close(); // Close steps file if results file fails
        return false; // Indicate failure to open file
    }

    // Write the initial SEED event to the steps file
    stepsFile << "SEED " << gameTime << " " << seed << std::endl;

    return true; // Indicate success (files opened and seed written)
}

// Records a tank cannon rotation event.
void GameRecorder::recordRotate(int gameTime, int playerID, int tankID, int leftForward, int rightForward, int direction) {
    if (isRecordingEnabled && (!isSilentMode)) { // Only check the flag
        if (stepsFile.is_open()) { // Still good to check if file is truly open before writing
            stepsFile << "ROTATE " << gameTime << " " << playerID << " " << tankID << " " << leftForward << " " << rightForward << " " << direction << std::endl;
        }
        else {
            // This warning is now more important, as isRecordingEnabled is true but file is not open
            std::cerr << "Warning: Steps file not open, cannot record rotation. (Recording enabled, but file failed to open previously?)" << std::endl;
        }
    }
}

// Records a tank movement event (body and cannon).
void GameRecorder::recordMove(int gameTime, int playerID, int tankID, bool forward, int direction) {
    if (isRecordingEnabled && (!isSilentMode)) { // Only check the flag
        if (stepsFile.is_open()) {
            stepsFile << "MOVE " << gameTime << " " << playerID << " " << tankID << " " << forward << " " << direction << std::endl;
        }
        else {
            std::cerr << "Warning: Steps file not open, cannot record move. (Recording enabled, but file failed to open previously?)" << std::endl;
        }
    }
}

void GameRecorder::recordStop(int gameTime, int playerID, int tankID, int direction) {
    if (isRecordingEnabled && (!isSilentMode)) { // Only check the flag
        if (stepsFile.is_open()) {
            stepsFile << "STOP " << gameTime << " " << playerID << " " << tankID << " " << direction << std::endl;
        }
        else {
            std::cerr << "Warning: Steps file not open, cannot record move. (Recording enabled, but file failed to open previously?)" << std::endl;
        }
    }
}

// Records a tank firing a shell event.
void GameRecorder::recordFire(int gameTime, int playerID, int tankID, int direction) {
    if (isRecordingEnabled && (!isSilentMode)) { // Only check the flag
        if (stepsFile.is_open()) {
            stepsFile << "FIRE " << gameTime << " " << playerID << " " << tankID << " " << direction  << std::endl;
        }
        else {
            std::cerr << "Warning: Steps file not open, cannot record fire. (Recording enabled, but file failed to open previously?)" << std::endl;
        }
    }
}

// Records a shell hitting something event.
void GameRecorder::recordHit(int gameTime, const std::string& hitType, int hitID, int x, int y) {
    if (isSilentMode) {
        if (resultsFile.is_open()) {
            resultsFile << "HIT " << gameTime << " " << hitType << " " << hitID << " " << x << " " << y << std::endl;
        }
    }
    if (isRecordingEnabled && (!isSilentMode)) { // Only check the flag
        if (resultsFile.is_open() and stepsFile.is_open()) {
            resultsFile << "HIT " << gameTime << " " << hitType << " " << hitID << " " << x << " " << y << std::endl;
            stepsFile << "HIT " << gameTime << " " << hitType << " " << hitID << " " << x << " " << y << std::endl;
        }
        else {
            std::cerr << "Warning: Results file not open, cannot record hit. (Recording enabled, but file failed to open previously?)" << std::endl;
        }
    }
}

// Records a tank being destroyed event.
void GameRecorder::recordDead(int gameTime, int playerID, int tankID, int how) {
    if (isSilentMode) {
        if (resultsFile.is_open()) {
            resultsFile << "DEAD " << gameTime << " " << playerID << " " << tankID << " " << how << std::endl; //0 for tank 1 for mine
        }
    }
    if (isRecordingEnabled && (!isSilentMode)) { // Only check the flag
        if (resultsFile.is_open() and stepsFile.is_open()) {
            resultsFile << "DEAD " << gameTime << " " << playerID << " " << tankID << " " << how << std::endl; //0 for tank 1 for mine
            stepsFile << "DEAD " << gameTime << " " << playerID << " " << tankID << " " << how << std::endl; //0 for tank 1 for mine
        }
        else {
            std::cerr << "Warning: Results file not open, cannot record dead tank. (Recording enabled, but file failed to open previously?)" << std::endl;
        }
    }
}

// Records the final scores for both players.
void GameRecorder::recordScores(int player1Score, int player2Score) {
    if (isSilentMode) {
        if (resultsFile.is_open()) {
            resultsFile << "SCORE " << player1Score << " " << player2Score << std::endl;
        }
    }
    if (isRecordingEnabled && (!isSilentMode)) { // Only check the flag
        if (resultsFile.is_open()) {
            resultsFile << "SCORE " << player1Score << " " << player2Score << std::endl;
        }
        else {
            std::cerr << "Warning: Results file not open, cannot record scores. (Recording enabled, but file failed to open previously?)" << std::endl;
        }
    }
}

// Stops recording and closes both file streams.
void GameRecorder::stopRecording() {
    // We still check isRecordingEnabled here, as the setRecordingEnabled(false) call path also uses it.
    // If it was already disabled, there's no need to try closing files.
    if (!isRecordingEnabled) {
        return;
    }

    if (stepsFile.is_open()) {
        stepsFile.close();
    }
    if (resultsFile.is_open()) {
        resultsFile.close();
    }
}
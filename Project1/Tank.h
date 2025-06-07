#pragma once
#include "Cannon.h"
#include "GameRecorder.h"
#include "Utils.h"
#include "Shell.h"
#include <string>
#include <vector>
using std::cout;
using std::endl;
using std::string;
using std::vector;


class Tank
{
public:
    enum TrackState { STOPPED = 0, FORWARD = 1, BACKWARD = -1 };
    enum RotationDirection { CLOCKWISE, COUNTER_CLOCKWISE };
    enum RotationSpeed { NONE = 0, REGULAR = 1, DOUBLE = 2 };
    Cannon cannon;

private:
    int x, y, shootCooldown = 0;
    TrackState leftTrack = STOPPED;
    TrackState rightTrack = STOPPED;
    string color;
    char symbol = 'O';
    char activeSymbol = '0';

    

public:
    Tank(int x, int y, Direction::Type direction,string color);
    int getX() const;
    int getY() const;
    int getMoveType() const;
    Direction::Type direction;
    Direction::Type getDirection() const;
    void render();
    void setLeftTrack(TrackState state);
    void setRightTrack(TrackState state);
    void setBothTracks(TrackState state);
    void move();
    void shoot(vector<Shell>& gameShells, int playerID, int tankIndex, int currentGameTime, GameRecorder * recorderrecorder = nullptr);
    void reduceCoolDown();
    int getMovementType();
    bool isStopped();
    Cannon& getCannon();
    TrackState getLeftTrack() const;
    TrackState getRightTrack() const;
    vector<int> calculateMovementOffset(int moveType);
    void drive(TrackState direction);
    Direction::Type rotate(RotationSpeed speed, RotationDirection dir, bool applyRotation);
    bool isActive = false; // Flag to indicate if this is the active tank
    void setActive(bool active); // New function to set the flag


    
};

#pragma once
#include "Cannon.h"
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
    Direction::Type direction;
    TrackState leftTrack = STOPPED;
    TrackState rightTrack = STOPPED;
    string color;
    char symbol = 'O';

public:
    Tank(int x, int y, Direction::Type direction,string color);
    int getX() const;
    int getY() const;
    Direction::Type getDirection() const;
    void render();
    void setLeftTrack(TrackState state);
    void setRightTrack(TrackState state);
    void setBothTracks(TrackState state);
    void move();
    void shoot(vector<Shell>& gameShells, int playerID);
    void reduceCoolDown();
    int getMovementType();
    bool isStopped();
    Cannon& getCannon();
    TrackState getLeftTrack() const;
    TrackState getRightTrack() const;
    vector<int> calculateMovementOffset(int moveType);
    void drive(TrackState direction);
    Direction::Type rotate(RotationSpeed speed, RotationDirection dir, bool applyRotation);

    
};

#pragma once
#include "Cannon.h"
#include "Utils.h"
#include <string>
#include <vector>
using namespace std;

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
    void move();
    int whichMove();
    bool isStopped();
    Cannon& getCannon();
    TrackState getLeftTrack() const;
    TrackState getRightTrack() const;
    vector<int> driveXY(int moveType);
    Direction::Type rotateCheak(RotationSpeed speed, RotationDirection dir);
    
private:
    void drive(TrackState direction);
    void rotate(RotationSpeed speed, RotationDirection dir);

    
};

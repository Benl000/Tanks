#pragma once
#include "Cannon.h"
#include "Utils.h"

class Tank
{
public:
    enum TrackState { STOPPED = 0, FORWARD = 1, BACKWARD = -1 };
    enum RotationDirection { CLOCKWISE, COUNTER_CLOCKWISE };
    enum RotationSpeed { NONE = 0, REGULAR = 1, DOUBLE = 2 };

private:
    int x, y, prevX, prevY, shootCooldown = 0;
    Direction::Type direction;
    TrackState leftTrack = STOPPED;
    TrackState rightTrack = STOPPED;
    Cannon cannon;
    char symbol = 'O';

public:
    Tank(int x, int y, Direction::Type direction);
    int getX() const;
    int getY() const;
    Direction::Type getDirection() const;
    void render();
    void setLeftTrack(TrackState state);
    void setRightTrack(TrackState state);
    void move();
    TrackState getLeftTrack() const;
    TrackState getRightTrack() const;

private:
    void drive(TrackState direction);
    void rotate(RotationSpeed speed, RotationDirection dir);

    
};

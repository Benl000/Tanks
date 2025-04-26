#pragma once
#include "Cannon.h"
#include "Utils.h"

class Tank
{
private:
    int x, y, shootCooldown = 0;
    enum TrackState { STOPPED = 0, FORWARD = 1, BACKWARD = -1 };
    enum RotationDirection { CLOCKWISE, COUNTER_CLOCKWISE };
    enum RotationSpeed { NONE = 0, REGULAR = 1, DOUBLE = 2 };
    Direction::Type direction;
    TrackState leftTrack = STOPPED;
    TrackState rightTrack = STOPPED;
    Cannon cannon;
    char symbol = 'O';

public:
    Tank(int x, int y, Direction::Type direction);
    int getX() const;
    int getY() const;
    void render() const;
    Direction::Type getDirection() const;
};

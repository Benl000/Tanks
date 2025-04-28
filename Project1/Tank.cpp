#include "Tank.h"
#include "Utils.h"
#include <iostream>
using namespace std;

Tank::Tank(int x, int y, Direction::Type direction)
    : x(x), y(y), direction(direction), cannon(this) {
}

int Tank::getX() const { return x; }

int Tank::getY() const { return y; }

Direction::Type Tank::getDirection() const { return direction; }

void Tank::render() {
    gotoxy(x, y);
    cout << symbol;
    cannon.render();
}

void Tank::setLeftTrack(TrackState state) {
    leftTrack = state;
}

void Tank::setRightTrack(TrackState state) {
    rightTrack = state;
}

void Tank::move() {
    if (leftTrack == STOPPED && rightTrack == STOPPED)
        return;

    if (leftTrack == rightTrack) {
        if (leftTrack == FORWARD)
            drive(FORWARD);
        else
            drive(BACKWARD);
    }
    else if (leftTrack == FORWARD && rightTrack == BACKWARD) {
        rotate(DOUBLE, CLOCKWISE);
    }
    else if (leftTrack == BACKWARD && rightTrack == FORWARD) {
        rotate(DOUBLE, COUNTER_CLOCKWISE);
    }
    else if (leftTrack == FORWARD || rightTrack == BACKWARD) {
        rotate(REGULAR, CLOCKWISE);
    }
    else if (leftTrack == BACKWARD || rightTrack == FORWARD) {
        rotate(REGULAR, COUNTER_CLOCKWISE);
    }
}

void Tank::drive(TrackState direction) {
    int dx = 0, dy = 0;
    switch (this->direction) {
    case Direction::U: dy = -1; break;
    case Direction::D: dy = 1; break;
    case Direction::L: dx = -1; break;
    case Direction::R: dx = 1; break;
    case Direction::UL: dx = -1; dy = -1; break;
    case Direction::UR: dx = 1; dy = -1; break;
    case Direction::DL: dx = -1; dy = 1; break;
    case Direction::DR: dx = 1; dy = 1; break;
    }
    if (direction == BACKWARD) {
        dx = -dx;
        dy = -dy;
    }
    x += dx;
    y += dy;
}

void Tank::rotate(RotationSpeed speed, RotationDirection dir) {
    int rotationAmount = (speed == DOUBLE) ? 2 : 1;
    int d = static_cast<int>(direction);
    if (dir == CLOCKWISE)
        d = (d + rotationAmount) % 8;
    else
        d = (d - rotationAmount + 8) % 8;
    direction = static_cast<Direction::Type>(d);
}

Tank::TrackState Tank::getLeftTrack() const {
    return leftTrack;
}

Tank::TrackState Tank::getRightTrack() const {
    return rightTrack;
}


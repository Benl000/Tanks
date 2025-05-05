#include "Tank.h"
#include "Utils.h"
#include <iostream>
#include <vector>
using namespace std;

Tank::Tank(int x, int y, Direction::Type direction,string color)
    : x(x), y(y), direction(direction), cannon(this),color(color) {
}

int Tank::getX() const { return x; }
int Tank::getY() const { return y; }
Direction::Type Tank::getDirection() const { return direction; }
Tank::TrackState Tank::getLeftTrack() const { return leftTrack; }
Tank::TrackState Tank::getRightTrack() const { return rightTrack; }

void Tank::setLeftTrack(TrackState state) {
    leftTrack = state;
}

void Tank::setRightTrack(TrackState state) {
    rightTrack = state;
}

int Tank::whichMove() {
    if (isStopped()) {
        return 0;
    }

    if (leftTrack == rightTrack) {
        if (leftTrack == FORWARD) {
            return 1;
        }
        else {
            return -1;
        }

    }
    else if (leftTrack == FORWARD && rightTrack == BACKWARD) {
        return 2;
    }
    else if (leftTrack == BACKWARD && rightTrack == FORWARD) {
        return 3;
    }
    else if (leftTrack == FORWARD || rightTrack == BACKWARD) {
        return 4;
    }
    else if (leftTrack == BACKWARD || rightTrack == FORWARD) {
        return 5;
    }

    return 0;
}


vector<int> Tank::driveXY(int moveType)
{
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
    if (moveType == -1) {
        dx = -dx;
        dy = -dy;
    }

    std::vector<int> newPosition;
    newPosition.push_back(x + dx); // First element is the new X
    newPosition.push_back(y + dy); // Second element is the new Y
    newPosition.push_back(x + (dx*2)); // First element of cannon is the new X
    newPosition.push_back(y + (dy*2)); // Second element of cannon is the new Y
    return newPosition;
}

void Tank::move() {
    if (isStopped()) {
        return;
    }

    if (leftTrack == rightTrack) {
        if (leftTrack == FORWARD) {
            drive(FORWARD);
        }
        else {
            drive(BACKWARD);
        }
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

    cannon.update(); // Update cannon after move
}

bool Tank::isStopped()
{
    return (leftTrack == STOPPED && rightTrack == STOPPED);
}

Cannon& Tank::getCannon()
{
    return cannon;
}

void Tank::render() {

    setColorByName(color);

    // Draw tank
    gotoxy(x, y);
    cout << symbol;

    // Draw cannon
    cannon.render();

    resetColor();
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

    wrapCoordinates(x, y);
}

Direction::Type Tank::rotateCheak(RotationSpeed speed, RotationDirection dir) {
    int steps = (speed == DOUBLE) ? 2 : 1;

    int d = static_cast<int>(direction);
    if (dir == CLOCKWISE)
        d = (d + steps) % 8;
    else
        d = (d - steps + 8) % 8;

    return static_cast<Direction::Type>(d);

}

void Tank::rotate(RotationSpeed speed, RotationDirection dir) {
    int steps = (speed == DOUBLE) ? 2 : 1;

    int d = static_cast<int>(direction);
    if (dir == CLOCKWISE)
        d = (d + steps) % 8;
    else
        d = (d - steps + 8) % 8;

    direction = static_cast<Direction::Type>(d);

}



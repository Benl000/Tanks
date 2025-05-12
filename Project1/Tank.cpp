#include "Tank.h"
#include "Utils.h"
#include <iostream>
#include <vector>
#include "Shell.h"
#include "Cannon.h"
using std::cout;
using std::endl;
using std::string;
using std::vector;


Tank::Tank(int x, int y, Direction::Type direction,string color)
    : x(x), y(y), direction(direction), cannon(this),color(color) {
    cannon.update();
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

void Tank::setBothTracks(TrackState state)
{
    setLeftTrack(state);
    setRightTrack(state);
}

int Tank::getMovementType() {
    if (isStopped()) {
        return 0;
    }

    if (leftTrack == rightTrack)
        return leftTrack == FORWARD ? 1 : -1;
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

vector<int> Tank::calculateMovementOffset(int moveType)
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

    vector<int> newPosition;
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
        rotate(DOUBLE, CLOCKWISE,true);
    }
    else if (leftTrack == BACKWARD && rightTrack == FORWARD) {
        rotate(DOUBLE, COUNTER_CLOCKWISE,true);
    }
    else if (leftTrack == FORWARD || rightTrack == BACKWARD) {
        rotate(REGULAR, CLOCKWISE,true);
    }
    else if (leftTrack == BACKWARD || rightTrack == FORWARD) {
        rotate(REGULAR, COUNTER_CLOCKWISE,true);
    }

    cannon.update(); // Update cannon after move
}

void Tank::shoot(vector<Shell>& gameShells, int playerID) {
    if (shootCooldown > 0) return;
    if (cannon.getCondition() == Cannon::Condition::BROKEN)return;
    shootCooldown = 5;
    int cx = cannon.getX();
    int cy = cannon.getY();

    Shell newShell(cx, cy, direction, playerID);
    gameShells.push_back(newShell);
}

void Tank::reduceCoolDown()
{
    if (shootCooldown <= 0) return;
    shootCooldown--;
}

bool Tank::isStopped()
{
    return (leftTrack == STOPPED && rightTrack == STOPPED);
}

Cannon& Tank::getCannon()
{
    return cannon;
}

// Set active flag
void Tank::setActive(bool active) {
    isActive = active;
    render();
}

// Render function (no parameter change)
void Tank::render() {
    setColorByName(color);

    gotoxy(x, y);
    if (isActive) {
        cout << activeSymbol;
    }
    else {
        cout << symbol;
    }


    // Draw cannon if not broken
    if (cannon.getCondition() == Cannon::Condition::FIXED) {
        cannon.render();
    }
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

Direction::Type Tank::rotate(RotationSpeed speed, RotationDirection dir, bool applyRotation = true) {
    int steps = (speed == DOUBLE) ? 2 : 1;
    int d = static_cast<int>(direction);

    if (dir == CLOCKWISE)
        d = (d + steps) % 8;
    else
        d = (d - steps + 8) % 8;

    if (applyRotation) {
        direction = static_cast<Direction::Type>(d);
    }

    return static_cast<Direction::Type>(d);
}
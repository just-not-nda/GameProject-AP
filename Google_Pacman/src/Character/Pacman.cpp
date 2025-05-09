#include "Pacman.h"

typedef pair<int, pair<int, int> > TurnID;

Pacman::Pacman() : Character(PACMAN_START_TILE_X, PACMAN_START_TILE_Y) {
    while (!Direction.empty()) Direction.pop();
    while (!turnPoints.empty())   turnPoints.pop();
}

void Pacman::updateDir(int newDir) {
    if (!Direction.empty()) Direction.pop();
    Direction.push(newDir);
}

void Pacman::addTurn(int newDir, pair<int, int> nextCross) {
    if (!turnPoints.empty()) {
        if (turnPoints.top().first != newDir) {
            turnPoints.pop();
            turnPoints.push( TurnID(newDir, nextCross) );
        }
    }
    else turnPoints.push( TurnID(newDir,nextCross) );
}

void Pacman::moving() {
    if (!Direction.empty()) {
        int veloX = 0, veloY = 0, dir = -1;

        switch (Direction.top()) {
            case UP   : {
                veloX = 0;
                veloY = -pacmanVelocity;
                dir = 0; break; }
            case DOWN : {
                veloX = 0;
                veloY =  pacmanVelocity;
                dir = 2; break; }
            case LEFT : {
                veloX = -pacmanVelocity;
                veloY = 0;
                dir = 3; break; }
            case RIGHT: {
                veloX =  pacmanVelocity;
                veloY = 0;
                dir = 1; break; }
        }
        changeVelocityDir(veloX, veloY, dir);
        go();
    }
}

void Pacman::turn() {
    if (!Direction.empty()) stopmoving();
    Direction.push(turnPoints.top().first);
    turnPoints.pop();
    moving();
}

void Pacman::stopmoving() {
    while (!Direction.empty()) Direction.pop();
}

void Pacman::eraseTurnPoints() {
    while (!turnPoints.empty()) turnPoints.pop();
}


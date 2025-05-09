#include "Ghost.h"


Ghost::Ghost(int tileX, int tileY, bool atHome) : Character(tileX, tileY) {
    frighten = 0;
    speedMode = 1;
    ghostVelocity = 2;
    scattering = false;
    nextTileX = tileX, nextTileY = tileY;
    this->atHome = atHome;
    if (atHome == false) ghostDir = RIGHT;
    else ghostDir = UP;
}

int Ghost::getNextTileX() const {
    return nextTileX;
}

int Ghost::getNextTileY() const {
    return nextTileY;
}

int Ghost::getGhostDir() const {
    return ghostDir;
}

void Ghost::setDir(int dir) {
    ghostDir = dir;
}

void Ghost::setFrighten(const bool status) {
    if (isAtHome()) return;
    if (frighten != status) reTilePos();
    frighten = status;
    if (status) {
        ghostDir = (ghostDir + 2) % 4;
        speedMode = 1;
    }
}

void Ghost::setScattering(const bool status) {
    scattering = status;
}

bool Ghost::isScattering() {
    return scattering;
}

bool Ghost::isFrighten() {
    return frighten;
}

void Ghost::setTargetTile(int tilX, int tilY, int _speedMode) {
    this->speedMode = _speedMode;
    nextTileX = tilX;
    nextTileY = tilY;
}

void Ghost::moving() {
    int veloX, veloY, dir;
    veloX = veloY = 0; dir = -1;

    if (speedMode == 1) {
        if (frighten) ghostVelocity = 1;
        else if (isDead()) ghostVelocity = 4;
        else ghostVelocity = 2;
    }
    else ghostVelocity = 4;

    switch (ghostDir) {
        case UP:    {
            veloY -= ghostVelocity;
            dir = UP;
            break; }
        case DOWN:  {
            veloY += ghostVelocity;
            dir = DOWN;
            break; }
        case LEFT:  {
            veloX -= ghostVelocity;
            dir = LEFT;
            break; }
        case RIGHT: {
            veloX += ghostVelocity;
            dir = RIGHT;
            break;}
    }
    changeVelocityDir(veloX, veloY, dir);
    go();
}

void Ghost::respawn(const int tileX, const int tileY, const bool atHome) {
    resetCharacterTile(tileX, tileY);
    this->atHome = atHome;
    if (atHome == false) {
        if (rand() % 2 == 0) ghostDir = LEFT;
        else ghostDir = RIGHT;
    }
    else ghostDir = UP;
}

bool Ghost::isAtHome() const {
    return atHome;
}

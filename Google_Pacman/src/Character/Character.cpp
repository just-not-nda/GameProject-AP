#include "Character.h"
#include <iostream>
#include <cmath>


Character::Character(int tileX, int tileY, int veloX, int veloY) {
    this->tileX = tileX;
    this->tileY = tileY;
    if (veloX != 0) this->veloX = veloX;
    if (veloY != 0) this->veloY = veloY;

    this->scrPosX = tileX * 16 + 8;
    this->scrPosY = tileY * 16;
    dead = false;
}

bool Character::isDead() const {
    return dead;
}

int Character::getPosX() const {
    return scrPosX;
}

int Character::getPosY() const {
    return scrPosY;
}

int Character::getTileX() const {
    return tileX;
}

int Character::getTileY() const {
    return tileY;
}

void Character::reTilePos() {
    scrPosX = tileX * 16;
    scrPosY = tileY * 16;
}

void Character::changeVelocityDir(int veloX, int veloY, int dir) {
    this->veloX = veloX;
    this->veloY = veloY;
    this->dir = dir;
}

void Character::setDead(bool status, int id) {
    dead = status;
    if (id == 1) return;
    scrPosX = tileX * 16;
    scrPosY = tileY * 16;
}

void Character::go() {
    scrPosX += veloX;
    scrPosY += veloY;

    if (scrPosX % 16 == 0) tileX = scrPosX / 16;
    else tileX = (scrPosX + 8) / 16;

    if (scrPosY % 16 == 0) tileY = scrPosY / 16;
    else tileY = (scrPosY + 8) / 16;
}

void Character::teleport() {
    if (scrPosY == 8 * 16) {
        if (dir == LEFT && scrPosX == 0) {
            tileX = 58 - 1;
            scrPosX = 58 * 16 - 16;
        }
        else if (dir == RIGHT && scrPosX == 58 * 16 - 16) {
            tileX = 0;
            scrPosX = 0;
        }
    }
}

void Character::resetCharacterTile(const int tileX, const int tileY) {
    this->tileX = tileX;
    this->tileY = tileY;
    dead = false;
    veloX = veloY = 0;
    scrPosX = tileX * 16;
    scrPosY = tileY * 16;
}

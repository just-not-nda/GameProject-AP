#include "Character.h"
#include <iostream>
#include <cmath>


Character::Character(int tileX, int tileY, int velX, int velY) {
    this->tileX = tileX;
    this->tileY = tileY;
    if (velX != 0) this->velX = velX;
    if (velY != 0) this->velY = velY;

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

void Character::changeVelocityDir(int velX, int velY, int dir) {
    this->velX = velX;
    this->velY = velY;
    this->dir = dir;
}

void Character::setDead(bool status, int id) {
    dead = status;
    if (id == 1) return;
    scrPosX = tileX * 16;
    scrPosY = tileY * 16;
}

void Character::go() {
    scrPosX += velX;
    scrPosY += velY;

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
    velX = velY = 0;
    scrPosX = tileX * 16;
    scrPosY = tileY * 16;
}

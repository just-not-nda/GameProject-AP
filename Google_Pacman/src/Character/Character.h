#pragma once

#ifndef CHARACTER_H
#define CHARACTER_H

class Character
{
    private:
        int scrPosX, scrPosY;
        int velX = 0, velY = 0;
        int tileX, tileY;
        int dir;
        bool dead;
    public:
        static const int UP = 0;
        static const int RIGHT = 1;
        static const int DOWN = 2;
        static const int LEFT = 3;
        static const int CHARACTER_WIDTH  = 30;
        static const int CHARACTER_HEIGHT = 30;

        Character(int tileX, int tileY, int velX = 0, int velY = 0);
        ~Character() {}

        void go();
        bool isDead() const;
        int getPosX() const;
        int getPosY() const;
        int getTileX() const;
        int getTileY() const;
        void reTilePos();
        void changeVelocityDir(int velX, int velY, int dir);
        void setDead(bool status, int id = 0);
        void resetCharacterTile(const int tileX, const int tileY);
        void teleport();
};

#endif // CHARACTER_H

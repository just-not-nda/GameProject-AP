#pragma once

#ifndef _GHOST_H_
#define _GHOST_H_

#include "Character.h"
#include <random>

class Ghost : public Character {
    private:
        int nextTileX, nextTileY, ghostDir;
        bool frighten, scattering, atHome;
        int ghostVelocity, speedMode;
    public:
        static const int GHOST_START_TILE_X = 35;
        static const int GHOST_START_TILE_Y = 4;

        static const int DEFAULT_BLINKY_TILE_X = 57;
        static const int DEFAULT_BLINKY_TILE_Y = 1;
        static const int BLINKY_START_TILE_X = 35;
        static const int BLINKY_START_TILE_Y = 4;

        static const int DEFAULT_PINKY_TILE_X = 1;
        static const int DEFAULT_PINKY_TILE_Y = 1;
        static const int PINKY_START_TILE_X = 35;
        static const int PINKY_START_TILE_Y = 7;

        static const int DEFAULT_INKY_TILE_X = 57;
        static const int DEFAULT_INKY_TILE_Y = 16;
        static const int INKY_START_TILE_X = 33;
        static const int INKY_START_TILE_Y = 7;

        static const int DEFAULT_CLYDE_TILE_X = 1;
        static const int DEFAULT_CLYDE_TILE_Y = 16;
        static const int CLYDE_START_TILE_X = 37;
        static const int CLYDE_START_TILE_Y = 7;

        Ghost(int tileX, int tileY, bool atHome);

        int getNextTileX() const;
        int getNextTileY() const;
        int getGhostDir() const;
        void setDir(int dir);
        void setFrighten(const bool status);
        void setScattering(const bool status);
        bool isScattering();
        bool isFrighten();
        void setTargetTile(int tilX, int tilY, int _speedMode = 1);
        void moving();
        void respawn(const int tileX, const int tileY, const bool atHome);
        bool isAtHome() const;
};

#endif // _GHOST_H_

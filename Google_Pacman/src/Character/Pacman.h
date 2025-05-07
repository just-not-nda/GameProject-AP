#pragma once

#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "Character.h"
#include <stack>
using namespace std;

class Pacman : public Character
{
    private:
        stack<int> Direction;
        stack< pair<int, pair<int, int> > > Special;

    public:
        static const int pacmanVelocity = 2;
        static const int PACMAN_START_TILE_X = 35;
        static const int PACMAN_START_TILE_Y = 15;

        Pacman();
        ~Pacman() {
            while (!Direction.empty()) Direction.pop();
            while (!Special.empty()) Special.pop();
        }
        bool emptyDirStack() {
            return Direction.empty();
        }
        bool emptySpecial() {
            return Special.empty();
        }
        void pushtoStack(int newDir);
        void pushSpecialStack(int newDir, pair<int, int> nextCross);
        int getDir() const {
            return Direction.top();
        }
        pair<int, int> getSpecial() {
            return Special.top().second;
        }
        void moving();
        void stopmoving();
        void turn();
        void eraseSpecial();
        void respawn() {
            resetCharacterTile(PACMAN_START_TILE_X, PACMAN_START_TILE_Y);
            while (!Direction.empty()) Direction.pop();
            while (!Special.empty())   Special.pop();
        }

};

#endif // _PACMAN_H_

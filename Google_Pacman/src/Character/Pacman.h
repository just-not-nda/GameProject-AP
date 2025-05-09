#pragma once

#ifndef _PACMAN_H_
#define _PACMAN_H_

#include "Character.h"
#include <stack>
#include <algorithm>
using namespace std;

class Pacman : public Character
{
    private:
        stack<int> Direction;
        stack< pair<int, pair<int, int> > > turnPoints;

    public:
        static const int pacmanVelocity = 2;
        static const int PACMAN_START_TILE_X = 35;
        static const int PACMAN_START_TILE_Y = 15;

        Pacman();
        ~Pacman() {
            while (!Direction.empty()) Direction.pop();
            while (!turnPoints.empty()) turnPoints.pop();
        }
        bool emptyDirStack() {
            return Direction.empty();
        }
        bool emptyTurnPoints() {
            return turnPoints.empty();
        }
        void updateDir(int newDir);
        void addTurn(int newDir, pair<int, int> nextCross);
        int getDir() const {
            return Direction.top();
        }
        pair<int, int> getTurnPoints() {
            return turnPoints.top().second;
        }
        void moving();
        void stopmoving();
        void turn();
        void eraseTurnPoints();
};

#endif // _PACMAN_H_

#pragma once

#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <algorithm>
#include <queue>
#include "GetError.h"
using namespace std;

class Map
{
    private:
        GetError* Check = new GetError("Map");

        static const int MAP_WIDTH = 58;
        static const int MAP_HEIGHT = 17;
        const string MAP_FILE = "src/Map/MAP.txt";

        int tile[MAP_HEIGHT][MAP_WIDTH];
        int dist[MAP_WIDTH * MAP_HEIGHT][MAP_WIDTH * MAP_HEIGHT][4];
        pair<int, int> nextCrossID[MAP_HEIGHT][MAP_WIDTH][4];
        bool validTurnDir[MAP_HEIGHT][MAP_WIDTH][4];

        void findingCrossRoad();
        void calculateDistance();
        void NextCrossTileID();
    public:
        static const int UP = 0;
        static const int RIGHT = 1;
        static const int DOWN = 2;
        static const int LEFT = 3;

        Map();
        ~Map() {

        }
        int getTileID(int x, int y);
        pair<int, int> getnextCrossID(int x, int y, int dir);
        bool isWall(pair<int, int> tileID);
        bool iscrossRoad(int y, int x);
        bool canChangeDir(int x, int y, int newDir);
        bool besideCrossIsWall(pair<int, int> Cross, int newDir);
        int eatDots(const int &pacmanTileX, const int &pacmanTileY);
        int getDist(pair<int, int> start, pair<int, int> end, int startDir);
        void reset();
};

#endif // MAP_H

#ifndef MAP_H
#define MAP_H

#include <fstream>
#include <iostream>
#include <algorithm>

#include "GetError.h"

class Map
{
    private:
        GetError* Check = new GetError("Map");

        static const int MAP_WIDTH = 58;
        static const int MAP_HEIGHT = 17;
        const string MAP_FILE = "src/Map/MAP.txt";

        int tile[MAP_HEIGHT][MAP_WIDTH];

    public:
        Map();
        ~Map(){}


};

#endif // MAP_H

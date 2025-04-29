#include "Map.h"

bool first_load_map;
int mapData[17][58];

Map::Map()
{
    if (first_load_map)
    {
        ifstream file(MAP_FILE);
        if (file) {
            for (int row = 0; row < MAP_HEIGHT; row++)
            {
                for (int col = 0; col < MAP_WIDTH; col++)
                {
                    file >> mapData[row][col];
                    if(mapData[row][col] < 0 || mapData[row][col] > 48)
                    {
                        Check->Status("Wrong Tile");
                        return;
                    }
                }
            }
            Check->Status("Map read successfully!");
            first_load_map = false;
        }
        else
        {
            Check->Status("Error reading file!");
            return;
        }
    }

    for(int row = 0; row < MAP_HEIGHT; ++row)
    {
        for(int col = 0; col < MAP_WIDTH; ++col)
        {
            tile[row][col] = mapData[row][col];
        }
    }
}

int Map::getTileID(int x, int y) {
    return tile[y][x];
}

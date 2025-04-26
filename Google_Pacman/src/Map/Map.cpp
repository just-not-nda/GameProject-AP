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
                        cout << "Wrong Tile at (" << row << ", " << col << "): " << mapData[row][col] << endl;
                        return;
                    }
                }
            }
            cout << "Map read successfully!" << endl;
            first_load_map = false;
        }
        else
        {
            cout << "Error reading file!" << endl;
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


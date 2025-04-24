#include "Map.h"

bool firstLoadMap;
int orgMap[17][58];

Map::Map()
{
    if (firstLoadMap)
    {
        ifstream file(MAP_FILE);
        if (file) {
            for (int row = 0; row < MAP_HEIGHT; row++)
            {
                for (int col = 0; col < MAP_WIDTH; col++)
                {
                    file >> orgMap[row][col];
                    if(orgMap[row][col] < 0 || orgMap[row][col] > 48)
                    {
                        cout << "Wrong Tile at (" << row << ", " << col << "): " << orgMap[row][col] << endl;
                        return;
                    }
                }
            }
            cout << "Map read successfully!" << endl;
            firstLoadMap = false;
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
            tile[row][col] = orgMap[row][col];
        }
    }
}


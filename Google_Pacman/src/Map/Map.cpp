#include "Map.h"

bool first_load_map = true;
int mapData[17][58];
int bfsVisitMark[17][58];

typedef pair<int, int> PR;

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
            Check->Status("Map is OK");
            first_load_map = false;
        }
        else
        {
            Check->Status("Error reading file");
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

    findingCrossRoad();
    NextCrossTileID();
    calculateDistance();
}

int Map::getTileID(int x, int y) {
    return tile[y][x];
}

pair<int, int> Map::getnextCrossID(int x, int y, int dir) {
    return nextCrossID[y][x][dir];
}

bool Map::isWall(PR tileID) {
    if (tileID == PR(0, 7) || tileID == PR(57, 7)) return false;
    if (tileID.second == 43) return false;
    if (tileID.first < 1 || tileID.first > 56) return true;
    if (tileID.second < 1 || tileID.second > 16) return true;
    return tile[tileID.second][tileID.first] != 42 && tile[tileID.second][tileID.first] != 30 && tile[tileID.second][tileID.first] != 31;
}

bool Map::iscrossRoad(int x, int y) {
    int cnt = 0;
    if (validTurnDir[y][x][UP]) ++cnt;
    if (validTurnDir[y][x][RIGHT]) ++cnt;
    if (validTurnDir[y][x][DOWN]) ++cnt;
    if (validTurnDir[y][x][LEFT]) ++cnt;
    if (cnt >= 3) return true;
    if (cnt == 2) {
        if (validTurnDir[y][x][UP] && validTurnDir[y][x][DOWN]) return false;
        if (validTurnDir[y][x][LEFT] && validTurnDir[y][x][RIGHT]) return false;
        return true;
    }
    return false;
}

bool Map::canChangeDir(int x, int y, int newDir) {
    return validTurnDir[y][x][newDir];
}

bool Map::besideCrossIsWall(PR Cross, int newDir) {
    if (newDir == UP) Cross.second -= 1;
    else if (newDir == DOWN) Cross.second += 1;
    else if (newDir == RIGHT) Cross.first += 1;
    else Cross.first -= 1;
    return isWall(Cross);
}

void Map::findingCrossRoad() {
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            for (int dir = 0; dir < 4; ++dir) validTurnDir[y][x][dir] = false;

            if (tile[y][x] != 42 && tile[y][x] != 30 && tile[y][x] != 31) continue;

            if (y > 0  && (tile[y - 1][x] == 42 || tile[y - 1][x] == 30 || tile[y - 1][x] == 31)) validTurnDir[y][x][0] = true;
            if (y < MAP_HEIGHT - 1 && (tile[y + 1][x] == 42 || tile[y + 1][x] == 30 || tile[y + 1][x] == 31)) validTurnDir[y][x][2] = true;
            if (x > 0  && (tile[y][x - 1] == 42 || tile[y][x - 1] == 30 || tile[y][x - 1] == 31)) validTurnDir[y][x][3] = true;
            if (x < MAP_WIDTH - 1 && (tile[y][x + 1] == 42 || tile[y][x + 1] == 30 || tile[y][x + 1] == 31)) validTurnDir[y][x][1] = true;
        }
    }
}

void Map::NextCrossTileID() {
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        nextCrossID[y][0][LEFT] = PR(-1, -1);
        for (int x = 1; x < MAP_WIDTH; ++x) {
            nextCrossID[y][x][LEFT] = PR(-1, -1);

            if ( !isWall(PR (x, y)) ) {
                if (nextCrossID[y][x - 1][LEFT] != PR(-1, -1)) nextCrossID[y][x][LEFT] = nextCrossID[y][x - 1][LEFT];
                if (iscrossRoad(x - 1, y)) nextCrossID[y][x][LEFT] = PR(x - 1, y);
            }
        }

        nextCrossID[y][MAP_WIDTH - 1][RIGHT] = PR(-1, -1);
        for (int x = MAP_WIDTH - 2; x >= 0; --x) {
            nextCrossID[y][x][RIGHT] = PR(-1, -1);

            if ( !isWall(PR (x, y)) ) {
                if (nextCrossID[y][x + 1][RIGHT] != PR(-1, -1)) nextCrossID[y][x][RIGHT] = nextCrossID[y][x + 1][RIGHT];
                if (iscrossRoad(x + 1, y)) nextCrossID[y][x][RIGHT] = PR(x + 1, y);
            }
        }
    }

    for (int x = 0; x < MAP_WIDTH; ++x) {
        nextCrossID[0][x][UP] = PR(-1, -1);
        for (int y = 1; y < MAP_HEIGHT; ++y) {
            nextCrossID[y][x][UP] = PR(-1, -1);

            if ( !isWall(PR (x, y)) ) {
                if (nextCrossID[y - 1][x][UP] != PR(-1, -1)) nextCrossID[y][x][UP] = nextCrossID[y - 1][x][UP];
                if (iscrossRoad(x, y - 1)) nextCrossID[y][x][UP] = PR(x, y - 1);
            }
        }

        nextCrossID[MAP_HEIGHT - 1][x][DOWN] = PR(-1, -1);
        for (int y = MAP_HEIGHT - 2; y >= 0; --y) {
            nextCrossID[y][x][DOWN] = PR(-1, -1);

            if ( !isWall(PR (x, y)) ) {
                if (nextCrossID[y + 1][x][DOWN] != PR(-1, -1)) nextCrossID[y][x][DOWN] = nextCrossID[y + 1][x][DOWN];
                if (iscrossRoad(x, y + 1)) nextCrossID[y][x][DOWN] = PR(x, y + 1);
            }
        }
    }
}

void Map::calculateDistance() {
    for (int x = 0; x < MAP_WIDTH; ++x)
        for (int y = 0; y < MAP_HEIGHT; ++y)
            for (int u = 0; u < MAP_WIDTH; ++u)
                for (int v = 0; v < MAP_HEIGHT; ++v)
                    for (int dir = 0; dir < 4; ++dir)
                        dist[x * MAP_HEIGHT + y][u * MAP_HEIGHT + v][dir] = -1;
    int id = 0;
    int dh[4] = { 0, 1, 0, -1};
    int dc[4] = {-1, 0, 1,  0};
    int tempDist[MAP_WIDTH * MAP_HEIGHT];
    queue< PR > bfsQueue;
    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            if (isWall(PR (x, y))) continue;
            if (y == 8 && (x == 0 || x == 57)) continue;

            for (int startDir = 0; startDir < 4; ++ startDir) {
                    int xn = x + dh[startDir];
                    int yn = y + dc[startDir];
                if (isWall(PR (xn, yn))) continue;
                for (int i = 0; i < MAP_HEIGHT * MAP_WIDTH; ++i) tempDist[i] = -1;
                ++id;
                bfsVisitMark[yn][xn] = id;
                tempDist[xn * MAP_HEIGHT + yn] = 0;
                bfsQueue.push(PR (yn * MAP_WIDTH + xn, startDir));
                while (!bfsQueue.empty()) {
                    int curx = bfsQueue.front().first % MAP_WIDTH;
                    int cury = bfsQueue.front().first / MAP_WIDTH;
                    int lasDir = bfsQueue.front().second;
                    bfsQueue.pop();
                    if (cury == 8 && (curx == 0 || curx == 57)) continue;
                    for (int dir = 0; dir < 4; ++dir) {
                        int u = curx + dh[dir];
                        int v = cury + dc[dir];
                        if (lasDir % 2 == dir % 2 && dir != lasDir) continue;
                        if (isWall(PR (u, v))) continue;
                        if (bfsVisitMark[v][u] != id) {
                            bfsVisitMark[v][u] = id;
                            tempDist[u * MAP_HEIGHT + v] = tempDist[curx * MAP_HEIGHT + cury] + 1;
                            bfsQueue.push(PR (v * MAP_WIDTH + u, dir));
                        }
                    }
                }
                for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i)
                    dist[xn * MAP_HEIGHT + yn][i][startDir] = tempDist[i];
            }
        }
    }
}

int Map::eatDots(const int &pacmanTileX, const int &pacmanTileY) {
    if (tile[pacmanTileY][pacmanTileX] == 30) {
        tile[pacmanTileY][pacmanTileX] = 31;
        return 30;
    }
    if (tile[pacmanTileY][pacmanTileX] == 42) {
        tile[pacmanTileY][pacmanTileX] = 31;
        return 42;
    }
    return 0;
}

int Map::getDist(PR start, PR end, int startDir) {
    if (isWall(end)) return (start.first - end.first) * (start.first - end.first) + (start.second - end.second) * (start.second - end.second);
    else {
        if (dist[start.first * MAP_HEIGHT + start.second][end.first * MAP_HEIGHT + end.second][startDir] == -1)
            return (start.first - end.first) * (start.first - end.first) + (start.second - end.second) * (start.second - end.second);
        else return dist[start.first * MAP_HEIGHT + start.second][end.first * MAP_HEIGHT + end.second][startDir];
    }
}

void Map::reset() {
    for (int i = 0; i < MAP_HEIGHT; ++i)
        for (int j = 0; j < MAP_WIDTH; ++j)
            tile[i][j] = mapData[i][j];
}


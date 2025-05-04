#include "Texture.h"

Texture::Texture() {
    tileTexture = nullptr;
    characterTexture = nullptr;
    ghostScore = nullptr;
    pacmanFrame = 0;
    for(int i = 0; i < 7; i++)ghostFrame[i] = 0;
}

Texture::~Texture() {
    SDL_DestroyTexture(tileTexture);
    tileTexture = nullptr;
    SDL_DestroyTexture(characterTexture);
    characterTexture = nullptr;
    SDL_DestroyTexture(ghostScore);
    ghostScore = nullptr;
    for(int i = 0; i < 7; i++)ghostFrame[i] = 0;
}

void Texture::loadTile(SDL_Renderer* &renderer) {
    SDL_Surface* img = IMG_Load("assets/Map Tile.png");

    if (img == nullptr)
    {
        Check->Status( IMG_GetError() );
    }
    else
    {
        tileTexture = SDL_CreateTextureFromSurface(renderer, img);

        int tileID = 0;
        for (int row = 0; row < TILES_PER_COL; ++row) {
            for (int col = 0; col < TILES_PER_ROW; ++col) {
                tileSprite[tileID].x = col * (TILE_WIDTH + SPACING);
                tileSprite[tileID].y = row * (TILE_HEIGHT + SPACING);
                tileSprite[tileID].w = TILE_WIDTH;
                tileSprite[tileID].h = TILE_HEIGHT;
                ++tileID;
            }
        }
        Check->Status("Tile Texture is OK");
    }

    SDL_FreeSurface(img);
    img = nullptr;
}

void Texture::renderTile(SDL_Renderer* &renderer, int tileID, SDL_Rect* tileRect) {
    SDL_RenderCopy(renderer, tileTexture, &tileSprite[tileID], tileRect);
}

void Texture::loadCharacterTextures(SDL_Renderer* &renderer) {
    SDL_Surface* img = IMG_Load("assets/Character Texture.png");
    if (img == nullptr) {
        Check->Status( IMG_GetError() );
    }
    else
    {
        characterTexture = SDL_CreateTextureFromSurface(renderer, img);

        int x = 0, y = 0;
        for (int i = 0; i < 3; ++i)  pacmanUP[i] = {x, y, 30, 30}, x += 31;
        for (int i = 3; i < 6; ++i)  pacmanDOWN[i % 3] = {x, y, 30, 30}, x += 31;
        for (int i = 6; i < 9; ++i)  pacmanLEFT[i % 3] = {x, y, 30, 30}, x += 31;
        for (int i = 9; i < 12; ++i) pacmanRIGHT[i % 3] = {x, y, 30, 30}, x += 31;
        x = 0;
        for (int i = 0; i < 11; ++i) pacmanDEAD[i] = {x, 155, 30, 30}, x += 31;

        Check->Status("Pacman Texture is OK");

        x = 0; y = 31;
        for (int i = 0; i < TOTAL_GHOST - 1; ++i) {
            ghost[i][UP   ][0] = {x, y, 30, 30}; x += 31;
            ghost[i][UP   ][1] = {x, y, 30, 30}; x += 31;
            ghost[i][DOWN ][0] = {x, y, 30, 30}; x += 31;
            ghost[i][DOWN ][1] = {x, y, 30, 30}; x += 31;
            ghost[i][LEFT ][0] = {x, y, 30, 30}; x += 31;
            ghost[i][LEFT ][1] = {x, y, 30, 30}; x += 31;
            ghost[i][RIGHT][0] = {x, y, 30, 30}; x += 31;
            ghost[i][RIGHT][1] = {x, y, 30, 30}; x += 31;
            ghost[i][GHOST_FRIGHTEN_1][0] = {x, 31, 30, 30}; x += 31;
            ghost[i][GHOST_FRIGHTEN_1][1] = {x, 31, 30, 30}; x += 31;
            ghost[i][GHOST_FRIGHTEN_2][0] = {x, 31, 30, 30}; x += 31;
            ghost[i][GHOST_FRIGHTEN_2][1] = {x, 31, 30, 30}; x = 0;
            y += 31;
        }
        x = 248; y = 62;
        ghost[GHOST_SPIRIT][UP   ][0] = {x, y, 30, 30};
        ghost[GHOST_SPIRIT][UP   ][1] = {x, y, 30, 30}; x += 31;
        ghost[GHOST_SPIRIT][DOWN ][0] = {x, y, 30, 30};
        ghost[GHOST_SPIRIT][DOWN ][1] = {x, y, 30, 30}; x = 248; y += 31;
        ghost[GHOST_SPIRIT][LEFT ][0] = {x, y, 30, 30};
        ghost[GHOST_SPIRIT][LEFT ][1] = {x, y, 30, 30}; x += 31;
        ghost[GHOST_SPIRIT][RIGHT][0] = {x, y, 30, 30};
        ghost[GHOST_SPIRIT][RIGHT][1] = {x, y, 30, 30}; x = y = 0;

        SDL_FreeSurface(img);
        img = IMG_Load("assets/Ghost Score.png");
        ghostScore = SDL_CreateTextureFromSurface(renderer, img);

        Check->Status("Ghost Texture is OK");
    }
    SDL_FreeSurface(img);
    img = nullptr;
}

void Texture::renderPacman(SDL_Renderer* &renderer, int x, int y, int state) {
    SDL_Rect pacmanRect, dsRect;
    dsRect = {x - 7, y - 7 + 100, 30, 30};
    ++pacmanFrame;

    if (state != PACMAN_DEAD_STATE && pacmanFrame == 30) pacmanFrame = 0;

    switch (state) {
        case -1: {
            pacmanRect = pacmanUP[0]; break; }
        case UP: {
            pacmanRect = pacmanUP   [ pacmanFrame / 10 ]; break;}
        case RIGHT: {
            pacmanRect = pacmanRIGHT[ pacmanFrame / 10 ]; break;}
        case DOWN: {
            pacmanRect = pacmanDOWN [ pacmanFrame / 10 ]; break;}
        case LEFT: {
            pacmanRect = pacmanLEFT [ pacmanFrame / 10 ]; break;}
        case PACMAN_DEAD_STATE: {
            pacmanRect = pacmanDEAD[pacmanFrame / 10]; break;}
    }
    SDL_RenderCopy(renderer, characterTexture, &pacmanRect, &dsRect);
}

void Texture::renderGhost(SDL_Renderer* &renderer, int x, int y, int ghostID, int state) {
    SDL_Rect ghostRect, dsRect;
    dsRect = {x - 7, y - 7 + 100, 30, 30};
    ++ghostFrame[ghostID];

    if (ghostFrame[ghostID] == 14) ghostFrame[ghostID] = 0;

    switch (state) {
        case UP: {
            ghostRect = ghost[ ghostID ][UP   ][ ghostFrame[ghostID] / 7 ]; break;}
        case RIGHT: {
            ghostRect = ghost[ ghostID ][RIGHT][ ghostFrame[ghostID] / 7 ]; break;}
        case DOWN: {
            ghostRect = ghost[ ghostID ][DOWN ][ ghostFrame[ghostID] / 7 ]; break;}
        case LEFT: {
            ghostRect = ghost[ ghostID ][LEFT ][ ghostFrame[ghostID] / 7 ]; break;}
        case GHOST_FRIGHTEN_1: {
            ghostRect = ghost[ghostID][GHOST_FRIGHTEN_1][ghostFrame[ghostID] / 7]; break;}
        case GHOST_FRIGHTEN_2: {
            if(ghostFrame[ghostID] / 7 == 0)ghostRect = ghost[ghostID][GHOST_FRIGHTEN_1][ghostFrame[ghostID] / 7];
            else ghostRect = ghost[ghostID][GHOST_FRIGHTEN_2][ghostFrame[ghostID] / 7];
            break;
            }
    }
    SDL_RenderCopy(renderer, characterTexture, &ghostRect, &dsRect);
}

void Texture::renderGhostScore(SDL_Renderer*& renderer, int eaten_ghost_x, int eaten_ghost_y, int streak) {
    SDL_Rect srcRect, dsRect;
    switch (streak) {
        case 0: srcRect = { 0,  0, 50, 50}; break;
        case 1: srcRect = {50,  0, 50, 50}; break;
        case 2: srcRect = {100, 0, 50, 50}; break;
        case 3: srcRect = {150, 0, 50, 50}; break;
    }
    dsRect = {eaten_ghost_x - 7, eaten_ghost_y - 7 + 100, 30, 30};
    SDL_RenderCopy(renderer, ghostScore, &srcRect, &dsRect);
}

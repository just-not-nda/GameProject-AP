#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include "../Setup/GetError.h"

class Texture
{
    private:
        GetError* Check = new GetError("Texture");

        SDL_Texture* tileTexture;
        SDL_Rect tileSprite[48];
        int pacmanFrame;
        int ghostFrame[7];

        SDL_Rect pacmanUP[3], pacmanDOWN[3], pacmanLEFT[3], pacmanRIGHT[3], pacmanDEAD[11];
        SDL_Rect ghost[7][6][2];

        SDL_Texture* characterTexture;
        SDL_Texture* ghostScore;
    public:
        static const int TILE_WIDTH = 16;
        static const int TILE_HEIGHT = 16;
        static const int SPACING = 1;
        static const int TILES_PER_ROW = 12;
        static const int TILES_PER_COL = 4;

        static const int UP = 0;
        static const int RIGHT = 1;
        static const int DOWN = 2;
        static const int LEFT = 3;

        static const int GHOST_FRIGHTEN_1 = 4;
        static const int GHOST_FRIGHTEN_2 = 5;
        static const int PACMAN_DEAD_STATE = 5;
        static const int BLINKY = 0;
        static const int PINKY  = 1;
        static const int INKY   = 2;
        static const int CLYDE  = 3;
        static const int GHOST_SPIRIT = 4;
        static const int TOTAL_GHOST = 5;

        Texture();
        ~Texture();

        bool pacmanIsDead();
        void loadTile(SDL_Renderer* &renderer);
        void renderTile(SDL_Renderer* &renderer, int tileID, SDL_Rect* tileRect);
        void loadCharacterTextures(SDL_Renderer* &renderer);
        void renderPacman(SDL_Renderer*& renderer, int x, int y, int state);
        void renderGhost(SDL_Renderer*& renderer, int x, int y, int ghost_id, int state);
        void renderGhostScore(SDL_Renderer*& renderer, int eaten_ghost_x, int eaten_ghost_y, int streak);
};

#endif // TEXTURE_H

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

    public:
        static const int TILE_WIDTH = 16;
        static const int TILE_HEIGHT = 16;
        static const int SPACING = 1;
        static const int TILES_PER_ROW = 12;
        static const int TILES_PER_COL = 4;

        Texture();
        ~Texture();

        void loadTile(SDL_Renderer* &renderer);
        void renderTile(SDL_Renderer* &renderer, int tileID, SDL_Rect* tileRect);
        void loadPacmanAndGhostTexture(SDL_Renderer* &renderer);
        void renderPacmanTexture(SDL_Renderer* &renderer, int posX, int posY, int status);
};

#endif // TEXTURE_H

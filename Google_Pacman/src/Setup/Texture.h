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
        const int TILE_WIDTH = 16;
        const int TILE_HEIGHT = 16;
        const int SPACING = 1;
        const int TILES_PER_ROW = 12;
        const int TILES_PER_COL = 4;

        Texture();
        ~Texture();

        void loadTileTexture(SDL_Renderer* &renderer);
        void renderTileTexture(SDL_Renderer* &renderer, int tileID, SDL_Rect* tileRect);
};

#endif // TEXTURE_H

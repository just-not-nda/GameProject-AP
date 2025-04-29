#include "Texture.h"

Texture::Texture() {
    tileTexture = nullptr;
}

Texture::~Texture() {
    SDL_DestroyTexture(tileTexture);
    tileTexture = nullptr;

}

void Texture::loadTileTexture(SDL_Renderer* &renderer) {
    SDL_Surface* Image = IMG_Load("assets/Pacman Google Map Tile.png");

    if (Image == nullptr)
    {
        Check->Status( IMG_GetError() );
    }
    else
    {
        tileTexture = SDL_CreateTextureFromSurface(renderer, Image);

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
        Check->Status("Tile Texture got successfully!");
    }

    SDL_FreeSurface(Image);
    Image = nullptr;
}


void Texture::renderTileTexture(SDL_Renderer* &renderer, int tileID, SDL_Rect* tileRect) {
    SDL_RenderCopy(renderer, tileTexture, &tileSprite[tileID], tileRect);
}

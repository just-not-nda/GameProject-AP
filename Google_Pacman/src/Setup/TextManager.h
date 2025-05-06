#pragma once

#ifndef _TEXTMANAGER_H_
#define _TEXTMANAGER_H_

#include <SDL_ttf.h>
#include <string>
using namespace std;

class TextManager {
    private:
        SDL_Rect dsRect;

        TTF_Font* font;
        SDL_Texture* textTexture;
        SDL_Surface* textSurface;
    public:
        const string FONT_NAME = "assets/Poppins-Bold.ttf";
        static const int CENTER = 0;
        static const int LEFT = 1;

        TextManager(int fontSize);
        ~TextManager();

        int getTextWidth() const;
        void loadRenderText(SDL_Renderer* &renderer, string text, SDL_Color textColor);
        void renderText(SDL_Renderer* &renderer, int x, int y, const int type);
};

#endif // _TEXTMANAGER_H_

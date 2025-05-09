#include "TextManager.h"

TextManager::TextManager(int fontSize) {
    font = TTF_OpenFont(FONT_FILE.c_str(), fontSize);
    textTexture = nullptr;
    textSurface = nullptr;
}

TextManager:: ~TextManager() {
    if (font != nullptr) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    if (textTexture != nullptr) {
        SDL_DestroyTexture(textTexture);
        textTexture = nullptr;
    }
    if (textSurface != nullptr) {
        SDL_FreeSurface(textSurface);
        textSurface = nullptr;
    }
}

int TextManager::getTextWidth() const {
    return textSurface->w;
}

void TextManager::loadRenderText(SDL_Renderer* &renderer, string text, SDL_Color textColor) {
    if (textSurface != nullptr) SDL_FreeSurface(textSurface);
    textSurface = TTF_RenderText_Blended(font, text.c_str(), textColor);
    if (textTexture != nullptr) SDL_DestroyTexture(textTexture);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
}

void TextManager::renderText(SDL_Renderer* &renderer, int x, int y, const int type) {
    dsRect = {x, y, textSurface->w, textSurface->h};
    if (type == CENTER) {
        dsRect.x = x - textSurface->w / 2;
        dsRect.y = y - textSurface->h / 2;
    }
    SDL_RenderCopy(renderer, textTexture, nullptr, &dsRect);
}

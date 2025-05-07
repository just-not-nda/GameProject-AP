#pragma once

#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>
#include <string>
#include "TextManager.h"
using namespace std;

class Button {
    private:
        SDL_Rect buttonRect;
        TextManager* normalText;
        TextManager* selectText;
        int buttonStatus;
        string buttonText;
    public:
        static const int BUTTON_OUT = 0;
        static const int BUTTON_IN  = 1;
        static const int BUTTON_PRESSED = 2;
        const SDL_Color normalColor = {66, 133, 244, 255};
        const SDL_Color selectColor = {234, 67, 53, 255};

        Button(int width, int height, int x, int y);

        void loadButton(SDL_Renderer* &renderer, string text = "");
        void renderButton(SDL_Renderer* &renderer);
        void setStatus(const int status);
        void changeSoundButton(SDL_Renderer* &renderer);
        string getText() const;
};

#endif // _BUTTON_H

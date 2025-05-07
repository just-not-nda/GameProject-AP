#pragma once

#ifndef _MENU_H_
#define _MENU_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "GetError.h"
#include "Button.h"
using namespace std;

class Menu {
    private:
        int buttonID;
        int menuStatus;

        int TOTAL_BUTTON;
        int MENU_WIDTH;
        int MENU_HEIGHT;
        int x, y;

        SDL_Texture* menuTexture;
        vector<Button*> menuButton;

        GetError* Check = new GetError("Menu");
        bool running = false;

    public:
        static const int RESUME = 0;
        static const int RUNNING = 1;
        static const int PLAY_BUTTON_PRESSED = 2;
        static const int EXIT_BUTTON_PRESSED = 3;
        static const bool ON = true;
        static const bool OFF = false;

        Menu(const int x, const int y, const int totalButton, const int buttonWidth, const int buttonHeight);
        ~Menu();

        void init(SDL_Renderer* &renderer, const string imgPath, vector<string> &buttonText);
        void render(SDL_Renderer* &renderer);
        void handleEvent(SDL_Event &e, SDL_Renderer* &renderer);
        bool isRunning() const;
        int getStatus() const;
        void reOpen();
        void changeRunStatus();
};

#endif // _MENU_H_

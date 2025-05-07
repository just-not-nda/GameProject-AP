#pragma once

#ifndef _GAMECORE_H_
#define _GAMECORE_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include "GameState.h"
#include "Button.h"
#include "GetError.h"
using namespace std;

class GameCore {
    private:
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        Menu* startMenu = nullptr;
        GameState* gameState = nullptr;

        GetError* Check = new GetError("GameCore");
        bool Running = false;
        bool runningMenu = false;
    public:
        static const int SCREEN_WIDTH = 930;
        static const int SCREEN_HEIGHT = 475;
        const string WINDOW_TITTLE = "Google Pacman";
        vector<string> startMenuButtonText = {"New Game", "Sound: ON", "Exit"};

        GameCore();

        ~GameCore();

        void init();

        void quit();

        void run();

};

#endif // _GAMECORE_H_


#pragma once

#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

#include <string>
#include <vector>
#include "Engine.h"
#include "SDL_mixer.h"
#include "Button.h"
#include "Menu.h"
using namespace std;

class GameState {
    private:
        vector<int> muteChannel;
        Engine* engine = nullptr;
        Menu* pauseMenu = nullptr;
        bool running = true;
        bool runPauseMenu = false;
    public:
        vector<string> pauseMenuButtonText = {"Resume", "New Game", "Sound: ON", "Exit to Start Menu"};
        GameState();

        ~GameState();

        void handleEvent(SDL_Event &e, SDL_Renderer* &renderer, bool &runningMain);

        void newGame(SDL_Renderer* &renderer);

        void runGame(bool &exitToMenu);

        void render(SDL_Renderer* &renderer);
};

#endif // _GAMESTATE_H_

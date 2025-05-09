#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include "Map.h"
#include "Pacman.h"
#include "Ghost.h"
#include "Texture.h"
#include "TimeManager.h"
#include "GameManager.h"
#include "AudioManager.h"
#include <utility>
#include <limits>
#include <random>
#include <time.h>
using namespace std;

class Engine
{
private:

    Map* _map;
    Pacman* pacman = nullptr;
    Ghost* blinky = nullptr;
    Ghost* pinky = nullptr;
    Ghost* inky = nullptr;
    Ghost* clyde = nullptr;
    Texture* texture;
    TimeManager* timeManager;
    GameManager* gameManager;
    AudioManager* audioManager;
    SDL_Texture* levelUp;
    SDL_Texture* ready;
    bool runGameOver = false;
    int delayTime = 0;
protected:
        void respawnObject();
        void ghostGo(Ghost* &ghost);
        void pacmanMeetGhost(Ghost* &ghost);
        void renderGhost(SDL_Renderer* &renderer, Ghost* &ghost, int ghostID);
public:
    Engine()
    {
        _map = nullptr;
        pacman = nullptr;
        blinky = nullptr;
        pinky  = nullptr;
        clyde  = nullptr;
        inky   = nullptr;
        texture = nullptr;
        timeManager   = nullptr;
        gameManager   = nullptr;
        audioManager  = nullptr;

    }
    ~Engine()
    {
            delete _map;
            _map = nullptr;
            delete pacman;
            pacman = nullptr;
            delete blinky;
            blinky = nullptr;
            delete pinky;
            pinky = nullptr;
            delete clyde;
            clyde = nullptr;
            delete inky;
            inky = nullptr;
            SDL_DestroyTexture(levelUp);
            levelUp = nullptr;
            SDL_DestroyTexture(ready);
            ready = nullptr;

            delete texture;
            texture = nullptr;
            delete timeManager;
            timeManager = nullptr;
            delete gameManager;
            gameManager = nullptr;
            delete audioManager;
            audioManager = nullptr;

    }

    void newGame();
    void init(SDL_Renderer* &renderer);
    void handleEvent(SDL_Event &e);
    void loop(bool &exitToMenu);
    void render(SDL_Renderer* &renderer);
};

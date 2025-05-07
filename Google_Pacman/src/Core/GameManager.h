#pragma once

#ifndef _GAMEMANAGER_H_
#define _GAMEMANAGER_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include "Ghost.h"
#include "TextManager.h"
#include "Button.h"

class GameManager {
    private:
        int level;
        int life;
        int eatenCoins;
        int eatenGhost;
        int scores;
        int playerDecision;
        int currentBut;
        int ghostEatenPosX;
        int ghostEatenPosY;

        int PINKY_COIN_LIMIT;
        int INKY_COIN_LIMIT;
        int CLYDE_COIN_LIMIT;

        TextManager* levelText;
        TextManager* liveText;
        TextManager* scoreText;

        SDL_Texture* egBoard = nullptr;
        Button* againBut;
        Button* quitBut;
    protected:
        SDL_Texture* loadImage(SDL_Renderer* &renderer, const string imagePath);
    public:

        const int TOTAL_COINS = 275;
        static const int normalCoin = 30;
        static const int superCoin = 42;
        static const int notCoin = 0;
        static const int pauseGame = 1;
        static const int AGAIN = 2;
        static const int QUIT  = 3;
        static const int WAITING = 4;

        GameManager(SDL_Renderer* &renderer);
        ~GameManager();

        void reset();
        void levelUp();
        void eatCoins(const int typeOfCoin);
        void eatGhost(const int ghostTileX, const int ghostTileY);
        void lostALife();
        int getEatenGhostStreak() const;
        int getEatenGhostPosX() const;
        int getEatenGhostPosY() const;
        int getRemainLife() const;
        bool clearAllCoins() const;
        int getLevel() const;
        int getPlayerDecision() const;
        int getRemainCoin() const;
        void handleGhostPos(Ghost* &pinky, Ghost* &inky, Ghost* &clyde);
        void handleEGBoard(SDL_Event &e);
        void renderHUD(SDL_Renderer* &renderer);
        void runEGBoard(SDL_Renderer* &renderer);

};

#endif // GAMEMANAGER_H_

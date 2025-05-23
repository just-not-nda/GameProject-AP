#pragma once

#ifndef _AUDIOMANAGER_H_
#define _AUDIOMANAGER_H_

#include <SDL_mixer.h>
#include <vector>
#include "GetError.h"

class AudioManager {
    private:
        bool dead;
        bool ghostTurnBlue;
        bool ghostGoHome;
        int eatDotTime;
        int oldMoveType, newMoveType;
        GetError* Check = new GetError("Audio Manager");
        Mix_Chunk* soundEffect[11];
    public:
        static const int MOVE_0 = 0;
        static const int MOVE_1 = 1;
        static const int MOVE_2 = 2;
        static const int MOVE_3 = 3;
        static const int START = 4;
        static const int EAT_DOT = 5;
        static const int EAT_GHOST = 6;
        static const int GHOST_TURN_BLUE = 7;
        static const int GHOST_GO_HOME = 8;
        static const int DEAD = 9;
        static const int WINNING = 10;
        static const int TOTAL_SOUND_EFFECT = 11;
        static const int NORMAL_GHOST = 11;
        static const int REVIVAL_GHOST = 12;

        AudioManager();
        ~AudioManager();

        void insertPlayList(const int soundID);
        void loadSound();
        void playSound();
        void reset();
};

#endif // _AUDIOMANAGER_H_

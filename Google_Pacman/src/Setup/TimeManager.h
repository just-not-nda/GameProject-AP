#pragma once

#ifndef _TIMEMANAGER_H_
#define _TIMEMANAGER_H_

#include <SDL.h>
#include <stack>
using namespace std;

typedef pair<const int, double> CID;

class TimeManager {
    private:
        Uint32 lastTick;
        Uint32 FlastTick;
        Uint32 GlastTick;
        Uint32 lastFrame = 0;

        stack<CID> mode;

        int lastStatus;
        bool pause;
        double FRIGHTEN_TIME = 10.0;
        double SCATTERING_TIME = 8.0;
        double CHASING_TIME = 20.0;
    public:
        const int FPS = 60;
        const int FRIGHTEN_MODE = 0;
        const int SCATTERING_MODE = 1;
        const int CHASING_MODE = 2;
        const double oo = -1;

        TimeManager();

        double remainFrightenTime() const;
        bool isFrightenTime() const;
        bool isScatteringTime() const;
        void setFrightenTime();
        void resetTick(const int level);
        void updateStatus();
        void stablizeFPS();
        bool pauseTick(const bool status);
};

#endif // TIMEMANAGER_H_

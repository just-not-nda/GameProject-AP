#include "TimeManager.h"

TimeManager::TimeManager() {
    lastTick = SDL_GetTicks();
    FlastTick = GlastTick = SDL_GetTicks();
    mode.push( CID(CHASING_MODE, oo) );
    mode.push( CID(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( CID(CHASING_MODE, CHASING_TIME) );
    mode.push( CID(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( CID(CHASING_MODE, CHASING_TIME) );
    mode.push( CID(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( CID(CHASING_MODE, CHASING_TIME) );
    mode.push( CID(SCATTERING_MODE, SCATTERING_TIME) );
}

bool TimeManager::isScatteringTime() const {
    return mode.top().first == SCATTERING_MODE;
}

void TimeManager::setFrightenTime() {
    lastStatus = mode.top().first;
    mode.pop();
    if (lastStatus == CHASING_MODE)
        mode.push( CID(CHASING_MODE, CHASING_TIME - (SDL_GetTicks() - lastTick)) );
    else if (lastStatus == SCATTERING_MODE)
        mode.push( CID(SCATTERING_MODE, SCATTERING_TIME - (SDL_GetTicks() - lastTick)) );
    mode.push( CID(FRIGHTEN_MODE, FRIGHTEN_TIME) );
    lastTick = SDL_GetTicks();
}


void TimeManager::resetTick(const int level) {
    while (!mode.empty()) mode.pop();

    mode.push( CID(CHASING_MODE, oo) );
    mode.push( CID(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( CID(CHASING_MODE, CHASING_TIME) );
    mode.push( CID(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( CID(CHASING_MODE, CHASING_TIME) );
    mode.push( CID(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( CID(CHASING_MODE, CHASING_TIME) );
    mode.push( CID(SCATTERING_MODE, SCATTERING_TIME) );
    FRIGHTEN_TIME = 10.0;

    lastTick = SDL_GetTicks();
}

void TimeManager::updateStatus() {
    if (pause == true) {
        lastTick = SDL_GetTicks();
        return;
    }
    double timePass = (SDL_GetTicks() - lastTick) / 1000.0;

    if (!mode.empty()) {
        if (timePass > mode.top().second && mode.top().second != oo) {
            mode.pop(); lastTick = SDL_GetTicks();
        }
    }
}

void TimeManager::stablizeFPS() {
    if ((int)(SDL_GetTicks() - lastFrame) < 1000 / FPS) {
        SDL_Delay(1000 / FPS + lastFrame - SDL_GetTicks());
    }
    lastFrame = SDL_GetTicks();
}

bool TimeManager::pauseTick(const bool status) {
    return pause = status;
}

bool TimeManager::isFrightenTime() const {
    return mode.top().first == FRIGHTEN_MODE;
}

double TimeManager::remainFrightenTime() const {
    return FRIGHTEN_TIME - (SDL_GetTicks() - lastTick) / 1000.0;
}

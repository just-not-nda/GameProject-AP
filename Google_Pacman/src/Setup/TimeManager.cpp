#include "TimeManager.h"

TimeManager::TimeManager() {
    lastTime = SDL_GetTicks();
    mode.push( GhostModeState(CHASING_MODE, oo) );
    mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( GhostModeState(CHASING_MODE, CHASING_TIME) );
    mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( GhostModeState(CHASING_MODE, CHASING_TIME) );
    mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( GhostModeState(CHASING_MODE, CHASING_TIME) );
    mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME) );
}

bool TimeManager::isScatteringTime() const {
    return mode.top().first == SCATTERING_MODE;
}

void TimeManager::setFrightenTime() {
    lastStatus = mode.top().first;
    mode.pop();
    if (lastStatus == CHASING_MODE)
        mode.push( GhostModeState(CHASING_MODE, CHASING_TIME - (SDL_GetTicks() - lastTime)) );
    else if (lastStatus == SCATTERING_MODE)
        mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME - (SDL_GetTicks() - lastTime)) );
    mode.push( GhostModeState(FRIGHTEN_MODE, FRIGHTEN_TIME) );
    lastTime = SDL_GetTicks();
}


void TimeManager::reset(const int level) {
    while (!mode.empty()) mode.pop();

    mode.push( GhostModeState(CHASING_MODE, oo) );
    mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( GhostModeState(CHASING_MODE, CHASING_TIME) );
    mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( GhostModeState(CHASING_MODE, CHASING_TIME) );
    mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME) );
    mode.push( GhostModeState(CHASING_MODE, CHASING_TIME) );
    mode.push( GhostModeState(SCATTERING_MODE, SCATTERING_TIME) );
    FRIGHTEN_TIME = 7.0;

    lastTime = SDL_GetTicks();
}

void TimeManager::updateStatus() {
    if (pause == true) {
        lastTime = SDL_GetTicks();
        return;
    }
    double timePass = (SDL_GetTicks() - lastTime) / 1000.0;

    if (!mode.empty()) {
        if (timePass > mode.top().second && mode.top().second != oo) {
            mode.pop(); lastTime = SDL_GetTicks();
        }
    }
}

void TimeManager::stablizeFPS() {
    if ((int)(SDL_GetTicks() - lastFrame) < 1000 / FPS) {
        SDL_Delay(1000 / FPS + lastFrame - SDL_GetTicks());
    }
    lastFrame = SDL_GetTicks();
}

bool TimeManager::pauseTime(const bool status) {
    return pause = status;
}

bool TimeManager::isFrightenTime() const {
    return mode.top().first == FRIGHTEN_MODE;
}

double TimeManager::remainFrightenTime() const {
    return FRIGHTEN_TIME - (SDL_GetTicks() - lastTime) / 1000.0;
}

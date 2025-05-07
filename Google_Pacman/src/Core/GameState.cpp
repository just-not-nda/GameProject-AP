#include "GameState.h"

GameState::GameState()
{
    engine = new Engine();
    pauseMenu = new Menu(210, 204, pauseMenuButtonText.size(), 320, 30);
    runPauseMenu = false;
}

GameState::~GameState() {
    delete engine;
    engine = nullptr;

    runPauseMenu = false;
}

void GameState::newGame(SDL_Renderer* &renderer) {
    static bool inited = false;
    if (!inited) {
        pauseMenu->init(renderer, "assets/Menu.png", pauseMenuButtonText);
        engine->init(renderer);
        inited = true;
    }
    else {
        engine->newGame();
    }
}

void GameState::runGame(bool &exitToMenu) {
    if (runPauseMenu == false) engine->loop(exitToMenu);
}

void GameState::handleEvent(SDL_Event& e, SDL_Renderer* &renderer, bool &exitToMainMenu) {
    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
        runPauseMenu = true;
        pauseMenu->reOpen();
        while (!muteChannel.empty()) muteChannel.pop_back();
        for (int channel = 1; channel <= 8; ++channel) {
            if (Mix_Paused(channel) == 0) {
                Mix_Pause(channel);
                muteChannel.push_back(channel);
            }
        }
    }
    else {
        if (runPauseMenu) {
            pauseMenu->handleEvent(e, renderer);
            switch (pauseMenu->getStatus()) {
                case Menu::RESUME:
                    runPauseMenu = false;
                    for (int channel : muteChannel) Mix_Resume(channel);
                    break;
                case Menu::PLAY_BUTTON_PRESSED:
                    runPauseMenu = false;
                    engine->newGame();
                    break;
                case Menu::EXIT_BUTTON_PRESSED:
                    exitToMainMenu = true;
                    runPauseMenu = false; break;
            }
        }
        else engine->handleEvent(e);
    }
}

void GameState::render(SDL_Renderer* &renderer) {
    if (runPauseMenu) pauseMenu->render(renderer);
    else engine->render(renderer);
}

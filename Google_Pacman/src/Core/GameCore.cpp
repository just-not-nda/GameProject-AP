#include "GameCore.h"

GameCore::GameCore() {
    window = nullptr;
    renderer = nullptr;
    gameState = nullptr;
    startMenu = nullptr;
    runMenu = false;
}

GameCore::~GameCore() {
    delete gameState;
    gameState = nullptr;

    delete startMenu;
    startMenu = nullptr;

    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

void GameCore::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        Check->Status( SDL_GetError() );
    }
    else {
        window = SDL_CreateWindow(WINDOW_TITTLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        Check->Status("Window is opened");
        running = true;

        if (window == nullptr) {
            Check->Status( SDL_GetError() );
        }
        else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            Check->Status("Renderer is created");
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

            if (TTF_Init() < 0) Check->Status( TTF_GetError() );
            else Check->Status("TTF is OK");

            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048) < 0) Check->Status( Mix_GetError() );
            else Check->Status("Audio is OK");
            Mix_Volume(-1, MIX_MAX_VOLUME);

        }
    }
}

void GameCore::quit() {
    SDL_DestroyRenderer(renderer);
    renderer = nullptr;
    SDL_DestroyWindow(window);
    window = nullptr;
    Mix_Quit();
    TTF_Quit();
    atexit(SDL_Quit);
    Check->Status("Exited");
    return;
}

void GameCore::run() {
    startMenu = new Menu(475, 285, startMenuButtonText.size(), 320, 30);
    startMenu->init(renderer, "assets/StartMenu.png", startMenuButtonText);
    startMenu->changeRunStatus();

    SDL_Event e;
    runMenu = true;
    bool startGame = false;
    gameState = new GameState();

    while (running) {

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) running = false;
            else {
                if (runMenu) {
                    startMenu->handleEvent(e, renderer);
                    switch (startMenu->getStatus()) {
                        case Menu::PLAY_BUTTON_PRESSED:
                            runMenu = false; break;
                        case Menu::EXIT_BUTTON_PRESSED:
                            running = false; break;
                    }
                }
                else {
                    gameState->handleEvent(e, renderer, runMenu);
                    if (runMenu) startMenu->reOpen();
                }
            }
        }
        if (!runMenu) {
            if (!startGame) {
                gameState->newGame(renderer);
                startGame = true;
            }
            gameState->runGame(runMenu);
            if (runMenu) startMenu->reOpen(), startGame = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (runMenu)
            startMenu->render(renderer);
        else gameState->render(renderer);

        SDL_RenderPresent(renderer);
    }
}

#include "Engine.h"

#include <iostream>
using namespace std;

bool Engine::init()
{
     if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
     {
         cout << "SDL ko khoi tao duoc" << SDL_GetError() << endl;
         return false;
     }
     window = SDL_CreateWindow("Pacman Google Doodle", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               930, 275 + 100 * 2, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE); // MAP_REAL_SIZE = 930 x 275

    if (!window) {
        std::cout << "Ko tao dc cua so: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Ko tao dc renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image ko khoi tao dc: " << IMG_GetError() << std::endl;
        return false;
    }

    is_running = true;
    return true;
}

void Engine::handleEvent(SDL_Event &e)
{
    if (e.type == SDL_QUIT)
    {
        is_running = false;
    }
}

void Engine::render(SDL_Renderer* &renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);
}

void Engine::run()
{
    SDL_Event event;
    while (is_running)
    {

        while (SDL_PollEvent(&event))
        {
            handleEvent(event);
        }
        render(renderer);
    }
}

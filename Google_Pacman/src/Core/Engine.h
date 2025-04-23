#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <SDL_image.h>

class Engine
{
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;

    protected:

    public:
        Engine()
        {
            window = nullptr;
            renderer = nullptr;
        }
        ~Engine()
        {
            if (renderer) SDL_DestroyRenderer(renderer);
            if (window) SDL_DestroyWindow(window);
            IMG_Quit();
            SDL_Quit();
        }

        bool init();
        void render();
};

#endif // ENGINE_H

#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <SDL_image.h>

class Engine
{
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool is_running;

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
        void render(SDL_Renderer* &renderer);
        void handleEvent(SDL_Event &e);
        void run();
};

#endif // ENGINE_H

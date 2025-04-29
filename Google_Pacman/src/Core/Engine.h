#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <SDL_image.h>
#include "../Map/Map.h"
#include "../Setup/Texture.h"

class Engine
{
    private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool is_running;

    Map* _map;
    Texture* texture;
    protected:

    public:
        Engine()
        {
            _map = nullptr;
            texture = nullptr;

            window = nullptr;
            renderer = nullptr;
        }
        ~Engine()
        {
            delete _map;
            _map = nullptr;
            delete texture;
            texture = nullptr;

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

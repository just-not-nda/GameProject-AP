#include "Menu.h"

Menu::Menu(const int baseScrPosX, const int baseScrPosY, const int totalButton, const int buttonWidth, const int buttonHeight) {
    menuTexture  = nullptr;
    TOTAL_BUTTON = totalButton;
    MENU_BUTTON_WIDTH  = buttonWidth;
    MENU_BUTTON_HEIGHT = buttonHeight;
    this->baseScrPosX  = baseScrPosX;
    this->baseScrPosY  = baseScrPosY;
}

Menu::~Menu() {
    Mix_FreeChunk(navigationSound);
    Mix_FreeChunk(selectionSound);

    SDL_DestroyTexture(menuTexture);
    menuTexture = nullptr;

    for (Button* button : menuButton) {
        delete button; button = nullptr;
    }
    menuButton.clear();
}



void Menu::init(SDL_Renderer* &renderer, const string imgPath, vector<string>& buttonText) {
    SDL_Surface* Image = IMG_Load(imgPath.c_str());
    if (Image == nullptr)
        Check->Status( IMG_GetError() );
    else {
        menuTexture = SDL_CreateTextureFromSurface(renderer, Image);
        SDL_FreeSurface(Image);


        for (int i = 0; i < TOTAL_BUTTON; ++i)
            menuButton.push_back(new Button(MENU_BUTTON_WIDTH, MENU_BUTTON_HEIGHT, baseScrPosX - MENU_BUTTON_WIDTH / 2, baseScrPosY + (5 + MENU_BUTTON_HEIGHT) * (i - 1)));

        for (int i = 0; i < TOTAL_BUTTON; ++i) {
            if (buttonText[i] == "Sound: ON") {
                if (Mix_Volume(-1, -1) == 0) buttonText[i] = "Sound: OFF";
            }
            menuButton[i]->loadButton(renderer, buttonText[i]);
            menuButton[i]->setStatus(Button::BUTTON_OUT);
        }
        menuButton[0]->setStatus(Button::BUTTON_IN);
        currentButtonID = 0;
        currentMenuStatus = RUNNING;
    }
}


void Menu::render(SDL_Renderer* &renderer) {
    SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr);
    for (int i = 0; i < TOTAL_BUTTON; ++i)
        menuButton[i]->renderButton(renderer);
}

void Menu::handleEvent(SDL_Event &e, SDL_Renderer* &renderer) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_s) {
            Mix_PlayChannel(7, navigationSound, 0);

                menuButton[currentButtonID]->setStatus(Button::BUTTON_OUT);
                (currentButtonID += 1) %= TOTAL_BUTTON;
                menuButton[currentButtonID]->setStatus(Button::BUTTON_IN);
        }
        else if (e.key.keysym.sym == SDLK_UP || e.key.keysym.sym == SDLK_w) {
            Mix_PlayChannel(7, navigationSound, 0);

                menuButton[currentButtonID]->setStatus(Button::BUTTON_OUT);
                (currentButtonID += TOTAL_BUTTON - 1) %= TOTAL_BUTTON;
                menuButton[currentButtonID]->setStatus(Button::BUTTON_IN);

        }
        else if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
            Mix_PlayChannel(7, navigationSound, 0);
        }
        else if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
            Mix_PlayChannel(7, navigationSound, 0);
        }
        else if (e.key.keysym.sym == SDLK_RETURN) {
            Mix_PlayChannel(7, navigationSound, 0);

                menuButton[currentButtonID]->setStatus(Button::BUTTON_IN);
                string menuText = menuButton[currentButtonID]->getText();
                if (menuText == "New Game" || menuText == "Play Again") currentMenuStatus = PLAY_BUTTON_PRESSED;
                else if (menuText == "Resume") currentMenuStatus = RESUME;
                else if (menuText == "Exit") currentMenuStatus = EXIT_BUTTON_PRESSED;
                else if (menuText == "Exit to Start Menu") currentMenuStatus = EXIT_BUTTON_PRESSED;
                else if (menuText == "Sound: ON") {
                    menuButton[currentButtonID]->changeSoundButton(renderer);
                    Mix_Volume(-1, 0);
                }
                else if (menuText == "Sound: OFF") {
                    menuButton[currentButtonID]->changeSoundButton(renderer),
                    Mix_Volume(-1, MIX_MAX_VOLUME);
                }
        }
    }
}

bool Menu::isRunning() const {
    return currentMenuStatus == RUNNING;
}

int Menu::getStatus() const {
    return currentMenuStatus;
}

void Menu::reOpen() {
    currentMenuStatus = RUNNING;
    menuButton[currentButtonID]->setStatus(Button::BUTTON_IN);
}

void Menu::changeRunStatus() {
    running = !running;
}

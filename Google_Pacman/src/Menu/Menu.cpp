#include "Menu.h"

Menu::Menu(const int x, const int y, const int totalButton, const int buttonWidth, const int buttonHeight) {
    menuTexture  = nullptr;
    TOTAL_BUTTON = totalButton;
    MENU_WIDTH  = buttonWidth;
    MENU_HEIGHT = buttonHeight;
    this->x  = x;
    this->y = y;
}

Menu::~Menu() {
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

        for (int i = 0; i < TOTAL_BUTTON; i++)
            menuButton.push_back(new Button(MENU_WIDTH, MENU_HEIGHT, x - MENU_WIDTH / 2, y+ (5 + MENU_HEIGHT) * (i - 1)));

        for (int i = 0; i < TOTAL_BUTTON; i++) {
            if (buttonText[i] == "Sound: ON") {
                if (Mix_Volume(-1, -1) == 0) buttonText[i] = "Sound: OFF";
            }
            menuButton[i]->loadButton(renderer, buttonText[i]);
            menuButton[i]->setStatus(Button::BUTTON_OUT);
        }
        menuButton[0]->setStatus(Button::BUTTON_IN);
        buttonID = 0;
        menuStatus = RUNNING;

        Check->Status("Menu is OK");
    }
}


void Menu::render(SDL_Renderer* &renderer) {
    SDL_RenderCopy(renderer, menuTexture, nullptr, nullptr);
    for (int i = 0; i < TOTAL_BUTTON; i++)
        menuButton[i]->renderButton(renderer);
}

void Menu::handleEvent(SDL_Event &e, SDL_Renderer* &renderer) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_DOWN) {
                menuButton[buttonID]->setStatus(Button::BUTTON_OUT);
                (buttonID += 1) %= TOTAL_BUTTON;
                menuButton[buttonID]->setStatus(Button::BUTTON_IN);
        }
        else if (e.key.keysym.sym == SDLK_UP) {
                menuButton[buttonID]->setStatus(Button::BUTTON_OUT);
                (buttonID += TOTAL_BUTTON - 1) %= TOTAL_BUTTON;
                menuButton[buttonID]->setStatus(Button::BUTTON_IN);
        }
        else if (e.key.keysym.sym == SDLK_RETURN) {
                menuButton[buttonID]->setStatus(Button::BUTTON_IN);
                string menuText = menuButton[buttonID]->getText();
                if (menuText == "New Game" || menuText == "Start") menuStatus = PLAY_BUTTON_PRESSED;
                else if (menuText == "Resume") menuStatus = RESUME;
                else if (menuText == "Exit") menuStatus = EXIT_BUTTON_PRESSED;
                else if (menuText == "Exit to Start Menu") menuStatus = EXIT_BUTTON_PRESSED;
                else if (menuText == "Sound: ON") {
                    menuButton[buttonID]->changeSoundButton(renderer);
                    Mix_Volume(-1, 0);
                }
                else if (menuText == "Sound: OFF") {
                    menuButton[buttonID]->changeSoundButton(renderer),
                    Mix_Volume(-1, MIX_MAX_VOLUME);
                }
        }
    }
}

bool Menu::isRunning() const {
    return menuStatus == RUNNING;
}

int Menu::getStatus() const {
    return menuStatus;
}

void Menu::reOpen() {
    menuStatus = RUNNING;
    menuButton[buttonID]->setStatus(Button::BUTTON_IN);
}

void Menu::changeRunStatus() {
    running = !running;
}

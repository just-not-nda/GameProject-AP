#include "Button.h"

Button::Button(int width, int height, int x, int y)
{
    buttonRect = {x, y, width, height};
    normalText = new TextManager(24);
    selectText = new TextManager(24);
}

void Button::loadButton(SDL_Renderer* &renderer, string text) {
    if (text == "") return;
    normalText->loadRenderText(renderer, text, normalColor);
    selectText->loadRenderText(renderer, text, selectColor);
    buttonText = text;
}

void Button::renderButton(SDL_Renderer* &renderer) {
    if (buttonStatus == BUTTON_IN) {
        SDL_SetRenderDrawColor(renderer, 254, 233, 0, 255);
        SDL_RenderFillRect(renderer, &buttonRect);

        SDL_SetRenderDrawColor(renderer, selectColor.r, selectColor.g, selectColor.b, selectColor.a);
        selectText->renderText(renderer, buttonRect.x + buttonRect.w / 2, buttonRect.y + buttonRect.h / 2, TextManager::CENTER);

    }
    else if (buttonStatus == BUTTON_OUT) {
        SDL_SetRenderDrawColor(renderer, normalColor.r, normalColor.g, normalColor.b, normalColor.a);
        normalText->renderText(renderer, buttonRect.x + buttonRect.w / 2, buttonRect.y + buttonRect.h / 2, TextManager::CENTER);
    }
}

void Button::setStatus(const int status) {
    buttonStatus = status;
}

void Button::changeSoundButton(SDL_Renderer* &renderer) {
    if (buttonText == "Sound: ON") buttonText = "Sound: OFF";
    else buttonText = "Sound: ON";
    normalText->loadRenderText(renderer, buttonText, normalColor);
    selectText->loadRenderText(renderer, buttonText, selectColor);
    buttonStatus = BUTTON_IN;
}


std::string Button::getText() const {
    return buttonText;
}

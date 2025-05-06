#include "Button.h"

Button::Button(int Width, int Height, int scrPosX, int scrPosY)
{
    buttonRect = {scrPosX, scrPosY, Width, Height};
    normalText = new TextManager(24);
    selectText = new TextManager(24);
}

void Button::loadButton(SDL_Renderer* &renderer, string text) {
    if (text == "") return;
    normalText->loadRenderText(renderer, text, normalColor);
    selectText->loadRenderText(renderer, text, selectColor);
    bText = text;
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
    if (bText == "Sound: ON") bText = "Sound: OFF";
    else bText = "Sound: ON";
    normalText->loadRenderText(renderer, bText, normalColor);
    selectText->loadRenderText(renderer, bText, selectColor);
    buttonStatus = BUTTON_IN;
}


std::string Button::getText() const {
    return bText;
}

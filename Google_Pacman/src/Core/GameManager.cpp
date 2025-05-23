#include "GameManager.h"

GameManager::GameManager(SDL_Renderer* &renderer) {
    level = 1;
    life = 3;
    eatenDots = 0;
    eatenGhost = -1;
    scores = 0;
    playerDecision = WAITING;
    currentBut = 1;
    PINKY_COIN_LIMIT = 5;
    INKY_COIN_LIMIT = 30;
    CLYDE_COIN_LIMIT = 90;
    liveText = new TextManager(28);
    liveText->loadRenderText(renderer, "LIFE: ", {255, 255, 255, 255});
    scoreText = new TextManager(45);
    scoreText->loadRenderText(renderer, "0", {255, 255, 255, 255});
    levelText = new TextManager(28);
    levelText->loadRenderText(renderer, "LEVEL: 1", {255, 255, 255, 255});

    gameOver = loadImage(renderer, "assets/GameOver.png");
    againBut = new Button(280, 40, 325, 280); againBut->loadButton(renderer, "Play again");
    againBut->setStatus(Button::BUTTON_IN);
    quitBut  = new Button(280, 40, 325, 330); quitBut ->loadButton(renderer, "Quit");
    quitBut ->setStatus(Button::BUTTON_OUT);
}

GameManager::~GameManager() {
    delete levelText;
    levelText = nullptr;
    delete liveText;
    liveText = nullptr;
    delete scoreText;
    scoreText = nullptr;
}

SDL_Texture* GameManager::loadImage(SDL_Renderer* &renderer, const string imagePath) {
    SDL_Surface* Image = IMG_Load(imagePath.c_str());
    SDL_Texture* loadTexture = SDL_CreateTextureFromSurface(renderer, Image);
    SDL_FreeSurface(Image);
    return loadTexture;
}

void GameManager::reset() {
    level = 1;
    life = 3;
    scores = 0;
    eatenDots = 0;
    eatenGhost = -1;
    currentBut = 1;
    PINKY_COIN_LIMIT = 5;
    INKY_COIN_LIMIT = 30;
    CLYDE_COIN_LIMIT = 90;
    playerDecision = WAITING;
    againBut->setStatus(Button::BUTTON_IN);
    quitBut->setStatus(Button::BUTTON_OUT);
}

void GameManager::eatDots(const int type_of_dot) {
    ++eatenDots;
    if (type_of_dot == dot) scores += 10;
    else if (type_of_dot == power_pellect) {
        eatenGhost = -1;
        scores += 50;
    }
}

void GameManager::eatGhost(const int ghostTileX, const int ghostTileY) {
    (++eatenGhost) %= 4;
    switch (eatenGhost) {
        case 0: scores += 200; break;
        case 1: scores += 400; break;
        case 2: scores += 800; break;
        case 3: scores += 1600; break;
    }
    ghostEatenPosX = ghostTileX;
    ghostEatenPosY = ghostTileY;
}

void GameManager::lostALife() {
    --life;
}

int GameManager::getEatenGhostStreak() const {
    return eatenGhost;
}

int GameManager::getEatenGhostPosX() const {
    return ghostEatenPosX;
}

int GameManager::getEatenGhostPosY() const {
    return ghostEatenPosY;
}

int GameManager::getRemainLife() const {
    return life;
}

void GameManager::levelUp() {
    ++level;
    eatenDots = 0;
    eatenGhost = -1;
    currentBut = 1;
    playerDecision = WAITING;
}

bool GameManager::clearAllDots() const {
    return TOTAL_DOTS == eatenDots;
}

int GameManager::getLevel() const {
    return level;
}

void GameManager::handleGhostPos(Ghost* &pinky, Ghost* &inky, Ghost* &clyde) {
    if (pinky->isAtHome() && eatenDots >= PINKY_COIN_LIMIT) pinky->respawn(Ghost::GHOST_START_TILE_X, Ghost::GHOST_START_TILE_Y, false);
    if (inky ->isAtHome() && eatenDots >=  INKY_COIN_LIMIT) inky ->respawn(Ghost::GHOST_START_TILE_X, Ghost::GHOST_START_TILE_Y, false);
    if (clyde->isAtHome() && eatenDots >= CLYDE_COIN_LIMIT) clyde->respawn(Ghost::GHOST_START_TILE_X, Ghost::GHOST_START_TILE_Y, false);
}

void GameManager::renderHUD(SDL_Renderer* &renderer) {
    levelText->loadRenderText(renderer, "LEVEL: " + to_string(level), {255, 255, 255, 255});
    levelText->renderText(renderer, 0, 375, TextManager::LEFT);
    liveText->loadRenderText(renderer, "LIFE: " + to_string(life), {255, 255, 255, 255});
    liveText->renderText(renderer, 0, 405, TextManager::LEFT);
    scoreText->loadRenderText(renderer, to_string(scores), {255, 255, 255, 255});
    scoreText->renderText(renderer, 480, 80, TextManager::CENTER);
}

void GameManager::handleEGBoard(SDL_Event &e){
    if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_DOWN) {
                currentBut = 2;
                quitBut ->setStatus(Button::BUTTON_IN);
                againBut->setStatus(Button::BUTTON_OUT);
            }
            else if (e.key.keysym.sym == SDLK_UP) {
                currentBut = 1;
                againBut->setStatus(Button::BUTTON_IN);
                quitBut ->setStatus(Button::BUTTON_OUT);
            }
            else if (e.key.keysym.sym == SDLK_RETURN) {
                if (currentBut == 1) playerDecision = AGAIN;
                else playerDecision = QUIT;
            }
            return;
        }
}

void GameManager::runEGBoard(SDL_Renderer* &renderer){
    SDL_Rect dsRect = {0, 0, 930, 475};
    SDL_RenderCopy(renderer, gameOver, nullptr, &dsRect);
    againBut->renderButton(renderer);
    quitBut ->renderButton(renderer);

    scoreText->loadRenderText(renderer, to_string(scores), {234, 67, 53, 255});
    scoreText->renderText(renderer, 470, 220, TextManager::CENTER);
}

int GameManager::getPlayerDecision() const {
    return playerDecision;
}

int GameManager::getRemainDot() const {
    return TOTAL_DOTS - eatenDots;
}

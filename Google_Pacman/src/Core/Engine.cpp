#include "Engine.h"
#include <iostream>
#include <utility>
#include <limits>
#include <random>
#include <time.h>

typedef std::pair<int, int> II;
typedef std::pair<int, std::pair<int, int> > IP;

void Engine::init(SDL_Renderer* &renderer)
{

    map = new Map();

    texture = new Texture();
    texture->loadTile(renderer);
    texture->loadCharacterTextures(renderer);

    timeManager = new TimeManager();
    gameManager = new GameManager(renderer);
    audioManager = new AudioManager();
    audioManager->loadSound();
    srand(time(nullptr));
    newGame();
    SDL_Surface* Image = IMG_Load("assets/levelUp.png");
    levelUp = SDL_CreateTextureFromSurface(renderer, Image);
    SDL_FreeSurface(Image);
    Image = IMG_Load("assets/ready.png");
    ready = SDL_CreateTextureFromSurface(renderer, Image);
    SDL_FreeSurface(Image);
}

void Engine::newGame() {
    map->reset();
    gameManager->reset();
    delete pacman;
    pacman = new Pacman();
    delete blinky;
    blinky = new Ghost(Ghost::BLINKY_START_TILE_X, Ghost::BLINKY_START_TILE_Y, false);
    delete pinky;
    pinky  = new Ghost(Ghost::PINKY_START_TILE_X, Ghost::PINKY_START_TILE_Y, true);
    delete inky;
    inky   = new Ghost(Ghost::INKY_START_TILE_X, Ghost::INKY_START_TILE_Y, true);
    delete clyde;
    clyde  = new Ghost(Ghost::CLYDE_START_TILE_X, Ghost::CLYDE_START_TILE_Y, true);

    audioManager->insertPlayList(AudioManager::START);
    timeManager->resetTick(gameManager->getLevel());
    timeManager->pauseTick(true);
    runningEGBoard = false;
}

void Engine::respawnObject() {
    delete pacman;
    pacman = new Pacman();
    delete blinky;
    blinky = new Ghost(Ghost::BLINKY_START_TILE_X, Ghost::BLINKY_START_TILE_Y, false);
    delete pinky;
    pinky  = new Ghost(Ghost::PINKY_START_TILE_X, Ghost::PINKY_START_TILE_Y, true);
    delete inky;
    inky   = new Ghost(Ghost::INKY_START_TILE_X, Ghost::INKY_START_TILE_Y, true);
    delete clyde;
    clyde  = new Ghost(Ghost::CLYDE_START_TILE_X, Ghost::INKY_START_TILE_Y, true);
    audioManager->reset();
    timeManager->pauseTick(false);
}


void Engine::handleEvent(SDL_Event &e) {
    if (Mix_Playing(2) || Mix_Playing(4)) return;

    if (pacman->isDead()) {
        if (runningEGBoard) gameManager->handleEGBoard(e);
        return;
    }
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_DOWN || e.key.keysym.sym == SDLK_UP ||
            e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_RIGHT) {

            int newDir  = -1;
            int lastDir = -1;
            int pacmanTileX = pacman->getTileX();
            int pacmanTileY = pacman->getTileY();
            int pacmanPosX  = pacman->getPosX();
            int pacmanPosY  = pacman->getPosY();

            if (!pacman->emptyDirStack())
                lastDir = pacman->getDir();

            if (e.key.keysym.sym == SDLK_UP)
                newDir = 0;
            else if (e.key.keysym.sym == SDLK_RIGHT)
                newDir = 1;
            else if (e.key.keysym.sym == SDLK_DOWN)
                newDir = 2;
            else if (e.key.keysym.sym == SDLK_LEFT)
                newDir = 3;

            if (lastDir == -1) {
                if (map->canChangeDir(pacmanTileX, pacmanTileY, newDir)) {
                    pacman->eraseSpecial();
                    pacman->pushtoStack(newDir);
                }
            }
            else {
                if (newDir % 2 == lastDir % 2) {
                    if (map->canChangeDir(pacmanTileX, pacmanTileY, newDir)) {
                        pacman->pushtoStack(newDir);
                        pacman->eraseSpecial();
                    }
                }
                else {
                    pair<int, int> nextCross = map->getnextCrossID(pacmanTileX, pacmanTileY, lastDir);
                    if (lastDir % 2 == 1 && newDir % 2 == 0) {
                        if (pacmanPosY == pacmanTileY * 16) {
                            if (map->canChangeDir(pacmanTileX, pacmanTileY, newDir)) {
                                pacman->pushSpecialStack(newDir, II(pacmanTileX, pacmanTileY));
                            }
                            else if (nextCross != II(-1, -1) &&
                                     !map->besideCrossIsWall(nextCross, newDir) &&
                                     abs(pacmanPosX - nextCross.first * 16) <= 32) {
                                pacman->pushSpecialStack(newDir, nextCross);
                            }
                        }
                    }
                    else if (lastDir % 2 == 0 && newDir % 2 == 1) {
                        if (pacmanPosX == pacmanTileX * 16) {
                            if (map->canChangeDir(pacmanTileX, pacmanTileY, newDir)) {
                                pacman->pushSpecialStack(newDir, II(pacmanTileX, pacmanTileY));
                            }
                            else if (nextCross != II(-1, -1) &&
                                     !map->besideCrossIsWall(nextCross, newDir) &&
                                     abs(pacmanPosY - nextCross.second * 16) <= 32) {
                                pacman->pushSpecialStack(newDir, nextCross);
                            }
                        }
                    }
                }
            }
        }
    }
}



void Engine::render(SDL_Renderer* &renderer)
{
    timeManager->stablizeFPS();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect dstRect;
    for (int row = 0; row < 17; ++row) {
        for (int col = 0; col < 58; ++col) {
            dstRect = { col * 16, row * 16 + 100, 16, 16 };
            int tileID = map->getTileID(col, row);
            texture->renderTile(renderer, tileID, &dstRect);
        }
    }

    if (!runningEGBoard) {
        int dir = -1;
        if (!pacman->emptyDirStack()) dir = pacman->getDir();
        if (!pacman->isDead()) {
            renderGhost(renderer, blinky, Texture::BLINKY);
            renderGhost(renderer, pinky , Texture::PINKY );
            renderGhost(renderer, inky  , Texture::INKY  );
            renderGhost(renderer, clyde , Texture::CLYDE );
            if (Mix_Playing(2)) {
                dstRect = {515, 254, 120, 30};
                SDL_RenderCopy(renderer, ready, nullptr, &dstRect);
            }
        }
        if (pacman->isDead()) {
            if (texture->pacmanIsDead()) {
                if (gameManager->getRemainLife() > 0) respawnObject();
                else {
                    runningEGBoard = true;
                }
                    }
            texture->renderPacman(renderer, pacman->getPosX(), pacman->getPosY(), Texture::PACMAN_DEAD_STATE);
        }
        else texture->renderPacman(renderer, pacman->getPosX(), pacman->getPosY(), dir);
        if (waitTime > 0) {
            dstRect = {315, 88, 300, 300};
            SDL_RenderCopy(renderer, levelUp, nullptr, &dstRect);
        }
        if (Mix_Playing(4)) {
            texture->renderGhostScore(renderer, gameManager->getEatenGhostPosX(), gameManager->getEatenGhostPosY(), gameManager->getEatenGhostStreak());
        }
        audioManager->playSound();

    }
    if (runningEGBoard) gameManager->runEGBoard(renderer);
    else gameManager->renderHUD(renderer);
}


void Engine::loop(bool &exitToMenu)
{
    if (gameManager->clearAllCoins()) {
        if (waitTime > 0) --waitTime;
        else {
            gameManager->levelUp();
            timeManager->resetTick(gameManager->getLevel());
            respawnObject();
            map->reset();
        }
        return;
    }
    if (Mix_Playing(2) || Mix_Playing(4)) {
        if (Mix_Playing(2)) timeManager->pauseTick(true);
        return;
    }
    if (pacman->isDead()) {
        if (runningEGBoard) {
            switch (gameManager->getPlayerDecision()) {
                case GameManager::AGAIN:
                    newGame();
                    break;
                case GameManager::QUIT:
                    exitToMenu = true; break;
            }
        }
        return;
    }
    timeManager->updateStatus();
    int pacmanTileX = pacman->getTileX();
    int pacmanTileY = pacman->getTileY();
    int pacmanPosX = pacman->getPosX();
    int pacmanPosY = pacman->getPosY();
    int lastDir = -1;
    if (!pacman->emptyDirStack()) lastDir = pacman->getDir();

    if (!pacman->isDead() && lastDir != -1) {
        if (pacmanTileX * 16 == pacmanPosX && pacmanTileY * 16 == pacmanPosY) {
            if (map->iscrossRoad(pacmanTileX, pacmanTileY)) {
                if (!pacman->emptySpecial() && pacman->getSpecial() == II(pacmanTileX, pacmanTileY)) pacman->turn();
            }
            if (map->canChangeDir(pacmanTileX, pacmanTileY, pacman->getDir())) pacman->moving();
            else pacman->stopmoving();
        }
        else {
            if (map->canChangeDir(pacmanTileX, pacmanTileY, lastDir)) pacman->moving();
            else {
                if (pacmanTileX * 16 == pacmanPosX && pacmanTileY * 16 != pacmanPosY) pacman->moving();
                else if (pacmanTileX * 16 != pacmanPosX && pacmanTileY * 16 == pacmanPosY) pacman->moving();
                else pacman->stopmoving();
            }
        }
    }

    int remainCoin = gameManager->getRemainCoin();
    if (remainCoin < 50) audioManager->insertPlayList(AudioManager::MOVE_3);
    else if (remainCoin < 100) audioManager->insertPlayList(AudioManager::MOVE_2);
    else if (remainCoin < 150) audioManager->insertPlayList(AudioManager::MOVE_1);
    else audioManager->insertPlayList(AudioManager::MOVE_0);

    pacmanTileX = pacman->getTileX();
    pacmanTileY = pacman->getTileY();
    int typeOfCoin = map->eatCoins(pacmanTileX, pacmanTileY);

    if (typeOfCoin != GameManager::notCoin) {
        gameManager->eatCoins(typeOfCoin);
        audioManager->insertPlayList(AudioManager::EAT_DOT);
        if (typeOfCoin == GameManager::superCoin) {
            timeManager->setFrightenTime();
            audioManager->insertPlayList(AudioManager::GHOST_TURN_BLUE);
            if (!blinky->isDead()) blinky->setFrighten(true);
            if (!pinky ->isDead()) pinky ->setFrighten(true);
            if (!inky  ->isDead()) inky  ->setFrighten(true);
            if (!clyde ->isDead()) clyde ->setFrighten(true);
        }
    }
    if (!timeManager->isFrightenTime()) {
        audioManager->insertPlayList(AudioManager::NORMAL_GHOST);
        blinky->setFrighten(false);
        pinky ->setFrighten(false);
        inky  ->setFrighten(false);
        clyde ->setFrighten(false);
    }
    bool scatter = timeManager->isScatteringTime();
    blinky->setScattering(scatter);
    pinky ->setScattering(scatter);
    inky  ->setScattering(scatter);
    clyde ->setScattering(scatter);


    pacmanPosX = pacman->getPosX();
    pacmanPosY = pacman->getPosY();
    lastDir = -1;
    if (!pacman->emptyDirStack()) lastDir = pacman->getDir();

    if (!pacman->isDead()) {
        timeManager->pauseTick(false);
        if (blinky->isDead())
            blinky->setDestination(35, 4);
        else if (!blinky->isScattering())
            blinky->setDestination(pacmanTileX, pacmanTileY);
        else blinky->setDestination(Ghost::DEFAULT_BLINKY_TILE_X, Ghost::DEFAULT_BLINKY_TILE_Y);

        if (pinky->isDead())
            pinky->setDestination(35, 4);
        else if (!pinky->isScattering()) {
            switch (lastDir) {
                case Map::UP:
                    pinky->setDestination(pacmanTileX, pacmanTileY - 4);
                    break;
                case Map::DOWN:
                    pinky->setDestination(pacmanTileX, pacmanTileY + 4);
                    break;
                case Map::LEFT:
                    pinky->setDestination(pacmanTileX - 4, pacmanTileY);
                    break;
                case Map::RIGHT:
                    pinky->setDestination(pacmanTileX + 4, pacmanTileY);
                    break;
            }
        }
        else pinky->setDestination(Ghost::DEFAULT_PINKY_TILE_X, Ghost::DEFAULT_PINKY_TILE_Y);

        if (inky->isDead())
            inky->setDestination(35, 4);
        else if (!inky->isScattering())
            inky->setDestination(2 * pacmanTileX - blinky->getTileX(), 2 * pacmanTileY - blinky->getTileY());
        else inky->setDestination(Ghost::DEFAULT_INKY_TILE_X, Ghost::DEFAULT_INKY_TILE_Y);

        if (clyde->isDead())
            clyde->setDestination(35, 4);
        else if (!clyde->isScattering()) {
            if ((pacmanTileX - clyde->getTileX()) * (pacmanTileX - clyde->getTileX()) + (pacmanTileY - clyde->getTileY()) * (pacmanTileY - clyde->getTileY()) <= 64)
                clyde->setDestination(Ghost::DEFAULT_CLYDE_TILE_X, Ghost::DEFAULT_CLYDE_TILE_Y);
            else
                clyde->setDestination(pacmanTileX, pacmanTileY);
        }
        else clyde->setDestination(Ghost::DEFAULT_CLYDE_TILE_X, Ghost::DEFAULT_CLYDE_TILE_Y);

    }
    pacman->teleport();
    ghostMove(blinky);
    ghostMove(pinky);
    ghostMove(inky);
    ghostMove(clyde);

    gameManager->handleGhostPos(pinky, inky, clyde);

    if (gameManager->clearAllCoins()) {
        audioManager->insertPlayList(AudioManager::WINNING);
        waitTime = 100;
    }
}

void Engine::ghostMove(Ghost* &ghost) {
    if (ghost == nullptr) return;
    int ghostTileX = ghost->getTileX();
    int ghostTileY = ghost->getTileY();
    int ghostPosX  = ghost->getPosX();
    int ghostPosY  = ghost->getPosY();
    int ghostOldDir = ghost->getGhostDir();
    int ghostNextTileX = ghost->getNextTileX();
    int ghostNextTileY = ghost->getNextTileY();

    if (ghostTileX * 16 == ghostPosX && ghostTileY * 16 == ghostPosY) {

            int validDirs = 0;
            int lastValidDir = -1;
            for (int d = 0; d < 4; ++d) {
                if (map->canChangeDir(ghostTileX, ghostTileY, d)) {
                    ++validDirs;
                    lastValidDir = d;
                }
            }
            if (validDirs == 1 && lastValidDir != -1 && lastValidDir != ghostOldDir) {
                ghost->setDir(lastValidDir);
            }

        if (map->iscrossRoad(ghostTileX, ghostTileY)) {
            if (ghost->isFrighten()) {

            stack<int> whichDir;
                if (ghostOldDir % 2 == 1) {
                    if (map->canChangeDir(ghostTileX, ghostTileY, Map::UP)) whichDir.push(0);
                    if (map->canChangeDir(ghostTileX, ghostTileY, Map::DOWN)) whichDir.push(2);
                    if (map->canChangeDir(ghostTileX, ghostTileY, ghostOldDir)) whichDir.push(ghostOldDir);
                }
                else {
                    if (map->canChangeDir(ghostTileX, ghostTileY, Map::LEFT)) whichDir.push(3);
                    if (map->canChangeDir(ghostTileX, ghostTileY, Map::RIGHT)) whichDir.push(1);
                    if (map->canChangeDir(ghostTileX, ghostTileY, ghostOldDir)) whichDir.push(ghostOldDir);
                }
                int dir = rand() % (int) whichDir.size() + 1;
                while (dir > 1) whichDir.pop(), --dir;
                ghost->setDir(whichDir.top());
                while (!whichDir.empty()) whichDir.pop();
            }
            else {
                int distanceUP, distanceDOWN, distanceLEFT, distanceRIGHT;
                distanceUP = distanceDOWN = distanceLEFT = distanceRIGHT = __INT32_MAX__;

                if (map->canChangeDir(ghostTileX, ghostTileY, Map::UP))
                    distanceUP = map->getDist(II(ghostTileX, ghostTileY - 1), II(ghostNextTileX, ghostNextTileY), Map::UP);

                if (map->canChangeDir(ghostTileX, ghostTileY, Map::DOWN))
                    distanceDOWN = map->getDist(II(ghostTileX, ghostTileY + 1), II(ghostNextTileX, ghostNextTileY), Map::DOWN);

                if (map->canChangeDir(ghostTileX, ghostTileY, Map::LEFT))
                    distanceLEFT = map->getDist(II(ghostTileX - 1, ghostTileY), II(ghostNextTileX, ghostNextTileY), Map::LEFT);

                if (map->canChangeDir(ghostTileX, ghostTileY, Map::RIGHT))
                    distanceRIGHT = map->getDist(II(ghostTileX + 1, ghostTileY), II(ghostNextTileX, ghostNextTileY), Map::RIGHT);

            int distanceMIN;
                if (ghostOldDir == Map::UP) {
                    distanceMIN = std::min(distanceUP, std::min(distanceLEFT, distanceRIGHT));
                    if (distanceMIN == distanceUP) ghost->setDir(Map::UP);
                    else if (distanceMIN == distanceLEFT) ghost->setDir(Map::LEFT);
                    else ghost->setDir(Map::RIGHT);
                }
                else if (ghostOldDir == Map::DOWN) {
                    distanceMIN = std::min(distanceDOWN, std::min(distanceLEFT, distanceRIGHT));
                    if (distanceMIN == distanceDOWN) ghost->setDir(Map::DOWN);
                    else if (distanceMIN == distanceLEFT) ghost->setDir(Map::LEFT);
                    else ghost->setDir(Map::RIGHT);
                }
                else if (ghostOldDir == Map::LEFT) {
                    distanceMIN = std::min(distanceUP, std::min(distanceDOWN, distanceLEFT));
                    if (distanceMIN == distanceUP) ghost->setDir(Map::UP);
                    else if (distanceMIN == distanceDOWN) ghost->setDir(Map::DOWN);
                    else ghost->setDir(Map::LEFT);
                }
                else if (ghostOldDir == Map::RIGHT) {
                    distanceMIN = std::min(distanceUP, std::min(distanceRIGHT, distanceDOWN));
                    if (distanceMIN == distanceUP) ghost->setDir(Map::UP);
                    else if (distanceMIN == distanceRIGHT) ghost->setDir(Map::RIGHT);
                    else ghost->setDir(Map::DOWN);
                }
            }
        }
        if (map->canChangeDir(ghostTileX, ghostTileY, ghost->getGhostDir())) ghost->moving();
    }
    else {
        if (map->canChangeDir(ghostTileX, ghostTileY, ghost->getGhostDir())) ghost->moving();
        else {
            if (ghostTileX * 16 == ghostPosX && ghostTileY * 16 != ghostPosY && ghost->getGhostDir() % 2 == 0) ghost->moving();
            else if (ghostTileY * 16 == ghostPosY && ghostTileX * 16 != ghostPosX && ghost->getGhostDir() % 2 == 1) ghost->moving();
        }
    }
    ghost->teleport();
    if (ghostPosX == ghostNextTileX * 16 && ghostPosY == ghostNextTileY * 16) {
        if (ghost->isDead()) {
            ghost->setDead(false);
            audioManager->insertPlayList(AudioManager::REVIVAL_GHOST);
        }
    }
    pacmanMeetGhost(ghost);
}

void Engine::pacmanMeetGhost(Ghost* &ghost) {
    if (ghost->isDead()) return;
    int dx = pacman->getPosX() - ghost->getPosX();
    int dy = pacman->getPosY() - ghost->getPosY();
    int distSq = dx * dx + dy * dy;
    if (distSq <= 81) {
        if (ghost->isFrighten()) {
            gameManager->eatGhost(ghost->getPosX(), ghost->getPosY());
            ghost->setDead(true);
            ghost->setFrighten(false);
            audioManager->insertPlayList(AudioManager::EAT_GHOST);
            audioManager->insertPlayList(AudioManager::GHOST_GO_HOME);
        }
        else {
            pacman->setDead(true, 1);
            gameManager->lostALife();
            audioManager->insertPlayList(AudioManager::DEAD);
            timeManager->pauseTick(true);
        }
    }
}

void Engine::renderGhost(SDL_Renderer* &renderer, Ghost* &ghost, int ghostID) {
    if (ghost == nullptr) return;
    if (ghost->isDead())
    {
        texture->renderGhost(renderer, ghost->getPosX(), ghost->getPosY(), Texture::GHOST_SPIRIT, ghost->getGhostDir());
        texture->renderGhostScore(renderer, gameManager->getEatenGhostPosX(), gameManager->getEatenGhostPosY(), gameManager->getEatenGhostStreak());
    }

    else if (ghost->isFrighten()) {
        if (timeManager->remainFrightenTime() < 2.0)
            texture->renderGhost(renderer, ghost->getPosX(), ghost->getPosY(), ghostID, Texture::GHOST_FRIGHTEN_2);
        else texture->renderGhost(renderer, ghost->getPosX(), ghost->getPosY(), ghostID, Texture::GHOST_FRIGHTEN_1);
    }
    else texture->renderGhost(renderer, ghost->getPosX(), ghost->getPosY(), ghostID, ghost->getGhostDir());
}



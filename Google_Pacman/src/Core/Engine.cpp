#include "Engine.h"

typedef pair<int, int> II;
typedef pair<int, pair<int, int> > IP;

void Engine::init(SDL_Renderer* &renderer)
{

    _map = new Map();

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
    _map->reset();
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
    timeManager->reset(gameManager->getLevel());
    timeManager->pauseTime(true);
    runGameOver = false;
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
    timeManager->pauseTime(false);
}


void Engine::handleEvent(SDL_Event &e) {
    if (Mix_Playing(2) || Mix_Playing(4)) return;

    if (pacman->isDead()) {
        if (runGameOver) gameManager->handleEGBoard(e);
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

            if (!pacman->emptyDir())
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
                if (_map->canChangeDir(pacmanTileX, pacmanTileY, newDir)) {
                    pacman->eraseTurnPoints();
                    pacman->updateDir(newDir);
                }
            }
            else {
                if (newDir % 2 == lastDir % 2) {
                    if (_map->canChangeDir(pacmanTileX, pacmanTileY, newDir)) {
                        pacman->updateDir(newDir);
                        pacman->eraseTurnPoints();
                    }
                }
                else {
                    pair<int, int> nextCross = _map->getnextCrossID(pacmanTileX, pacmanTileY, lastDir);
                    if (lastDir % 2 == 1 && newDir % 2 == 0) {
                        if (pacmanPosY == pacmanTileY * 16) {
                            if (_map->canChangeDir(pacmanTileX, pacmanTileY, newDir)) {
                                pacman->addTurn(newDir, II(pacmanTileX, pacmanTileY));
                            }
                            else if (nextCross != II(-1, -1) && !_map->besideCrossIsWall(nextCross, newDir) && abs(pacmanPosX - nextCross.first * 16) <= 32) {
                                pacman->addTurn(newDir, nextCross);
                            }
                        }
                    }
                    else if (lastDir % 2 == 0 && newDir % 2 == 1) {
                        if (pacmanPosX == pacmanTileX * 16) {
                            if (_map->canChangeDir(pacmanTileX, pacmanTileY, newDir)) {
                                pacman->addTurn(newDir, II(pacmanTileX, pacmanTileY));
                            }
                            else if (nextCross != II(-1, -1) && !_map->besideCrossIsWall(nextCross, newDir) && abs(pacmanPosY - nextCross.second * 16) <= 32) {
                                pacman->addTurn(newDir, nextCross);
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
            int tileID = _map->getTileID(col, row);
            texture->renderTile(renderer, tileID, &dstRect);
        }
    }

    if (!runGameOver) {
        int dir = -1;
        if (!pacman->emptyDir()) dir = pacman->getDir();
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
                    runGameOver = true;
                }
                    }
            texture->renderPacman(renderer, pacman->getPosX(), pacman->getPosY(), Texture::PACMAN_DEAD_STATE);
        }
        else texture->renderPacman(renderer, pacman->getPosX(), pacman->getPosY(), dir);
        if (delayTime > 0) {
            dstRect = {315, 108, 300, 200};
            SDL_RenderCopy(renderer, levelUp, nullptr, &dstRect);
        }
        if (Mix_Playing(4)) {
            texture->renderGhostScore(renderer, gameManager->getEatenGhostPosX(), gameManager->getEatenGhostPosY(), gameManager->getEatenGhostStreak());
        }
        audioManager->playSound();

    }
    if (runGameOver) gameManager->runEGBoard(renderer);
    else gameManager->renderHUD(renderer);
}


void Engine::loop(bool &exitToMenu)
{
    if (gameManager->clearAllDots()) {
        if (delayTime > 0) --delayTime;
        else {
            gameManager->levelUp();
            timeManager->reset(gameManager->getLevel());
            respawnObject();
            _map->reset();
        }
        return;
    }
    if (Mix_Playing(2) || Mix_Playing(4)) {
        if (Mix_Playing(2)) timeManager->pauseTime(true);
        return;
    }
    if (pacman->isDead()) {
        if (runGameOver) {
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
    if (!pacman->emptyDir()) lastDir = pacman->getDir();

    if (!pacman->isDead() && lastDir != -1) {
        if (pacmanTileX * 16 == pacmanPosX && pacmanTileY * 16 == pacmanPosY) {
            if (_map->iscrossRoad(pacmanTileX, pacmanTileY)) {
                if (!pacman->emptyTurnPoints() && pacman->getTurnPoints() == II(pacmanTileX, pacmanTileY)) pacman->turn();
            }
            if (_map->canChangeDir(pacmanTileX, pacmanTileY, pacman->getDir())) pacman->moving();
            else pacman->stopmoving();
        }
        else {
            if (_map->canChangeDir(pacmanTileX, pacmanTileY, lastDir)) pacman->moving();
            else {
                if (pacmanTileX * 16 == pacmanPosX && pacmanTileY * 16 != pacmanPosY) pacman->moving();
                else if (pacmanTileX * 16 != pacmanPosX && pacmanTileY * 16 == pacmanPosY) pacman->moving();
                else pacman->stopmoving();
            }
        }
    }

    int remainCoin = gameManager->getRemainDot();
    if (remainCoin < 50) audioManager->insertPlayList(AudioManager::MOVE_3);
    else if (remainCoin < 100) audioManager->insertPlayList(AudioManager::MOVE_2);
    else if (remainCoin < 150) audioManager->insertPlayList(AudioManager::MOVE_1);
    else audioManager->insertPlayList(AudioManager::MOVE_0);

    pacmanTileX = pacman->getTileX();
    pacmanTileY = pacman->getTileY();
    int type_of_dot = _map->eatDots(pacmanTileX, pacmanTileY);

    if (type_of_dot != GameManager::notDot) {
        gameManager->eatDots(type_of_dot);
        audioManager->insertPlayList(AudioManager::EAT_DOT);
        if (type_of_dot == GameManager::power_pellect) {
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
    if (!pacman->emptyDir()) lastDir = pacman->getDir();

    if (!pacman->isDead()) {
        timeManager->pauseTime(false);
        if (blinky->isDead())
            blinky->setTargetTile(35, 4);
        else if (!blinky->isScattering())
            blinky->setTargetTile(pacmanTileX, pacmanTileY);
        else blinky->setTargetTile(Ghost::DEFAULT_BLINKY_TILE_X, Ghost::DEFAULT_BLINKY_TILE_Y);

        if (pinky->isDead())
            pinky->setTargetTile(35, 4);
        else if (!pinky->isScattering()) {
            switch (lastDir) {
                case Map::UP:
                    pinky->setTargetTile(pacmanTileX, pacmanTileY - 4);
                    break;
                case Map::DOWN:
                    pinky->setTargetTile(pacmanTileX, pacmanTileY + 4);
                    break;
                case Map::LEFT:
                    pinky->setTargetTile(pacmanTileX - 4, pacmanTileY);
                    break;
                case Map::RIGHT:
                    pinky->setTargetTile(pacmanTileX + 4, pacmanTileY);
                    break;
            }
        }
        else pinky->setTargetTile(Ghost::DEFAULT_PINKY_TILE_X, Ghost::DEFAULT_PINKY_TILE_Y);

        if (inky->isDead())
            inky->setTargetTile(35, 4);
        else if (!inky->isScattering())
            inky->setTargetTile(2 * pacmanTileX - blinky->getTileX(), 2 * pacmanTileY - blinky->getTileY());
        else inky->setTargetTile(Ghost::DEFAULT_INKY_TILE_X, Ghost::DEFAULT_INKY_TILE_Y);

        if (clyde->isDead())
            clyde->setTargetTile(35, 4);
        else if (!clyde->isScattering()) {
            if ((pacmanTileX - clyde->getTileX()) * (pacmanTileX - clyde->getTileX()) + (pacmanTileY - clyde->getTileY()) * (pacmanTileY - clyde->getTileY()) <= 64)
                clyde->setTargetTile(Ghost::DEFAULT_CLYDE_TILE_X, Ghost::DEFAULT_CLYDE_TILE_Y);
            else
                clyde->setTargetTile(pacmanTileX, pacmanTileY);
        }
        else clyde->setTargetTile(Ghost::DEFAULT_CLYDE_TILE_X, Ghost::DEFAULT_CLYDE_TILE_Y);

    }
    pacman->teleport();
    ghostGo(blinky);
    ghostGo(pinky);
    ghostGo(inky);
    ghostGo(clyde);

    gameManager->handleGhostPos(pinky, inky, clyde);

    if (gameManager->clearAllDots()) {
        audioManager->insertPlayList(AudioManager::WINNING);
        delayTime = 100;
    }
}

void Engine::ghostGo(Ghost* &ghost) {
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
                if (_map->canChangeDir(ghostTileX, ghostTileY, d)) {
                    ++validDirs;
                    lastValidDir = d;
                }
            }
            if (validDirs == 1 && lastValidDir != -1 && lastValidDir != ghostOldDir) {
                ghost->setDir(lastValidDir);
            }

        if (_map->iscrossRoad(ghostTileX, ghostTileY)) {
            if (ghost->isFrighten()) {

            stack<int> movableDirs;
                if (ghostOldDir % 2 == 1) {
                    if (_map->canChangeDir(ghostTileX, ghostTileY, Map::UP)) movableDirs.push(0);
                    if (_map->canChangeDir(ghostTileX, ghostTileY, Map::DOWN)) movableDirs.push(2);
                    if (_map->canChangeDir(ghostTileX, ghostTileY, ghostOldDir)) movableDirs.push(ghostOldDir);
                }
                else {
                    if (_map->canChangeDir(ghostTileX, ghostTileY, Map::LEFT)) movableDirs.push(3);
                    if (_map->canChangeDir(ghostTileX, ghostTileY, Map::RIGHT)) movableDirs.push(1);
                    if (_map->canChangeDir(ghostTileX, ghostTileY, ghostOldDir)) movableDirs.push(ghostOldDir);
                }
                int dir = rand() % (int) movableDirs.size() + 1;
                while (dir > 1) movableDirs.pop(), --dir;
                ghost->setDir(movableDirs.top());
                while (!movableDirs.empty()) movableDirs.pop();
            }
            else {
                int distanceUP, distanceDOWN, distanceLEFT, distanceRIGHT;
                distanceUP = distanceDOWN = distanceLEFT = distanceRIGHT = __INT32_MAX__;

                if (_map->canChangeDir(ghostTileX, ghostTileY, Map::UP))
                    distanceUP = _map->getDist(II(ghostTileX, ghostTileY - 1), II(ghostNextTileX, ghostNextTileY), Map::UP);

                if (_map->canChangeDir(ghostTileX, ghostTileY, Map::DOWN))
                    distanceDOWN = _map->getDist(II(ghostTileX, ghostTileY + 1), II(ghostNextTileX, ghostNextTileY), Map::DOWN);

                if (_map->canChangeDir(ghostTileX, ghostTileY, Map::LEFT))
                    distanceLEFT = _map->getDist(II(ghostTileX - 1, ghostTileY), II(ghostNextTileX, ghostNextTileY), Map::LEFT);

                if (_map->canChangeDir(ghostTileX, ghostTileY, Map::RIGHT))
                    distanceRIGHT = _map->getDist(II(ghostTileX + 1, ghostTileY), II(ghostNextTileX, ghostNextTileY), Map::RIGHT);

            int distanceMIN;
                if (ghostOldDir == Map::UP) {
                    distanceMIN = min(distanceUP, min(distanceLEFT, distanceRIGHT));
                    if (distanceMIN == distanceUP) ghost->setDir(Map::UP);
                    else if (distanceMIN == distanceLEFT) ghost->setDir(Map::LEFT);
                    else ghost->setDir(Map::RIGHT);
                }
                else if (ghostOldDir == Map::DOWN) {
                    distanceMIN = min(distanceDOWN, min(distanceLEFT, distanceRIGHT));
                    if (distanceMIN == distanceDOWN) ghost->setDir(Map::DOWN);
                    else if (distanceMIN == distanceLEFT) ghost->setDir(Map::LEFT);
                    else ghost->setDir(Map::RIGHT);
                }
                else if (ghostOldDir == Map::LEFT) {
                    distanceMIN = min(distanceUP, min(distanceDOWN, distanceLEFT));
                    if (distanceMIN == distanceUP) ghost->setDir(Map::UP);
                    else if (distanceMIN == distanceDOWN) ghost->setDir(Map::DOWN);
                    else ghost->setDir(Map::LEFT);
                }
                else if (ghostOldDir == Map::RIGHT) {
                    distanceMIN = min(distanceUP, min(distanceRIGHT, distanceDOWN));
                    if (distanceMIN == distanceUP) ghost->setDir(Map::UP);
                    else if (distanceMIN == distanceRIGHT) ghost->setDir(Map::RIGHT);
                    else ghost->setDir(Map::DOWN);
                }
            }
        }
        if (_map->canChangeDir(ghostTileX, ghostTileY, ghost->getGhostDir())) ghost->moving();
    }
    else {
        if (_map->canChangeDir(ghostTileX, ghostTileY, ghost->getGhostDir())) ghost->moving();
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
            timeManager->pauseTime(true);
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



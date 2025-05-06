#include "GameCore.h"

using namespace std;

int main(int argc, char* argv[])
{
    GameCore* gamecore = new GameCore();
    gamecore->initSDL();
    gamecore->runGame();
    gamecore->quitSDL();
    return 0;
}

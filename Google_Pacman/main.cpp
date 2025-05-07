#include "GameCore.h"

using namespace std;

int main(int argc, char* argv[])
{
    GameCore* gamecore = new GameCore();
    gamecore->init();
    gamecore->run();
    gamecore->quit();
    return 0;
}

#include "Engine.h"

using namespace std;

int main(int argc, char* argv[])
{
    Engine engine;
    if(!engine.init())return -1;
    engine.render();
    return 0;
}

#include "Engine.h"

using namespace std;

int main(int argc, char* argv[])
{
    Engine engine;
    if(!engine.init())return -1;
    engine.run();
    return 0;
}

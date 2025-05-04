#ifndef PACMAN_H
#define PACMAN_H

#include "Character.h"

#include <stack>
using namespace std;

class Pacman : public Character
{
    private:
        stack <int> direction;

    public:
        static const int pacman_velocity = 2;

        Pacman();
        ~Pacman();

};

#endif // PACMAN_H

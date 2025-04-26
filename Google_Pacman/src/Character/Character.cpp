#include "Character.h"


Character::Character(int tile_x, int tile_y, int velocity_x, int velocity_y)
{
    this->tile_x = tile_x;
    this->tile_y = tile_y;
    if(velocity_x != 0)this->velocity_x = velocity_x;
    if(velocity_y != 0)this->velocity_y = velocity_y;
    this->pos_x = tile_x * 16 + 8;
    this->pos_y = tile_y * 16;
    dead = false;
}

bool Character::isDead() const
{
    return dead;
}

int Character::getPosX() const
{
    return pos_x;
}

int Character::getPosY() const
{
    return pos_y;
}

int Character::getTileX() const
{
    return tile_x;
}

int Character::getTileY() const
{
    return tile_y;
}

void Character::move()
{
    pos_x += velocity_x;
    pos_y += velocity_y;
    if(pos_x % 16 == 0)tile_x = pos_x / 16;
    else tile_x = (pos_x + 8) / 16;
    if(pos_y % 16 == 0)tile_y = pos_y / 16;
    else tile_x = (pos_y + 8) / 16;
}

#ifndef CHARACTER_H
#define CHARACTER_H


class Character
{
    private:
        int pos_x, pos_y, tile_x, tile_y, dir;
        int velocity_x = 0, velocity_y = 0;
        bool dead;
    public:
        static const int UP = 0;
        static const int RIGHT = 1;
        static const int DOWN = 2;
        static const int LEFT = 3;
        static const int CHARACTER_WIDTH  = 30;
        static const int CHARACTER_HEIGHT = 30;

        Character(int tile_x, int tile_y, int velocity_x = 0, int velocity_y = 0);
        ~Character() {}

        void move();
        bool isDead() const;

        int getPosX() const;
        int getPosY() const;

        int getTileX() const;
        int getTileY() const;

};

#endif // CHARACTER_H

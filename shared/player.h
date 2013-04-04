#ifndef PLAYER_H
#define PLAYER_H

#include "entityliving.h"

class Player: public EntityLiving
{
    public:
        Player();
        void MoveUp(float);
        void MoveDown(float);
        void MoveLeft(float);
        void MoveRight(float);
    private:
        static const int defaultSpeed = 400;
        float speed;
};

#endif // PLAYER_H
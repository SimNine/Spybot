#ifndef AIMODERATE_H
#define AIMODERATE_H

#include "Standard.h"
#include "AICore.h"

class Game;

class AIModerate : public AICore
{
    public:
        AIModerate(Player* owner);
        virtual ~AIModerate();
    protected:
        bool actStep();
    private:
};

#endif // AIMODERATE_H

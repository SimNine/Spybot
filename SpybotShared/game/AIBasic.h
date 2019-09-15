#ifndef AIBASIC_H
#define AIBASIC_H

#include "Standard.h"
#include "AICore.h"

class Game;

class AIBasic : public AICore
{
    public:
        AIBasic(Player* owner);
        virtual ~AIBasic();
    protected:
        bool actStep() override;
    private:
};

#endif // AIBASIC_H

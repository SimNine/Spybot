#ifndef AIMODERATE_H
#define AIMODERATE_H

#include "AICore.h"

class AIModerate : public AICore
{
    public:
        AIModerate(Game*, int);
        virtual ~AIModerate();
    protected:
        bool actStep();
    private:
};

#endif // AIMODERATE_H

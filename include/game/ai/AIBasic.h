#ifndef AIBASIC_H
#define AIBASIC_H

#include "AICore.h"

class AIBasic : public AICore
{
    public:
        AIBasic(Game*, int);
        virtual ~AIBasic();
    protected:
        bool actStep() override;
    private:
};

#endif // AIBASIC_H

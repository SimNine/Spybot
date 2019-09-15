#ifndef PROGRAMMOVE_H
#define PROGRAMMOVE_H

#include "Standard.h"

class ProgramAction
{
    public:
        ProgramAction(MOVEPRESET);
        ProgramAction(ACTIONTYPE, std::string, std::string, int, int, int);
        virtual ~ProgramAction();
        ACTIONTYPE type;
        std::string name;
        std::string description;
        int range;
        int power;
        int requiredSize;
        bool hitsFriendlies, hitsEnemies;
    protected:
    private:
        void setMove(ACTIONTYPE, std::string, std::string, int, int, int);
};

#endif // PROGRAMMOVE_H

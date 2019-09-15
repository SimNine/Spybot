#ifndef PROGRAMMOVE_H
#define PROGRAMMOVE_H

#include <string>

#include "Enums.h"

class ProgramAction
{
    public:
        ProgramAction(MOVEPRESET);
        ProgramAction(MOVETYPE, std::string, std::string, int, int, int);
        virtual ~ProgramAction();
        MOVETYPE type;
        std::string name;
        std::string description;
        int range;
        int power;
        int requiredSize;
        bool hitsFriendlies, hitsEnemies;
    protected:
    private:
        void setMove(MOVETYPE, std::string, std::string, int, int, int);
};

#endif // PROGRAMMOVE_H

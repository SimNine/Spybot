#ifndef PROGRAMMOVE_H
#define PROGRAMMOVE_H

#include <string>

#include "Enums.h"

class ProgramMove
{
    public:
        ProgramMove(MOVE, std::string, std::string, int, int, int);
        virtual ~ProgramMove();
        MOVE type;
        std::string name;
        std::string description;
        int range;
        int power;
        int requiredSize;
        bool hitsFriendlies, hitsEnemies;
    protected:
    private:
};

#endif // PROGRAMMOVE_H

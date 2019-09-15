#include "Global.h"
#include "ProgramMove.h"

ProgramMove::ProgramMove(MOVE t, std::string name, std::string description, int range, int power, int reqSize)
{
    this->type = t;
    this->name = name;
    this->description = description;
    this->range = range;
    this->power = power;
    this->requiredSize = reqSize;
}

ProgramMove::~ProgramMove()
{
    if (debug)
    {
        printf("Move '%s' deleted\n", name.c_str());
    }
}

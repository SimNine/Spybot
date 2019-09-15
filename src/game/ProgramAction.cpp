#include "Global.h"
#include "ProgramAction.h"

ProgramAction::ProgramAction(MOVE t, std::string name, std::string description, int range, int power, int reqSize)
{
    this->type = t;
    this->name = name;
    this->description = description;
    this->range = range;
    this->power = power;
    this->requiredSize = reqSize;
}

ProgramAction::~ProgramAction()
{
    if (debug)
    {
        printf("Move '%s' deleted\n", name.c_str());
    }
}

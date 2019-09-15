#include "AIModerate.h"

AIModerate::AIModerate(Game* g, int i)
    : AICore(g, i)
{
    //ctor
}

AIModerate::~AIModerate()
{
    //dtor
}

bool AIModerate::actStep()
{
    // get the next usable program
    Program* myProg = getNextUsableProgram();
    if (myProg == NULL) return false;

    // select this program and compute its distances
    setSelectedProgram(myProg);

    // get the nearest enemy program to this program

}

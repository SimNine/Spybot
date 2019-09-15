#include "Program.h"
#include "Global.h"

#include <SDL.h>
#include <stdlib.h>

Program::Program(PROGRAM type, int team, int xHead, int yHead)
{
    this->xHead = xHead;
    this->yHead = yHead;
    this->team = team;
    this->type = type;
    icon = dataContainer->program_icons[type];
    color[0] = rand()%255;
    color[1] = rand()%255;
    color[2] = rand()%255;
    actionList = new LinkedList<ProgramAction*>();

    if (this->type == PROGRAM_CUSTOM)
    {
        return;
    }

    switch (this->type)
    {
    case PROGRAM_NONE:
    case PROGRAM_NUM_PROGTYPES:
        printf("ERROR: trying to instantiate an invalid program type");
        exit(1);
        break;
    case PROGRAM_CUSTOM:
        break;
    case PROGRAM_BALLISTA:
        name = "Ballista";
        description = "Extreme-range attack program";
        maxMoves = 1;
        maxHealth = 2;
        cost = 3000;
        addAction(new ProgramAction(MOVE_DAMAGE, "Fling", "Range:4 Damage:2", 4, 2, 0));
        break;
    case PROGRAM_BITMAN:
        name = "Bit-Man";
        description = "Makes sectors of the grid appear or disappear";
        maxMoves = 3;
        maxHealth = 3;
        cost = 250;
        addAction(new ProgramAction(MOVE_TILEPLACE, "One", "Repairs one grid square", 1, 0, 0));
        addAction(new ProgramAction(MOVE_TILEDELETE, "Zero", "Deletes one grid square", 1, 0, 0));
        break;
    case PROGRAM_BITMAN2:
        name = "Bit-Woman";
        description = "Makes distant sectors of the grid appear or disappear";
        maxMoves = 4;
        maxHealth = 2;
        cost = 1000;
        addAction(new ProgramAction(MOVE_TILEPLACE, "Constructor", "Repairs one grid square", 3, 0, 0));
        addAction(new ProgramAction(MOVE_TILEDELETE, "Deconstructor", "Deletes one grid square", 3, 0, 0));
        break;
    case PROGRAM_BLACKWIDOW:
        name = "Black Widow";
        description = "Speedier and creepier";
        maxMoves = 4;
        maxHealth = 3;
        cost = 2000;
        addAction(new ProgramAction(MOVE_DAMAGE, "Byte", "Range:1 Damage:2", 1, 2, 0));
        addAction(new ProgramAction(MOVE_SPEEDDOWN, "Paralyze", "Decreases move rate of target program by 3", 1, 3, 0));
        break;
    case PROGRAM_BOSS:
        name = "Boss";
        description = "Prepare to be owned";
        maxMoves = 6;
        maxHealth = 25;
        cost = 50000;
        //addMove(new ProgramMove(MOVE_DAMAGE, "Shutdown",));
        break;
    case PROGRAM_BUG:
        name = "Bug";
        description = "Fast, cheap, and out of control";
        maxMoves = 5;
        maxHealth = 1;
        cost = 750;
        // TODO "Glitch"
        break;
    case PROGRAM_BUG2:
        name = "MandelBug";
        description = "It's not a bug, it's a feature";
        maxMoves = 5;
        maxHealth = 1;
        cost = 3000;
        // TODO "Fractal Glitch"
        break;
    case PROGRAM_BUG3:
        name = "HeisenBug";
        description = "They can't kill what they can't catch";
        maxMoves = 5;
        maxHealth = 1;
        cost = 4000;
        // TODO "Quantum Glitch"
        break;
    case PROGRAM_CATAPULT:
        name = "Catapult";
        description = "Extreme-range mobile attacker";
        maxMoves = 2;
        maxHealth = 3;
        cost = 4000;
        // TODO "Fling"
        break;
    case PROGRAM_CLOG:
        name = "Clog.01";
        description = "Slows down hostile programs";
        maxMoves = 2;
        maxHealth = 4;
        cost = 1000;
        // TODO "Lag"
        break;
    case PROGRAM_CLOG2:
        name = "Clog.02";
        description = "Twice as effective as version .01";
        maxMoves = 2;
        maxHealth = 4;
        cost = 2000;
        // TODO "Chug"
        break;
    case PROGRAM_CLOG3:
        name = "Clog.03";
        description = "Brings hostile programs to a halt";
        maxMoves = 2;
        maxHealth = 4;
        cost = 3500;
        // TODO "Chug"
        // TODO "Hang"
        break;
    case PROGRAM_DATABOMB:
        name = "LogicBomb";
        description = "Self-destructing attack program";
        maxMoves = 3;
        maxHealth = 6;
        cost = 3500;
        // TODO "Selfdestruct"
        break;
    case PROGRAM_DATADOCTOR:
        name = "Data Doctor";
        description = "Helps your programs grow";
        maxMoves = 4;
        maxHealth = 5;
        cost = 500;
        // TODO "Grow"
        break;
    case PROGRAM_DATADOCTOR2:
        name = "Data Doctor Pro";
        description = "Twice the expansion power of Data Doctor";
        maxMoves = 5;
        maxHealth = 8;
        cost = 1500;
        // TODO "Megagrow"
        // TODO "Surgery"
        break;
    case PROGRAM_DOG:
        name = "Guard Pup";
        description = "A speedy little corporate cur";
        maxMoves = 3;
        maxHealth = 2;
        cost = 300;
        // TODO "Byte"
        break;
    case PROGRAM_DOG2:
        name = "Guard Dog";
        description = "Who let the dogs out?";
        maxMoves = 3;
        maxHealth = 3;
        cost = 300;
        // TODO "Byte"
        break;
    case PROGRAM_DOG3:
        name = "Attack Dog";
        description = "Ravenous and bloodthirsty corporate canine";
        maxMoves = 4;
        maxHealth = 7;
        cost = 300;
        // TODO "Megabyte"
        break;
    case PROGRAM_FIDDLE:
        name = "Fiddle";
        description = "Twiddle and tweak the power of your programs";
        maxMoves = 3;
        maxHealth = 3;
        cost = 2400;
        // TODO "Tweak"
        // TODO "Twiddle"
        break;
    case PROGRAM_FIREWALL:
        name = "Fire Wall";
        description = "Keeps unwanted programs out of corporate sectors";
        maxMoves = 2;
        maxHealth = 20;
        cost = 300;
        // TODO "Burn"
        break;
    case PROGRAM_GOLEM:
        name = "Golem.mud";
        description = "Slow and steady attack program";
        maxMoves = 1;
        maxHealth = 5;
        cost = 1200;
        // TODO "Thump"
        break;
    case PROGRAM_GOLEM2:
        name = "Golem.clay";
        description = "Clay is stronger than mud";
        maxMoves = 2;
        maxHealth = 6;
        cost = 3000;
        // TODO "Bash"
        break;
    case PROGRAM_GOLEM3:
        name = "Golem.stone";
        description = "Nothing can stand in its way";
        maxMoves = 3;
        maxHealth = 7;
        cost = 5000;
        // TODO "Crash"
        break;
    case PROGRAM_HACK:
        name = "Hack";
        description = "Basic attack program";
        maxMoves = 2;
        maxHealth = 4;
        cost = 500;
        // TODO "Slice"
        break;
    case PROGRAM_HACK2:
        name = "Hack 2.0";
        description = "Improved Hack: larger size and better attacks";
        maxMoves = 3;
        maxHealth = 4;
        cost = 1500;
        // TODO "Slice"
        // TODO "Dice"
        break;
    case PROGRAM_HACK3:
        name = "Hack 3.0";
        description = "The top of the Hack series";
        maxMoves = 4;
        maxHealth = 4;
        cost = 3500;
        // TODO "Slice"
        // TODO "Mutilate"
        break;
    case PROGRAM_KAMIKAZEE:
        name = "BuzzBomb";
        description = "Fast and annoying";
        maxMoves = 8;
        maxHealth = 2;
        cost = 3500;
        // TODO "Sting"
        // TODO "Kamikazee"
        break;
    case PROGRAM_MEDIC:
        name = "Medic";
        description = "Grows your programs from a distance";
        maxMoves = 3;
        maxHealth = 3;
        cost = 1000;
        // TODO "Hypo"
        break;
    case PROGRAM_MEMHOG:
        name = "Memory Hog";
        description = "Massive memory-filling bloatware";
        maxMoves = 5;
        maxHealth = 30;
        cost = 300;
        break;
    case PROGRAM_MOBILETOWER:
        name = "Tower";
        description = "Immobile long-range program";
        maxMoves = 0;
        maxHealth = 1;
        cost = 1000;
        // TODO "Launch";
        break;
    case PROGRAM_SATELLITE:
        name = "Satellite";
        description = "Short-range hard-hitting program";
        maxMoves = 1;
        maxHealth = 1;
        cost = 3500;
        // TODO "Scramble";
        break;
    case PROGRAM_SATELLITE2:
        name = "Laser Satellite";
        description = "Long-range hard-hitting program";
        maxMoves = 2;
        maxHealth = 1;
        cost = 5000;
        // TODO "Megascramble"
        break;
    case PROGRAM_SEEKER:
        name = "Seeker";
        description = "Solid distance attack program";
        maxMoves = 3;
        maxHealth = 3;
        cost = 1000;
        // TODO "Peek"
        break;
    case PROGRAM_SEEKER2:
        name = "Seeker 2.0";
        description = "Seeker 2.0";
        maxMoves = 3;
        maxHealth = 4;
        cost = 2500;
        // TODO "Poke"
        break;
    case PROGRAM_SEEKER3:
        name = "Seeker 3.0";
        description = "Seeker with extra deletion power";
        maxMoves = 4;
        maxHealth = 5;
        cost = 4500;
        // TODO "Poke"
        // TODO "Seek and Destroy"
        break;
    case PROGRAM_SLINGSHOT:
        name = "Slingshot";
        description = "Basic ranged attack program";
        maxMoves = 2;
        maxHealth = 2;
        cost = 750;
        // TODO "Stone"
        break;
    case PROGRAM_SONAR:
        name = "Sensor";
        description = "Immobile program eradicator";
        maxMoves = 0;
        maxHealth = 1;
        cost = 1750;
        // TODO "Blip"
        break;
    case PROGRAM_SONAR2:
        name = "Radar";
        description = "Long-range program eradicator";
        maxMoves = 0;
        maxHealth = 1;
        cost = 1750;
        // TODO "Ping"
        break;
    case PROGRAM_SONAR3:
        name = "Radar";
        description = "Deadly program eradicator";
        maxMoves = 0;
        maxHealth = 1;
        cost = 1750;
        // TODO "Pong"
        break;
    case PROGRAM_SPECS:
        name = "Guru";
        description = "Multipurpose software for the l33tist of the l33t";
        maxMoves = 2;
        maxHealth = 3;
        cost = 4500;
        // TODO "Fire"
        // TODO "Ice"
        break;
    case PROGRAM_SUMO:
        name = "Sumo";
        description = "A massive and slow-moving powerhouse";
        maxMoves = 2;
        maxHealth = 12;
        cost = 4500;
        // TODO "Dataslam"
        break;
    case PROGRAM_TARANTULA:
        name = "Tarantula";
        description = "Fast, with a venomous bite";
        maxMoves = 5;
        maxHealth = 3;
        cost = 3500;
        // TODO "Megabyte"
        // TODO "Paralyze"
        break;
    case PROGRAM_TOWER:
        name = "Tower";
        description = "Immobile long-range program";
        maxMoves = 0;
        maxHealth = 1;
        cost = 1000;
        // TODO "Launch"
        break;
    case PROGRAM_TURBO:
        name = "Turbo";
        description = "Speeds up your programs";
        maxMoves = 3;
        maxHealth = 3;
        cost = 1000;
        // TODO "Boost"
        break;
    case PROGRAM_TURBO2:
        name = "Turbo Deluxe";
        description = "Slow and steady is for losers";
        maxMoves = 4;
        maxHealth = 4;
        cost = 1750;
        // TODO "Megaboost"
        break;
    case PROGRAM_TURBO3:
        name = "Turbo DLC";
        description = "Gotta go fast";
        maxMoves = 5;
        maxHealth = 5;
        cost = 3000;
        // TODO "Gigaboost"
        break;
    case PROGRAM_WALKER:
        name = "Sentinel";
        description = "Corporate data defender";
        maxMoves = 1;
        maxHealth = 3;
        cost = 300;
        // TODO "Cut"
        break;
    case PROGRAM_WALKER2:
        name = "Sentinel 2.0";
        description = "Improved corporate data defender";
        maxMoves = 2;
        maxHealth = 4;
        cost = 300;
        // TODO "Cut"
        break;
    case PROGRAM_WALKER3:
        name = "Sentinel 3.0";
        description = "Top of the line in corporate data defense";
        maxMoves = 2;
        maxHealth = 4;
        cost = 300;
        // TODO "Taser"
        break;
    case PROGRAM_WARDEN:
        name = "Warden";
        description = "Slow and steady corporate attack program";
        maxMoves = 1;
        maxHealth = 5;
        cost = 2500;
        // TODO "Thump"
        break;
    case PROGRAM_WARDEN2:
        name = "Warden+";
        description = "Get out of its way";
        maxMoves = 2;
        maxHealth = 6;
        cost = 3500;
        // TODO "Bash"
        break;
    case PROGRAM_WARDEN3:
        name = "Warden++";
        description = "The last word in corporate security";
        maxMoves = 3;
        maxHealth = 7;
        cost = 5000;
        // TODO "Crash"
        break;
    case PROGRAM_WATCHMAN:
        name = "Watchman";
        description = "Basic ranged attack program";
        maxMoves = 1;
        maxHealth = 2;
        cost = 300;
        // TODO "Phaser"
        break;
    case PROGRAM_WATCHMAN2:
        name = "Watchman X";
        description = "Improved version of Watchman";
        maxMoves = 1;
        maxHealth = 4;
        cost = 300;
        // TODO "Phaser"
        break;
    case PROGRAM_WATCHMAN3:
        name = "Watchman SP";
        description = "QUI CUSTODIET IPSOS CUSTODES?";
        maxMoves = 1;
        maxHealth = 4;
        cost = 300;
        // TODO "Photon"
        break;
    case PROGRAM_WIZARD:
        name = "Wizard";
        description = "Pay no attention to the man behind the curtain";
        maxMoves = 3;
        maxHealth = 4;
        cost = 300;
        // TODO "Scorch"
        // TODO "Stretch"
        break;
    case PROGRAM_WOLFSPIDER:
        name = "Wolf Spider";
        description = "Speedy and creepy little program";
        maxMoves = 3;
        maxHealth = 3;
        cost = 750;
        // TODO "Byte"
        break;
    }

    moves = maxMoves;

}

Program::~Program()
{
    while (actionList->getLength() > 0)
    {
        ProgramAction* m = actionList->poll();
        delete m;
    }
    delete actionList;

    if (debug)
    {
        printf("Program '%s' deleted\n", name.c_str());
    }
}

SDL_Texture* Program::getIcon()
{
    return icon;
}

int Program::getCoreX()
{
    return xHead;
}

int Program::getCoreY()
{
    return yHead;
}

int Program::getColor(int n)
{
    if (n < 0 || n > 2)
    {
        return 0;
    }

    return color[n];
}

void Program::setColor(int r, int g, int b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
}

int Program::getTeam()
{
    return team;
}

int Program::getHealth()
{
    return health;
}

int Program::getMaxHealth()
{
    return maxHealth;
}

PROGRAM Program::getType()
{
    return type;
}

int Program::getMoves()
{
    return moves;
}

int Program::getMaxMoves()
{
    return maxMoves;
}

void Program::setCoreX(int i)
{
    xHead = i;
}

void Program::setCoreY(int i)
{
    yHead = i;
}

void Program::setType(PROGRAM i)
{
    type = i;
}

void Program::setHealth(int i)
{
    health = i;
}

void Program::setMaxHealth(int i)
{
    maxHealth = i;
}

void Program::setMoves(int i)
{
    moves = i;
}

void Program::setMaxMoves(int i)
{
    maxMoves = i;
}

void Program::setTeam(int i)
{
    team = i;
}

std::string Program::getName()
{
    return name;
}

void Program::setName(std::string n)
{
    name = n;
}

void Program::addAction(ProgramAction* m)
{
    if (actionList == NULL)
    {
        actionList = new LinkedList<ProgramAction*>();
    }

    actionList->addLast(m);
}

void Program::endTurn()
{
    moves = maxMoves;
}

LinkedList<ProgramAction*>* Program::getActions()
{
    return actionList;
}

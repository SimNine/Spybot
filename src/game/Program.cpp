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
    moveList = NULL;

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
        speed = 1;
        maxHealth = 2;
        cost = 3000;
        addMove(new ProgramMove(MOVE_DAMAGE, "Fling", "Range:4 Damage:2", 4, 2, 0));
        break;
    case PROGRAM_BITMAN:
        name = "Bit-Man";
        description = "Makes sectors of the grid appear or disappear";
        speed = 3;
        maxHealth = 3;
        cost = 250;
        addMove(new ProgramMove(MOVE_TILEPLACE, "One", "Repairs one grid square", 1, 0, 0));
        addMove(new ProgramMove(MOVE_TILEDELETE, "Zero", "Deletes one grid square", 1, 0, 0));
        break;
    case PROGRAM_BITMAN2:
        name = "Bit-Woman";
        description = "Makes distant sectors of the grid appear or disappear";
        speed = 4;
        maxHealth = 2;
        cost = 1000;
        addMove(new ProgramMove(MOVE_TILEPLACE, "Constructor", "Repairs one grid square", 3, 0, 0));
        addMove(new ProgramMove(MOVE_TILEPLACE, "Deconstructor", "Deletes one grid square", 3, 0, 0));
        break;
    case PROGRAM_BLACKWIDOW:
        name = "Black Widow";
        description = "Speedier and creepier";
        speed = 4;
        maxHealth = 3;
        cost = 2000;
        addMove(new ProgramMove(MOVE_DAMAGE, "Byte", "Range:1 Damage:2", 1, 2, 0));
        addMove(new ProgramMove(MOVE_SPEEDDOWN, "Paralyze", "Decreases move rate of target program by 3", 1, 3, 0));
        break;
    case PROGRAM_BOSS:
        name = "Boss";
        description = "Prepare to be owned";
        speed = 6;
        maxHealth = 25;
        cost = 50000;
        //addMove(new ProgramMove(MOVE_DAMAGE, "Shutdown",));
        break;
    case PROGRAM_BUG:
        name = "Bug";
        description = "Fast, cheap, and out of control";
        speed = 5;
        maxHealth = 1;
        cost = 750;
        // TODO "Glitch"
        break;
    case PROGRAM_BUG2:
        name = "MandelBug";
        description = "It's not a bug, it's a feature";
        speed = 5;
        maxHealth = 1;
        cost = 3000;
        // TODO "Fractal Glitch"
        break;
    case PROGRAM_BUG3:
        name = "HeisenBug";
        description = "They can't kill what they can't catch";
        speed = 5;
        maxHealth = 1;
        cost = 4000;
        // TODO "Quantum Glitch"
        break;
    case PROGRAM_CATAPULT:
        name = "Catapult";
        description = "Extreme-range mobile attacker";
        speed = 2;
        maxHealth = 3;
        cost = 4000;
        // TODO "Fling"
        break;
    case PROGRAM_CLOG:
        name = "Clog.01";
        description = "Slows down hostile programs";
        speed = 2;
        maxHealth = 4;
        cost = 1000;
        // TODO "Lag"
        break;
    case PROGRAM_CLOG2:
        name = "Clog.02";
        description = "Twice as effective as version .01";
        speed = 2;
        maxHealth = 4;
        cost = 2000;
        // TODO "Chug"
        break;
    case PROGRAM_CLOG3:
        name = "Clog.03";
        description = "Brings hostile programs to a halt";
        speed = 2;
        maxHealth = 4;
        cost = 3500;
        // TODO "Chug"
        // TODO "Hang"
        break;
    case PROGRAM_DATABOMB:
        name = "LogicBomb";
        description = "Self-destructing attack program";
        speed = 3;
        maxHealth = 6;
        cost = 3500;
        // TODO "Selfdestruct"
        break;
    case PROGRAM_DATADOCTOR:
        name = "Data Doctor";
        description = "Helps your programs grow";
        speed = 4;
        maxHealth = 5;
        cost = 500;
        // TODO "Grow"
        break;
    case PROGRAM_DATADOCTOR2:
        name = "Data Doctor Pro";
        description = "Twice the expansion power of Data Doctor";
        speed = 5;
        maxHealth = 8;
        cost = 1500;
        // TODO "Megagrow"
        // TODO "Surgery"
        break;
    case PROGRAM_DOG:
        name = "Guard Pup";
        description = "A speedy little corporate cur";
        speed = 3;
        maxHealth = 2;
        cost = 300;
        // TODO "Byte"
        break;
    case PROGRAM_DOG2:
        name = "Guard Dog";
        description = "Who let the dogs out?";
        speed = 3;
        maxHealth = 3;
        cost = 300;
        // TODO "Byte"
        break;
    case PROGRAM_DOG3:
        name = "Attack Dog";
        description = "Ravenous and bloodthirsty corporate canine";
        speed = 4;
        maxHealth = 7;
        cost = 300;
        // TODO "Megabyte"
        break;
    case PROGRAM_FIDDLE:
        name = "Fiddle";
        description = "Twiddle and tweak the power of your programs";
        speed = 3;
        maxHealth = 3;
        cost = 2400;
        // TODO "Tweak"
        // TODO "Twiddle"
        break;
    case PROGRAM_FIREWALL:
        name = "Fire Wall";
        description = "Keeps unwanted programs out of corporate sectors";
        speed = 2;
        maxHealth = 20;
        cost = 300;
        // TODO "Burn"
        break;
    case PROGRAM_GOLEM:
        name = "Golem.mud";
        description = "Slow and steady attack program";
        speed = 1;
        maxHealth = 5;
        cost = 1200;
        // TODO "Thump"
        break;
    case PROGRAM_GOLEM2:
        name = "Golem.clay";
        description = "Clay is stronger than mud";
        speed = 2;
        maxHealth = 6;
        cost = 3000;
        // TODO "Bash"
        break;
    case PROGRAM_GOLEM3:
        name = "Golem.stone";
        description = "Nothing can stand in its way";
        speed = 3;
        maxHealth = 7;
        cost = 5000;
        // TODO "Crash"
        break;
    case PROGRAM_HACK:
        name = "Hack";
        description = "Basic attack program";
        speed = 2;
        maxHealth = 4;
        cost = 500;
        // TODO "Slice"
        break;
    case PROGRAM_HACK2:
        name = "Hack 2.0";
        description = "Improved Hack: larger size and better attacks";
        speed = 3;
        maxHealth = 4;
        cost = 1500;
        // TODO "Slice"
        // TODO "Dice"
        break;
    case PROGRAM_HACK3:
        name = "Hack 3.0";
        description = "The top of the Hack series";
        speed = 4;
        maxHealth = 4;
        cost = 3500;
        // TODO "Slice"
        // TODO "Mutilate"
        break;
    case PROGRAM_KAMIKAZEE:
        name = "BuzzBomb";
        description = "Fast and annoying";
        speed = 8;
        maxHealth = 2;
        cost = 3500;
        // TODO "Sting"
        // TODO "Kamikazee"
        break;
    case PROGRAM_MEDIC:
        name = "Medic";
        description = "Grows your programs from a distance";
        speed = 3;
        maxHealth = 3;
        cost = 1000;
        // TODO "Hypo"
        break;
    case PROGRAM_MEMHOG:
        name = "Memory Hog";
        description = "Massive memory-filling bloatware";
        speed = 5;
        maxHealth = 30;
        cost = 300;
        break;
    case PROGRAM_MOBILETOWER:
        name = "Tower";
        description = "Immobile long-range program";
        speed = 0;
        maxHealth = 1;
        cost = 1000;
        // TODO "Launch";
        break;
    case PROGRAM_SATELLITE:
        name = "Satellite";
        description = "Short-range hard-hitting program";
        speed = 1;
        maxHealth = 1;
        cost = 3500;
        // TODO "Scramble";
        break;
    case PROGRAM_SATELLITE2:
        name = "Laser Satellite";
        description = "Long-range hard-hitting program";
        speed = 2;
        maxHealth = 1;
        cost = 5000;
        // TODO "Megascramble"
        break;
    case PROGRAM_SEEKER:
        name = "Seeker";
        description = "Solid distance attack program";
        speed = 3;
        maxHealth = 3;
        cost = 1000;
        // TODO "Peek"
        break;
    case PROGRAM_SEEKER2:
        name = "Seeker 2.0";
        description = "Seeker 2.0";
        speed = 3;
        maxHealth = 4;
        cost = 2500;
        // TODO "Poke"
        break;
    case PROGRAM_SEEKER3:
        name = "Seeker 3.0";
        description = "Seeker with extra deletion power";
        speed = 4;
        maxHealth = 5;
        cost = 4500;
        // TODO "Poke"
        // TODO "Seek and Destroy"
        break;
    case PROGRAM_SLINGSHOT:
        name = "Slingshot";
        description = "Basic ranged attack program";
        speed = 2;
        maxHealth = 2;
        cost = 750;
        // TODO "Stone"
        break;
    case PROGRAM_SONAR:
        name = "Sensor";
        description = "Immobile program eradicator";
        speed = 0;
        maxHealth = 1;
        cost = 1750;
        // TODO "Blip"
        break;
    case PROGRAM_SONAR2:
        name = "Radar";
        description = "Long-range program eradicator";
        speed = 0;
        maxHealth = 1;
        cost = 1750;
        // TODO "Ping"
        break;
    case PROGRAM_SONAR3:
        name = "Radar";
        description = "Deadly program eradicator";
        speed = 0;
        maxHealth = 1;
        cost = 1750;
        // TODO "Pong"
        break;
    case PROGRAM_SPECS:
        name = "Guru";
        description = "Multipurpose software for the l33tist of the l33t";
        speed = 2;
        maxHealth = 3;
        cost = 4500;
        // TODO "Fire"
        // TODO "Ice"
        break;
    case PROGRAM_SUMO:
        name = "Sumo";
        description = "A massive and slow-moving powerhouse";
        speed = 2;
        maxHealth = 12;
        cost = 4500;
        // TODO "Dataslam"
        break;
    case PROGRAM_TARANTULA:
        name = "Tarantula";
        description = "Fast, with a venomous bite";
        speed = 5;
        maxHealth = 3;
        cost = 3500;
        // TODO "Megabyte"
        // TODO "Paralyze"
        break;
    case PROGRAM_TOWER:
        name = "Tower";
        description = "Immobile long-range program";
        speed = 0;
        maxHealth = 1;
        cost = 1000;
        // TODO "Launch"
        break;
    case PROGRAM_TURBO:
        name = "Turbo";
        description = "Speeds up your programs";
        speed = 3;
        maxHealth = 3;
        cost = 1000;
        // TODO "Boost"
        break;
    case PROGRAM_TURBO2:
        name = "Turbo Deluxe";
        description = "Slow and steady is for losers";
        speed = 4;
        maxHealth = 4;
        cost = 1750;
        // TODO "Megaboost"
        break;
    case PROGRAM_TURBO3:
        name = "Turbo DLC";
        description = "Gotta go fast";
        speed = 5;
        maxHealth = 5;
        cost = 3000;
        // TODO "Gigaboost"
        break;
    case PROGRAM_WALKER:
        name = "Sentinel";
        description = "Corporate data defender";
        speed = 1;
        maxHealth = 3;
        cost = 300;
        // TODO "Cut"
        break;
    case PROGRAM_WALKER2:
        name = "Sentinel 2.0";
        description = "Improved corporate data defender";
        speed = 2;
        maxHealth = 4;
        cost = 300;
        // TODO "Cut"
        break;
    case PROGRAM_WALKER3:
        name = "Sentinel 3.0";
        description = "Top of the line in corporate data defense";
        speed = 2;
        maxHealth = 4;
        cost = 300;
        // TODO "Taser"
        break;
    case PROGRAM_WARDEN:
        name = "Warden";
        description = "Slow and steady corporate attack program";
        speed = 1;
        maxHealth = 5;
        cost = 2500;
        // TODO "Thump"
        break;
    case PROGRAM_WARDEN2:
        name = "Warden+";
        description = "Get out of its way";
        speed = 2;
        maxHealth = 6;
        cost = 3500;
        // TODO "Bash"
        break;
    case PROGRAM_WARDEN3:
        name = "Warden++";
        description = "The last word in corporate security";
        speed = 3;
        maxHealth = 7;
        cost = 5000;
        // TODO "Crash"
        break;
    case PROGRAM_WATCHMAN:
        name = "Watchman";
        description = "Basic ranged attack program";
        speed = 1;
        maxHealth = 2;
        cost = 300;
        // TODO "Phaser"
        break;
    case PROGRAM_WATCHMAN2:
        name = "Watchman X";
        description = "Improved version of Watchman";
        speed = 1;
        maxHealth = 4;
        cost = 300;
        // TODO "Phaser"
        break;
    case PROGRAM_WATCHMAN3:
        name = "Watchman SP";
        description = "QUI CUSTODIET IPSOS CUSTODES?";
        speed = 1;
        maxHealth = 4;
        cost = 300;
        // TODO "Photon"
        break;
    case PROGRAM_WIZARD:
        name = "Wizard";
        description = "Pay no attention to the man behind the curtain";
        speed = 3;
        maxHealth = 4;
        cost = 300;
        // TODO "Scorch"
        // TODO "Stretch"
        break;
    case PROGRAM_WOLFSPIDER:
        name = "Wolf Spider";
        description = "Speedy and creepy little program";
        speed = 3;
        maxHealth = 3;
        cost = 750;
        // TODO "Byte"
        break;
    }
}

Program::~Program()
{
    while (moveList != NULL)
    {
        ProgramMove* m = moveList->getContents();
        delete m;
        LinkedList<ProgramMove*>* tempNode = moveList;
        moveList = moveList->getNext();
        delete tempNode;
    }

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

int Program::getSpeed()
{
    return speed;
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

void Program::setSpeed(int i)
{
    speed = i;
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

void Program::addMove(ProgramMove* m)
{
    if (moveList == NULL)
    {
        moveList = new LinkedList<ProgramMove*>(m);
    }
    else
    {
        LL_addObject(moveList, m);
    }
}

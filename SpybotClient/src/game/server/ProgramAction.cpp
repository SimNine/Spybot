#include "Standard.h"
#include "ProgramAction.h"

#include "Global.h"

ProgramAction::ProgramAction(ACTIONTYPE t, std::string name, std::string description, int range, int power, int reqSize)
{
	this->type = t;
	this->name = name;
	this->description = description;
	this->range = range;
	this->power = power;
	this->requiredSize = reqSize;
	this->actionID = -1;
}

ProgramAction::ProgramAction(MOVEPRESET p)
{
	actionID = -1;

    switch (p)
    {
    case MOVEPRESET_BASH:
        setMove(ACTIONTYPE_DAMAGE, "Bash", "Range:1 Damage:5", 1, 5, 0);
        break;
    case MOVEPRESET_BLIP:
        setMove(ACTIONTYPE_DAMAGE, "Blip", "Range:5 Damage:1", 5, 1, 0);
        break;
    case MOVEPRESET_BOOST:
        setMove(ACTIONTYPE_SPEEDUP, "Boost", "Range:1 Increases move rate of target by 1 & deletes 1 sector from Turbo", 1, 1, 0);
        // TODO: add cost (of 1) as parameter
        break;
    case MOVEPRESET_BURN:
        setMove(ACTIONTYPE_DAMAGE, "Burn", "Range:1 Damage:1", 1, 1, 0);
        break;
    case MOVEPRESET_BYTE:
        setMove(ACTIONTYPE_DAMAGE, "Byte", "Range:1 Damage:2", 1, 2, 0);
        break;
    case MOVEPRESET_CHUG:
        setMove(ACTIONTYPE_SPEEDDOWN, "Chug", "Range:3 Decreases move rate of target by 2", 3, 2, 0);
        break;
    case MOVEPRESET_CONSTRUCTOR:
        setMove(ACTIONTYPE_TILEPLACE, "Constructor", "Repairs one grid square", 3, 0, 0);
        break;
    case MOVEPRESET_CRASH:
        setMove(ACTIONTYPE_DAMAGE, "Crash", "Range:1 Damage:7", 1, 7, 0);
        break;
    case MOVEPRESET_CRUNCHTIME:
        // TODO: special effect - DELETES 1 SECTOR FROM ALL ENEMY PROGRAMS THAT ARE CURRENTLY SIZE 3 OR BIGGER
        break;
    case MOVEPRESET_CUT:
        setMove(ACTIONTYPE_DAMAGE, "Cut", "Range:1 Damage:2", 1, 2, 0);
        break;
    case MOVEPRESET_DARKSTORM:
        // TODO: special effect - DELETES 1 SECTOR FROM ALL PROGRAMS INCLUDING YOURS
        break;
    case MOVEPRESET_DATASLAM:
        setMove(ACTIONTYPE_DAMAGE, "Dataslam", "Sizereq:6 Deletes 8 sectors from target", 1, 8, 6);
        break;
    case MOVEPRESET_DECONSTRUCTOR:
        setMove(ACTIONTYPE_TILEDELETE, "Deconstructor", "Deletes one grid square", 3, 0, 0);
        break;
    case MOVEPRESET_DICE:
        setMove(ACTIONTYPE_DAMAGE, "Dice", "Sizereq:3 Deletes 3 sectors from target", 1, 3, 3);
        break;
    case MOVEPRESET_FIRE:
        setMove(ACTIONTYPE_DAMAGE, "Fire", "Range:2 Damage:4", 2, 4, 0);
        break;
    case MOVEPRESET_FLASHGROW:
        // TODO: special effect - ADDS 1 SECTOR TO ALL OF YOUR PROGRAMS
        break;
    case MOVEPRESET_FLING:
        setMove(ACTIONTYPE_DAMAGE, "Fling", "Range:4 Damage:2", 4, 2, 0);
        break;
    case MOVEPRESET_FRACTAL_GLITCH:
        setMove(ACTIONTYPE_DAMAGE, "Fractal Glitch", "Range:1 Damage:4", 1, 4, 0);
        break;
    case MOVEPRESET_FRY:
        // TODO: special effect - DELETES 4 SECTORS FROM ALL TOUCHING PROGRAMS & DELETES 3 SECTORS FROM EEL
        break;
    case MOVEPRESET_GIGABOOST:
        setMove(ACTIONTYPE_SPEEDUP, "Gigaboost", "Sizereq:4 Range:2 Adds 4 to move rate & deletes 3 sectors from Turbo Deluxe", 2, 4, 4);
        // TODO: add cost (of 3) as parameter
        break;
    case MOVEPRESET_GLITCH:
        setMove(ACTIONTYPE_DAMAGE, "Glitch", "Range:1 Damage:2", 1, 2, 0);
        break;
    case MOVEPRESET_GROW:
        setMove(ACTIONTYPE_HEAL, "Grow", "Range:1 Adds 2 sectors to target", 1, 2, 0);
        break;
    case MOVEPRESET_HANG:
        setMove(ACTIONTYPE_SPEEDDOWN, "Hang", "Sizereq:4 Range:3 Decreases move speed of target to 0", 3, 3, 4);
        break;
    case MOVEPRESET_HYPO:
        setMove(ACTIONTYPE_HEAL, "Hypo", "Range:3 Damage:2", 3, 2, 0);
        break;
    case MOVEPRESET_ICE:
        setMove(ACTIONTYPE_SPEEDDOWN, "Ice", "Range:2 Decreases movement of target by 3", 2, 3, 0);
        break;
    case MOVEPRESET_KAMIKAZEE:
        setMove(ACTIONTYPE_DAMAGE, "Kamikazee", "Range:1 Deletes 5 sectors from target and erases BuzzBomb", 1, 5, 0);
        // TODO: add cost (of all) as parameter
        break;
    case MOVEPRESET_LAG:
        setMove(ACTIONTYPE_SPEEDDOWN, "Lag", "Range:3 Decreases move rate of target by 1", 3, 1, 0);
        break;
    case MOVEPRESET_LAUNCH:
        setMove(ACTIONTYPE_DAMAGE, "Launch", "Range:3 Damage:3", 3, 3, 0);
        break;
    case MOVEPRESET_MEGABOOST:
        setMove(ACTIONTYPE_SPEEDUP, "Megaboost", "Sizereq:3 Range:2 Adds 2 to move rate & deletes 2 sectors from Turbo Deluxe", 2, 2, 3);
        // TODO: add cost (of 2) as parameter
        break;
    case MOVEPRESET_MEGABYTE:
        setMove(ACTIONTYPE_DAMAGE, "Megabyte", "Range:1 Damage:3", 1, 3, 0);
        break;
    case MOVEPRESET_MEGAGROW:
        setMove(ACTIONTYPE_HEAL, "Megagrow", "Range:1 Adds 4 sectors to target", 1, 4, 0);
        break;
    case MOVEPRESET_MEGASCRAMBLE:
        setMove(ACTIONTYPE_DAMAGE, "Megascramble", "Range:3 Damage:4", 3, 4, 0);
        break;
    case MOVEPRESET_MUTILATE:
        setMove(ACTIONTYPE_DAMAGE, "Mutilate", "Sizereq:4 Range:1 Damage:4", 1, 4, 4);
        break;
    case MOVEPRESET_ONE:
        setMove(ACTIONTYPE_TILEPLACE, "One", "Repairs one grid square", 1, 0, 0);
        break;
    case MOVEPRESET_OVERCLOCK:
        // TODO: special effect - adds 1 to the movement rate of all programs, including enemies
        break;
    case MOVEPRESET_PARALYZE:
        setMove(ACTIONTYPE_SPEEDDOWN, "Paralyze", "Decreases move rate of target program by 3", 1, 3, 0);
        break;
    case MOVEPRESET_PEEK:
        setMove(ACTIONTYPE_DAMAGE, "Peek", "Range:2 Damage:2", 2, 2, 0);
        break;
    case MOVEPRESET_PHASER:
        setMove(ACTIONTYPE_DAMAGE, "Phaser", "Range:2 Damage:2", 2, 2, 0);
        break;
    case MOVEPRESET_PHOTON:
        setMove(ACTIONTYPE_DAMAGE, "Photon", "Range:3 Damage:2", 3, 2, 0);
        break;
    case MOVEPRESET_PING:
        setMove(ACTIONTYPE_DAMAGE, "Ping", "Range:8 Damage:1", 8, 1, 0);
        break;
    case MOVEPRESET_POKE:
        setMove(ACTIONTYPE_DAMAGE, "Poke", "Range:3 Damage:2", 3, 2, 0);
        break;
    case MOVEPRESET_PONG:
        setMove(ACTIONTYPE_DAMAGE, "Pong", "Range:5 Damage:2", 5, 2, 0);
        break;
    case MOVEPRESET_QUANTUM_GLITCH:
        setMove(ACTIONTYPE_DAMAGE, "Quantum Glitch", "Range:1 Damage:6", 1, 6, 0);
        break;
    case MOVEPRESET_SCORCH:
        setMove(ACTIONTYPE_DAMAGE, "Scorch", "Range:3 Damage:2", 3, 2, 0);
        break;
    case MOVEPRESET_SCRAMBLE:
        setMove(ACTIONTYPE_DAMAGE, "Scramble", "Range:2 Damage:4", 2, 4, 0);
        break;
    case MOVEPRESET_SEEK_AND_DESTROY:
        setMove(ACTIONTYPE_DAMAGE, "Seek and Destroy", "Sizereq:5 Range:2 Deletes 5 sectors from target & 2 from Seeker", 2, 5, 5);
        // TODO: add cost (of 2) as parameter
        break;
    case MOVEPRESET_SELFDESTRUCT:
        setMove(ACTIONTYPE_DAMAGE, "Selfdestruct", "Sizereq:6 Deletes 10 sectors from target and erases LogicBomb", 1, 10, 6);
        // TODO: add cost (of all) as parameter
        break;
    case MOVEPRESET_SHUTDOWN:
        setMove(ACTIONTYPE_DAMAGE, "Shutdown", "Range:5 Damage:5", 5, 5, 0);
        break;
    case MOVEPRESET_SLICE:
        setMove(ACTIONTYPE_DAMAGE, "Slice", "Range:1 Damage:2", 1, 2, 0);
        break;
    case MOVEPRESET_STING:
        setMove(ACTIONTYPE_DAMAGE, "Sting", "Range:1 Damage:1", 1, 1, 0);
        break;
    case MOVEPRESET_STONE:
        setMove(ACTIONTYPE_DAMAGE, "Stone", "Range:3 Damage:1", 3, 1, 0);
        break;
    case MOVEPRESET_STRETCH:
        setMove(ACTIONTYPE_MAXHEALTHUP, "Stretch", "Range:2 Increases max size of target by 1", 2, 1, 0);
        break;
    case MOVEPRESET_SURGERY:
        setMove(ACTIONTYPE_MAXHEALTHUP, "Surgery", "Range:1 Increases max size of target by 1", 1, 1, 0);
        break;
    case MOVEPRESET_TASER:
        setMove(ACTIONTYPE_DAMAGE, "Taser", "Range:1 Damage:4", 1, 4, 0);
        break;
    case MOVEPRESET_THUMP:
        setMove(ACTIONTYPE_DAMAGE, "Thump", "Range:1 Damage:3", 1, 3, 0);
        break;
    case MOVEPRESET_TWEAK:
        setMove(ACTIONTYPE_SPEEDUP, "Tweak", "Range:1 Increases move rate of target by 1 & deletes 1 sector from Fiddle", 1, 1, 0);
        // TODO: add cost (of 1) as parameter
        break;
    case MOVEPRESET_TWIDDLE:
        setMove(ACTIONTYPE_MAXHEALTHUP, "Twiddle", "Range:1 Increases max health of target by 1 & deletes 1 sector from Fiddle", 1, 1, 0);
        // TODO: add cost (of 1) as parameter
        break;
    case MOVEPRESET_ZAP:
        // TODO: special effect - Deletes 2 sectors from all adjacent programs
        break;
    case MOVEPRESET_ZERO:
        setMove(ACTIONTYPE_TILEDELETE, "Zero", "Deletes one grid square", 1, 0, 0);
        break;
    case MOVEPRESET_CUSTOM:
        printf("err: tried to initialize ProgramAction of invalid type\n");
        exit(1);
        break;
    case MOVEPRESET_NUM_MOVEPRESETS:
        printf("err: tried to initialize ProgramAction of invalid type\n");
        exit(1);
        break;
    default:
        printf("err: tried to initialize ProgramAction of invalid type\n");
        exit(1);
        break;
    }
}

ProgramAction::~ProgramAction()
{
    if (debug >= DEBUG_NORMAL)
    {
        printf("Move '%s' deleted\n", name.c_str());
    }
}

void ProgramAction::setMove(ACTIONTYPE t, std::string name, std::string description, int range, int power, int reqSize)
{
    this->type = t;
    this->name = name;
    this->description = description;
    this->range = range;
    this->power = power;
    this->requiredSize = reqSize;
}

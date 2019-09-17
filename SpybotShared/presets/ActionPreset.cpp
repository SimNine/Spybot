#include "ActionPreset.h"

ActionPreset getActionPreset(ACTION a) {
	ActionPreset ret;
	ret.name_ = "";
	ret.desc_ = "";
	ret.type_ = ACTIONTYPE_NONE;
	ret.range_ = 0;
	ret.power_ = 0;
	ret.sizeCost_ = 0;
	ret.numUses_ = -1;
	ret.minSize_ = 0;
	ret.maxSize_ = -1;

	switch (a) {
	case ACTION_BASH:
		ret.name_ = "Bash";
		ret.desc_ = "Range:1 Damage:5";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 5;
		break;
	case ACTION_BLIP:
		ret.name_ = "Blip";
		ret.desc_ = "Range:5 Damage:1";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 5;
		ret.power_ = 1;
		break;
	case ACTION_BOOST:
		ret.name_ = "Boost";
		ret.desc_ = "Range:1 Increases move rate of target by 1 & deletes 1 sector from Turbo";
		ret.type_ = ACTIONTYPE_SPEEDUP;
		ret.range_ = 1;
		ret.power_ = 1;
		ret.sizeCost_ = 1;
		break;
	case ACTION_BURN:
		ret.name_ = "Burn";
		ret.desc_ = "Range:1 Damage:1";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 1;
		break;
	case ACTION_BYTE:
		ret.name_ = "Byte";
		ret.desc_ = "Range:1 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 2;
		break;
	case ACTION_CHUG:
		ret.name_ = "Chug";
		ret.desc_ = "Range:3 Decreases more rate of target by 2";
		ret.type_ = ACTIONTYPE_SPEEDDOWN;
		ret.range_ = 3;
		ret.power_ = 2;
		break;
	case ACTION_CONSTRUCTOR:
		ret.name_ = "Constructor";
		ret.desc_ = "Repairs one grid square";
		ret.type_ = ACTIONTYPE_TILEPLACE;
		ret.range_ = 3;
		ret.power_ = 0;
		break;
	case ACTION_CRASH:
		ret.name_ = "Crash";
		ret.desc_ = "Range:1 Damage:7";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 7;
		break;
	case ACTION_CRUNCHTIME:
		// TODO: special effect - DELETES 1 SECTOR FROM ALL ENEMY PROGRAMS THAT ARE CURRENTLY SIZE 3 OR BIGGER
		break;
	case ACTION_CUT:
		ret.name_ = "Cut";
		ret.desc_ = "Range:1 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 2;
		break;
	case ACTION_DARKSTORM:
		// TODO: special effect - DELETES 1 SECTOR FROM ALL PROGRAMS INCLUDING YOURS
		break;
	case ACTION_DATASLAM:
		ret.name_ = "Dataslam";
		ret.desc_ = "Sizereq:6 Deletes 8 sectors from target";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 8;
		ret.minSize_ = 6;
		break;
	case ACTION_DECONSTRUCTOR:
		ret.name_ = "Deconstructor";
		ret.desc_ = "Deletes one grid square";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 3;
		ret.power_ = 0;
		break;
	case ACTION_DICE:
		ret.name_ = "Dice";
		ret.desc_ = "Sizereq:3 Deletes 3 sectors from target";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 3;
		ret.minSize_ = 3;
		break;
	case ACTION_FIRE:
		ret.name_ = "Fire";
		ret.desc_ = "Range:2 Damage:4";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 2;
		ret.power_ = 4;
		break;
	case ACTION_FLASHGROW:
		// TODO: special effect - ADDS 1 SECTOR TO ALL OF YOUR PROGRAMS
		break;
	case ACTION_FLING:
		ret.name_ = "Fling";
		ret.desc_ = "Range:4 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 4;
		ret.power_ = 2;
		break;
	case ACTION_FRACTAL_GLITCH:
		ret.name_ = "Fractal Glitch";
		ret.desc_ = "Range:1 Damage:4";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 4;
		break;
	case ACTION_FRY:
		// TODO: special effect - DELETES 4 SECTORS FROM ALL TOUCHING PROGRAMS & DELETES 3 SECTORS FROM EEL
		break;
	case ACTION_GIGABOOST:
		ret.name_ = "Gigaboost";
		ret.desc_ = "Sizereq:4 Range:2 Adds 4 to move rate & deletes 3 sectors from Turbo Deluxe";
		ret.type_ = ACTIONTYPE_SPEEDUP;
		ret.range_ = 2;
		ret.power_ = 4;
		ret.minSize_ = 4;
		ret.sizeCost_ = 3;
		break;
	case ACTION_GLITCH:
		ret.name_ = "Glitch";
		ret.desc_ = "Range:1 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 2;
		break;
	case ACTION_GROW:
		ret.name_ = "Grow";
		ret.desc_ = "Range:1 Adds 2 sectors to target";
		ret.type_ = ACTIONTYPE_HEAL;
		ret.range_ = 1;
		ret.power_ = 2;
		break;
	case ACTION_HANG:
		ret.name_ = "Hang";
		ret.desc_ = "Sizereq:4 Range:3 Decreases move speed of target to 0";
		ret.type_ = ACTIONTYPE_SPEEDDOWN;
		ret.range_ = 3;
		ret.power_ = -1;
		ret.minSize_ = 4;
		break;
	case ACTION_HYPO:
		ret.name_ = "Hypo";
		ret.desc_ = "Range:3 Damage:2";
		ret.type_ = ACTIONTYPE_HEAL;
		ret.range_ = 3;
		ret.power_ = 2;
		break;
	case ACTION_ICE:
		ret.name_ = "Ice";
		ret.desc_ = "Range:2 Decreases movement of target by 3";
		ret.type_ = ACTIONTYPE_SPEEDDOWN;
		ret.range_ = 2;
		ret.power_ = 3;
		break;
	case ACTION_KAMIKAZEE:
		ret.name_ = "Kamikazee";
		ret.desc_ = "Range:1 Deletes 5 sectors from target and erases BuzzBomb";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 5;
		ret.sizeCost_ = -1;
		break;
	case ACTION_LAG:
		ret.name_ = "Lag";
		ret.desc_ = "Range:3 Decreases move rate of target by 1";
		ret.type_ = ACTIONTYPE_SPEEDDOWN;
		ret.range_ = 3;
		ret.power_ = 1;
		break;
	case ACTION_LAUNCH:
		ret.name_ = "Launch";
		ret.desc_ = "Range:3 Damage:3";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 3;
		ret.power_ = 3;
		break;
	case ACTION_MEGABOOST:
		ret.name_ = "Megaboost";
		ret.desc_ = "Sizereq:3 Range:2 Adds 2 to move rate & deletes 2 sectors from Turbo Deluxe";
		ret.type_ = ACTIONTYPE_SPEEDUP;
		ret.range_ = 2;
		ret.power_ = 2;
		ret.minSize_ = 3;
		ret.sizeCost_ = 2;
		break;
	case ACTION_MEGABYTE:
		ret.name_ = "Megabyte";
		ret.desc_ = "Range:1 Damage:3";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 3;
		break;
	case ACTION_MEGAGROW:
		ret.name_ = "Megagrow";
		ret.desc_ = "Range:1 Adds 4 sectors to target";
		ret.type_ = ACTIONTYPE_HEAL;
		ret.range_ = 1;
		ret.power_ = 4;
		break;
	case ACTION_MEGASCRAMBLE:
		ret.name_ = "Megascramble";
		ret.desc_ = "Range:3 Damage:4";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 3;
		ret.power_ = 4;
		break;
	case ACTION_MUTILATE:
		ret.name_ = "Mutilate";
		ret.desc_ = "Sizereq:4 Range:1 Damage:4";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 4;
		ret.minSize_ = 4;
		break;
	case ACTION_ONE:
		ret.name_ = "One";
		ret.desc_ = "Repairs one grid square";
		ret.type_ = ACTIONTYPE_TILEPLACE;
		ret.range_ = 1;
		ret.power_ = 0;
		break;
	case ACTION_OVERCLOCK:
		// TODO: special effect - adds 1 to the movement rate of all programs, including enemies
		break;
	case ACTION_PARALYZE:
		ret.name_ = "Paralyze";
		ret.desc_ = "Decreases move rate of target program by 3";
		ret.type_ = ACTIONTYPE_SPEEDDOWN;
		ret.range_ = 1;
		ret.power_ = 3;
		break;
	case ACTION_PEEK:
		ret.name_ = "Peek";
		ret.desc_ = "Range:2 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 2;
		ret.power_ = 2;
		break;
	case ACTION_PHASER:
		ret.name_ = "Phaser";
		ret.desc_ = "Range:2 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 2;
		ret.power_ = 2;
		break;
	case ACTION_PHOTON:
		ret.name_ = "Photon";
		ret.desc_ = "Range:3 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 3;
		ret.power_ = 2;
		break;
	case ACTION_PING:
		ret.name_ = "Ping";
		ret.desc_ = "Range:8 Damage:1";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 8;
		ret.power_ = 1;
		break;
	case ACTION_POKE:
		ret.name_ = "Poke";
		ret.desc_ = "Range:3 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 3;
		ret.power_ = 2;
		break;
	case ACTION_PONG:
		ret.name_ = "Pong";
		ret.desc_ = "Range:5 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 5;
		ret.power_ = 2;
		break;
	case ACTION_QUANTUM_GLITCH:
		ret.name_ = "Quantum Glitch";
		ret.desc_ = "Range:1 Damage:6";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 6;
		break;
	case ACTION_SCORCH:
		ret.name_ = "Scorch";
		ret.desc_ = "Range:3 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 3;
		ret.power_ = 2;
		break;
	case ACTION_SCRAMBLE:
		ret.name_ = "Scramble";
		ret.desc_ = "Range:2 Damage:4";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 2;
		ret.power_ = 4;
		break;
	case ACTION_SEEKANDDESTROY:
		ret.name_ = "Seek and Destroy";
		ret.desc_ = "Sizereq:5 Range:2 Deletes 5 sectors from target & 2 from Seeker";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 2;
		ret.power_ = 5;
		ret.minSize_ = 5;
		ret.sizeCost_ = 2;
		break;
	case ACTION_SELFDESTRUCT:
		ret.name_ = "Selfdestruct";
		ret.desc_ = "Sizereq:6 Deletes 10 sectors from target and erases LogicBomb";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 10;
		ret.minSize_ = 6;
		ret.sizeCost_ = -1;
		break;
	case ACTION_SHUTDOWN:
		ret.name_ = "Shutdown";
		ret.desc_ = "Range:5 Damage:5";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 5;
		ret.power_ = 5;
		break;
	case ACTION_SLICE:
		ret.name_ = "Slice";
		ret.desc_ = "Range:1 Damage:2";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 2;
		break;
	case ACTION_STING:
		ret.name_ = "Sting";
		ret.desc_ = "Range:1 Damage:1";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 1;
		break;
	case ACTION_STONE:
		ret.name_ = "Stone";
		ret.desc_ = "Range:3 Damage:1";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 3;
		ret.power_ = 1;
		break;
	case ACTION_STRETCH:
		ret.name_ = "Stretch";
		ret.desc_ = "Range:2 Increases max size of target by 1";
		ret.type_ = ACTIONTYPE_MAXHEALTHUP;
		ret.range_ = 2;
		ret.power_ = 1;
		break;
	case ACTION_SURGERY:
		ret.name_ = "Surgery";
		ret.desc_ = "Range:1 Increases max size of target by 1";
		ret.type_ = ACTIONTYPE_MAXHEALTHUP;
		ret.range_ = 1;
		ret.power_ = 1;
		break;
	case ACTION_TASER:
		ret.name_ = "Taser";
		ret.desc_ = "Range:1 Damage:4";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 4;
		break;
	case ACTION_THUMP:
		ret.name_ = "Thump";
		ret.desc_ = "Range:1 Damage:3";
		ret.type_ = ACTIONTYPE_DAMAGE;
		ret.range_ = 1;
		ret.power_ = 3;
		break;
	case ACTION_TWEAK:
		ret.name_ = "Tweak";
		ret.desc_ = "Range:1 Increases move rate of target by 1 & deletes 1 sector from Fiddle";
		ret.type_ = ACTIONTYPE_SPEEDUP;
		ret.range_ = 1;
		ret.power_ = 1;
		ret.sizeCost_ = 1;
		break;
	case ACTION_TWIDDLE:
		ret.name_ = "Twiddle";
		ret.desc_ = "Range:1 Increases max health of target by 1 & deletes 1 sector from Fiddle";
		ret.type_ = ACTIONTYPE_MAXHEALTHUP;
		ret.range_ = 1;
		ret.power_ = 1;
		ret.sizeCost_ = 1;
		break;
	case ACTION_ZAP:
		// TODO: special effect - Deletes 2 sectors from all adjacent programs
		break;
	case ACTION_ZERO:
		ret.name_ = "Zero";
		ret.desc_ = "Deletes one grid square";
		ret.type_ = ACTIONTYPE_TILEDELETE;
		ret.range_ = 1;
		ret.power_ = 0;
		break;

	case ACTION_FEAR:
		ret.name_ = "Fear";
		ret.desc_ = "Decreases the max actions of the target by 1";
		ret.type_ = ACTIONTYPE_MAXACTIONSDOWN;
		ret.range_ = 3;
		ret.power_ = 1;
		break;
	case ACTION_COURAGE:
		ret.name_ = "Courage";
		ret.desc_ = "Increases the max actions of the target by 1";
		ret.type_ = ACTIONTYPE_MAXACTIONSUP;
		ret.range_ = 3;
		ret.power_ = 1;
		break;
	case ACTION_TELEPORT:
		ret.name_ = "Teleport";
		ret.desc_ = "Moves the front of the targeted program to a random tile";
		ret.type_ = ACTIONTYPE_TELEPORT;
		ret.range_ = 1;
		ret.power_ = 0;
		break;
	case ACTION_TRANSMIT:
		ret.name_ = "Transmit";
		ret.desc_ = "Moves the front of the targeted program to another available Transmitter";
		ret.type_ = ACTIONTYPE_TRANSMIT;
		ret.range_ = 1;
		ret.power_ = 0;
		break;
	case ACTION_FRAGMENT:
		ret.name_ = "Fragment";
		ret.desc_ = "Randomly scatters the cells of the targeted program across the grid";
		ret.type_ = ACTIONTYPE_FRAGMENT;
		ret.range_ = 1;
		ret.power_ = 0;
		break;

	case ACTION_CUSTOM:
		log("CLIENT ERR: tried to initialize ProgramActionMirror of invalid type\n");
		exit(1);
		break;
	case ACTION_NUM_ACTIONPRESETS:
		log("CLIENT ERR: tried to initialize ProgramActionMirror of invalid type\n");
		exit(1);
		break;
	default:
		log("CLIENT ERR: tried to initialize ProgramActionMirror of invalid type\n");
		exit(1);
		break;
	}

	return ret;
}
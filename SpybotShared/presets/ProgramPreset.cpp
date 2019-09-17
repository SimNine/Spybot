#include "ProgramPreset.h"

ProgramPreset getProgramPreset(PROGRAM p) {
	ProgramPreset ret;
	ret.name_ = "";
	ret.desc_ = "";
	ret.maxActions_ = 1;
	ret.maxHealth_ = 1;
	ret.maxMoves_ = 0;
	ret.cost_ = 100;
	ret.action1_ = ACTION_NONE;
	ret.action2_ = ACTION_NONE;
	ret.action3_ = ACTION_NONE;

	switch (p) {
	case PROGRAM_NONE:
	case PROGRAM_NUM_PROGTYPES:
		log("SERVER ERR: trying to instantiate an invalid program type");
		exit(1);
		break;
	case PROGRAM_CUSTOM:
		break;
	case PROGRAM_BALLISTA:
		ret.name_ = "Ballista";
		ret.desc_ = "Extreme-range attack program";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 2;
		ret.cost_ = 3000;
		ret.action1_ = ACTION_FLING;
		break;
	case PROGRAM_BITMAN:
		ret.name_ = "Bit-Man";
		ret.desc_ = "Makes sectors of the grid appear or disappear";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 3;
		ret.cost_ = 250;
		ret.action1_ = ACTION_ONE;
		ret.action2_ = ACTION_ZERO;
		break;
	case PROGRAM_BITMAN2:
		ret.name_ = "Bit-Woman";
		ret.desc_ = "Makes distant sectors of the grid appear or disappear";
		ret.maxMoves_ = 4;
		ret.maxHealth_ = 2;
		ret.cost_ = 1000;
		ret.action1_ = ACTION_CONSTRUCTOR;
		ret.action2_ = ACTION_DECONSTRUCTOR;
		break;
	case PROGRAM_BLACKWIDOW:
		ret.name_ = "Black Widow";
		ret.desc_ = "Speedier and creepier";
		ret.maxMoves_ = 4;
		ret.maxHealth_ = 3;
		ret.cost_ = 2000;
		ret.action1_ = ACTION_BYTE;
		ret.action2_ = ACTION_PARALYZE;
		break;
	case PROGRAM_BOSS:
		ret.name_ = "Boss";
		ret.desc_ = "Prepare to be owned";
		ret.maxMoves_ = 6;
		ret.maxHealth_ = 25;
		ret.cost_ = 50000;
		ret.action1_ = ACTION_SHUTDOWN;
		break;
	case PROGRAM_BUG:
		ret.name_ = "Bug";
		ret.desc_ = "Fast, cheap, and out of control";
		ret.maxMoves_ = 5;
		ret.maxHealth_ = 1;
		ret.cost_ = 750;
		ret.action1_ = ACTION_GLITCH;
		break;
	case PROGRAM_BUG2:
		ret.name_ = "MandelBug";
		ret.desc_ = "It's not a bug, it's a feature";
		ret.maxMoves_ = 5;
		ret.maxHealth_ = 1;
		ret.cost_ = 3000;
		ret.action1_ = ACTION_FRACTAL_GLITCH;
		break;
	case PROGRAM_BUG3:
		ret.name_ = "HeisenBug";
		ret.desc_ = "They can't kill what they can't catch";
		ret.maxMoves_ = 5;
		ret.maxHealth_ = 1;
		ret.cost_ = 4000;
		ret.action1_ = ACTION_QUANTUM_GLITCH;
		break;
	case PROGRAM_CATAPULT:
		ret.name_ = "Catapult";
		ret.desc_ = "Extreme-range mobile attacker";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 3;
		ret.cost_ = 4000;
		ret.action1_ = ACTION_FLING;
		break;
	case PROGRAM_CLOG:
		ret.name_ = "Clog.01";
		ret.desc_ = "Slows down hostile programs";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 4;
		ret.cost_ = 1000;
		ret.action1_ = ACTION_LAG;
		break;
	case PROGRAM_CLOG2:
		ret.name_ = "Clog.02";
		ret.desc_ = "Twice as effective as version .01";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 4;
		ret.cost_ = 2000;
		ret.action1_ = ACTION_CHUG;
		break;
	case PROGRAM_CLOG3:
		ret.name_ = "Clog.03";
		ret.desc_ = "Brings hostile programs to a halt";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 4;
		ret.cost_ = 3500;
		ret.action1_ = ACTION_CHUG;
		ret.action2_ = ACTION_HANG;
		break;
	case PROGRAM_DATABOMB:
		ret.name_ = "LogicBomb";
		ret.desc_ = "Self-destructing attack program";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 6;
		ret.cost_ = 3500;
		ret.action1_ = ACTION_SELFDESTRUCT;
		break;
	case PROGRAM_DATADOCTOR:
		ret.name_ = "Data Doctor";
		ret.desc_ = "Helps your programs grow";
		ret.maxMoves_ = 4;
		ret.maxHealth_ = 5;
		ret.cost_ = 500;
		ret.action1_ = ACTION_GROW;
		break;
	case PROGRAM_DATADOCTOR2:
		ret.name_ = "Data Doctor Pro";
		ret.desc_ = "Twice the expansion power of Data Doctor";
		ret.maxMoves_ = 5;
		ret.maxHealth_ = 8;
		ret.cost_ = 1500;
		ret.action1_ = ACTION_MEGAGROW;
		ret.action2_ = ACTION_SURGERY;
		break;
	case PROGRAM_DOG:
		ret.name_ = "Guard Pup";
		ret.desc_ = "A speedy little corporate cur";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 2;
		ret.cost_ = 300;
		ret.action1_ = ACTION_BYTE;
		break;
	case PROGRAM_DOG2:
		ret.name_ = "Guard Dog";
		ret.desc_ = "Who let the dogs out?";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 3;
		ret.cost_ = 300;
		ret.action1_ = ACTION_BYTE;
		break;
	case PROGRAM_DOG3:
		ret.name_ = "Attack Dog";
		ret.desc_ = "Ravenous and bloodthirsty corporate canine";
		ret.maxMoves_ = 4;
		ret.maxHealth_ = 7;
		ret.cost_ = 300;
		ret.action1_ = ACTION_MEGABYTE;
		break;
	case PROGRAM_FIDDLE:
		ret.name_ = "Fiddle";
		ret.desc_ = "Twiddle and tweak the power of your programs";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 3;
		ret.cost_ = 2400;
		ret.action1_ = ACTION_TWEAK;
		ret.action2_ = ACTION_TWIDDLE;
		break;
	case PROGRAM_FIREWALL:
		ret.name_ = "Fire Wall";
		ret.desc_ = "Keeps unwanted programs out of corporate sectors";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 20;
		ret.cost_ = 300;
		ret.action1_ = ACTION_BURN;
		break;
	case PROGRAM_GOLEM:
		ret.name_ = "Golem.mud";
		ret.desc_ = "Slow and steady attack program";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 5;
		ret.cost_ = 1200;
		ret.action1_ = ACTION_THUMP;
		break;
	case PROGRAM_GOLEM2:
		ret.name_ = "Golem.clay";
		ret.desc_ = "Clay is stronger than mud";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 6;
		ret.cost_ = 3000;
		ret.action1_ = ACTION_BASH;
		break;
	case PROGRAM_GOLEM3:
		ret.name_ = "Golem.stone";
		ret.desc_ = "Nothing can stand in its way";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 7;
		ret.cost_ = 5000;
		ret.action1_ = ACTION_CRASH;
		break;
	case PROGRAM_HACK:
		ret.name_ = "Hack";
		ret.desc_ = "Basic attack program";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 4;
		ret.cost_ = 500;
		ret.action1_ = ACTION_SLICE;
		break;
	case PROGRAM_HACK2:
		ret.name_ = "Hack 2.0";
		ret.desc_ = "Improved Hack: larger size and better attacks";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 4;
		ret.cost_ = 1500;
		ret.action1_ = ACTION_SLICE;
		ret.action2_ = ACTION_DICE;
		break;
	case PROGRAM_HACK3:
		ret.name_ = "Hack 3.0";
		ret.desc_ = "The top of the Hack series";
		ret.maxMoves_ = 4;
		ret.maxHealth_ = 4;
		ret.cost_ = 3500;
		ret.action1_ = ACTION_SLICE;
		ret.action2_ = ACTION_MUTILATE;
		break;
	case PROGRAM_KAMIKAZEE:
		ret.name_ = "BuzzBomb";
		ret.desc_ = "Fast and annoying";
		ret.maxMoves_ = 8;
		ret.maxHealth_ = 2;
		ret.cost_ = 3500;
		ret.action1_ = ACTION_STING;
		ret.action2_ = ACTION_KAMIKAZEE;
		break;
	case PROGRAM_MEDIC:
		ret.name_ = "Medic";
		ret.desc_ = "Grows your programs from a distance";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 3;
		ret.cost_ = 1000;
		ret.action1_ = ACTION_HYPO;
		break;
	case PROGRAM_MEMHOG:
		ret.name_ = "Memory Hog";
		ret.desc_ = "Massive memory-filling bloatware";
		ret.maxMoves_ = 5;
		ret.maxHealth_ = 30;
		ret.cost_ = 300;
		break;
	case PROGRAM_MOBILETOWER:
		// TODO: check this program's description and moves again
		ret.name_ = "Mobile Tower";
		ret.desc_ = "Immobile long-range program";
		ret.maxMoves_ = 0;
		ret.maxHealth_ = 1;
		ret.cost_ = 1000;
		ret.action1_ = ACTION_LAUNCH;
		break;
	case PROGRAM_SATELLITE:
		ret.name_ = "Satellite";
		ret.desc_ = "Short-range hard-hitting program";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 1;
		ret.cost_ = 3500;
		ret.action1_ = ACTION_SCRAMBLE;
		break;
	case PROGRAM_SATELLITE2:
		ret.name_ = "Laser Satellite";
		ret.desc_ = "Long-range hard-hitting program";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 1;
		ret.cost_ = 5000;
		ret.action1_ = ACTION_MEGASCRAMBLE;
		break;
	case PROGRAM_SEEKER:
		ret.name_ = "Seeker";
		ret.desc_ = "Solid distance attack program";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 3;
		ret.cost_ = 1000;
		ret.action1_ = ACTION_PEEK;
		break;
	case PROGRAM_SEEKER2:
		ret.name_ = "Seeker 2.0";
		ret.desc_ = "Seeker 2.0";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 4;
		ret.cost_ = 2500;
		ret.action1_ = ACTION_POKE;
		break;
	case PROGRAM_SEEKER3:
		ret.name_ = "Seeker 3.0";
		ret.desc_ = "Seeker with extra deletion power";
		ret.maxMoves_ = 4;
		ret.maxHealth_ = 5;
		ret.cost_ = 4500;
		ret.action1_ = ACTION_POKE;
		ret.action2_ = ACTION_SEEKANDDESTROY;
		break;
	case PROGRAM_SLINGSHOT:
		ret.name_ = "Slingshot";
		ret.desc_ = "Basic ranged attack program";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 2;
		ret.cost_ = 750;
		ret.action1_ = ACTION_STONE;
		break;
	case PROGRAM_SONAR:
		ret.name_ = "Sensor";
		ret.desc_ = "Immobile program eradicator";
		ret.maxMoves_ = 0;
		ret.maxHealth_ = 1;
		ret.cost_ = 1750;
		ret.action1_ = ACTION_BLIP;
		break;
	case PROGRAM_SONAR2:
		ret.name_ = "Radar";
		ret.desc_ = "Long-range program eradicator";
		ret.maxMoves_ = 0;
		ret.maxHealth_ = 1;
		ret.cost_ = 1750;
		ret.action1_ = ACTION_PING;
		break;
	case PROGRAM_SONAR3:
		ret.name_ = "Radar";
		ret.desc_ = "Deadly program eradicator";
		ret.maxMoves_ = 0;
		ret.maxHealth_ = 1;
		ret.cost_ = 1750;
		ret.action1_ = ACTION_PONG;
		break;
	case PROGRAM_SPECS:
		ret.name_ = "Guru";
		ret.desc_ = "Multipurpose software for the l33tist of the l33t";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 3;
		ret.cost_ = 4500;
		ret.action1_ = ACTION_FIRE;
		ret.action2_ = ACTION_ICE;
		break;
	case PROGRAM_SUMO:
		ret.name_ = "Sumo";
		ret.desc_ = "A massive and slow-moving powerhouse";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 12;
		ret.cost_ = 4500;
		ret.action1_ = ACTION_DATASLAM;
		break;
	case PROGRAM_TARANTULA:
		ret.name_ = "Tarantula";
		ret.desc_ = "Fast, with a venomous bite";
		ret.maxMoves_ = 5;
		ret.maxHealth_ = 3;
		ret.cost_ = 3500;
		ret.action1_ = ACTION_MEGABYTE;
		ret.action2_ = ACTION_PARALYZE;
		break;
	case PROGRAM_TOWER:
		ret.name_ = "Tower";
		ret.desc_ = "Immobile long-range program";
		ret.maxMoves_ = 0;
		ret.maxHealth_ = 1;
		ret.cost_ = 1000;
		ret.action1_ = ACTION_LAUNCH;
		break;
	case PROGRAM_TURBO:
		ret.name_ = "Turbo";
		ret.desc_ = "Speeds up your programs";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 3;
		ret.cost_ = 1000;
		ret.action1_ = ACTION_BOOST;
		break;
	case PROGRAM_TURBO2:
		ret.name_ = "Turbo Deluxe";
		ret.desc_ = "Slow and steady is for losers";
		ret.maxMoves_ = 4;
		ret.maxHealth_ = 4;
		ret.cost_ = 1750;
		ret.action1_ = ACTION_MEGABOOST;
		break;
	case PROGRAM_TURBO3:
		ret.name_ = "Turbo DLC";
		ret.desc_ = "Gotta go fast";
		ret.maxMoves_ = 5;
		ret.maxHealth_ = 5;
		ret.cost_ = 3000;
		ret.action1_ = ACTION_GIGABOOST;
		break;
	case PROGRAM_WALKER:
		ret.name_ = "Sentinel";
		ret.desc_ = "Corporate data defender";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 3;
		ret.cost_ = 300;
		ret.action1_ = ACTION_CUT;
		break;
	case PROGRAM_WALKER2:
		ret.name_ = "Sentinel 2.0";
		ret.desc_ = "Improved corporate data defender";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 4;
		ret.cost_ = 300;
		ret.action1_ = ACTION_CUT;
		break;
	case PROGRAM_WALKER3:
		ret.name_ = "Sentinel 3.0";
		ret.desc_ = "Top of the line in corporate data defense";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 4;
		ret.cost_ = 300;
		ret.action1_ = ACTION_TASER;
		break;
	case PROGRAM_WARDEN:
		ret.name_ = "Warden";
		ret.desc_ = "Slow and steady corporate attack program";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 5;
		ret.cost_ = 2500;
		ret.action1_ = ACTION_THUMP;
		break;
	case PROGRAM_WARDEN2:
		ret.name_ = "Warden+";
		ret.desc_ = "Get out of its way";
		ret.maxMoves_ = 2;
		ret.maxHealth_ = 6;
		ret.cost_ = 3500;
		ret.action1_ = ACTION_BASH;
		break;
	case PROGRAM_WARDEN3:
		ret.name_ = "Warden++";
		ret.desc_ = "The last word in corporate security";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 7;
		ret.cost_ = 5000;
		ret.action1_ = ACTION_CRASH;
		break;
	case PROGRAM_WATCHMAN:
		ret.name_ = "Watchman";
		ret.desc_ = "Basic ranged attack program";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 2;
		ret.cost_ = 300;
		ret.action1_ = ACTION_PHASER;
		break;
	case PROGRAM_WATCHMAN2:
		ret.name_ = "Watchman X";
		ret.desc_ = "Improved version of Watchman";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 4;
		ret.cost_ = 300;
		ret.action1_ = ACTION_PHASER;
		break;
	case PROGRAM_WATCHMAN3:
		ret.name_ = "Watchman SP";
		ret.desc_ = "QUI CUSTODIET IPSOS CUSTODES?";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 4;
		ret.cost_ = 300;
		ret.action1_ = ACTION_PHOTON;
		break;
	case PROGRAM_WIZARD:
		ret.name_ = "Wizard";
		ret.desc_ = "Pay no attention to the man behind the curtain";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 4;
		ret.cost_ = 300;
		ret.action1_ = ACTION_SCORCH;
		ret.action2_ = ACTION_STRETCH;
		break;
	case PROGRAM_WOLFSPIDER:
		ret.name_ = "Wolf Spider";
		ret.desc_ = "Speedy and creepy little program";
		ret.maxMoves_ = 3;
		ret.maxHealth_ = 3;
		ret.cost_ = 750;
		ret.action1_ = ACTION_BYTE;
		break;
	case PROGRAM_MENTALIST:
		ret.name_ = "Mentalist";
		ret.desc_ = "A master manipulator of the mind";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 2;
		ret.cost_ = 10000;
		ret.action1_ = ACTION_FEAR;
		ret.action2_ = ACTION_COURAGE;
		break;
	case PROGRAM_TELEPORTER:
		ret.name_ = "Teleporter";
		ret.desc_ = "Streams programs aimlessly across the netscape";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 1;
		ret.cost_ = 10000;
		ret.action1_ = ACTION_TELEPORT;
		break;
	case PROGRAM_TRANSMITTER:
		ret.name_ = "Transmitter";
		ret.desc_ = "Links two or more points in the netscape";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 1;
		ret.cost_ = 15000;
		ret.action1_ = ACTION_TRANSMIT;
		break;
	case PROGRAM_FRAGMENTER:
		ret.name_ = "Fragmenter";
		ret.desc_ = "Pulls enemy programs apart";
		ret.maxMoves_ = 1;
		ret.maxHealth_ = 1;
		ret.cost_ = 20000;
		ret.action1_ = ACTION_FRAGMENT;
		break;
	}

	return ret;
}
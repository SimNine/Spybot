#pragma once

enum ANIMPARTICLE {
	ANIMPARTICLE_WHITEDOT = 0,
	ANIMPARTICLE_SELECTRING
};

enum ANIMTYPE {
	ANIMTYPE_ACTION_ATTACK = 0,
	ANIMTYPE_ACTION_TILEDESTROY,
	ANIMTYPE_ACTION_TILECREATE,
	ANIMTYPE_ACTION_HEAL,
	ANIMTYPE_ACTION_SPEEDUP,
	ANIMTYPE_ACTION_SPEEDDOWN,
	ANIMTYPE_ACTION_MAXHEALTHUP,
	ANIMTYPE_ACTION_MAXHEALTHDOWN,
	ANIMTYPE_PROGRAMTILEDESTROY,
	ANIMTYPE_PROGRAMTILECREATE,
	ANIMTYPE_SELECT,
	ANIMTYPE_TILEFADE
};

enum NODESTATUS {
	NODESTATUS_HIDDEN = 0,
	NODESTATUS_UNOWNED,
	NODESTATUS_UNOWNED_SELECTED,
	NODESTATUS_OWNED,
	NODESTATUS_OWNED_SELECTED
};

enum PROGRAM {
	// player programs
	PROGRAM_BALLISTA = 0,
	PROGRAM_BITMAN,
	PROGRAM_BITMAN2,
	PROGRAM_BLACKWIDOW,
	PROGRAM_BUG,
	PROGRAM_BUG2,
	PROGRAM_BUG3,
	PROGRAM_CATAPULT,
	PROGRAM_CLOG,
	PROGRAM_CLOG2,
	PROGRAM_CLOG3 = 10,
	PROGRAM_DATABOMB,
	PROGRAM_DATADOCTOR,
	PROGRAM_DATADOCTOR2,
	PROGRAM_FIDDLE,
	PROGRAM_GOLEM,
	PROGRAM_GOLEM2,
	PROGRAM_GOLEM3,
	PROGRAM_HACK,
	PROGRAM_HACK2,
	PROGRAM_HACK3 = 20,
	PROGRAM_KAMIKAZEE,
	PROGRAM_MEDIC,
	PROGRAM_MEMHOG,
	PROGRAM_MOBILETOWER,
	PROGRAM_SATELLITE,
	PROGRAM_SATELLITE2,
	PROGRAM_SEEKER,
	PROGRAM_SEEKER2,
	PROGRAM_SEEKER3,
	PROGRAM_SLINGSHOT = 30,
	PROGRAM_SPECS,
	PROGRAM_SUMO,
	PROGRAM_TARANTULA,
	PROGRAM_TOWER,
	PROGRAM_TURBO,
	PROGRAM_TURBO2,
	PROGRAM_TURBO3,
	PROGRAM_WIZARD,
	PROGRAM_WOLFSPIDER,

	// enemy programs
	PROGRAM_BOSS = 40,
	PROGRAM_DOG,
	PROGRAM_DOG2,
	PROGRAM_DOG3,
	PROGRAM_FIREWALL,
	PROGRAM_SONAR,
	PROGRAM_SONAR2,
	PROGRAM_SONAR3,
	PROGRAM_WALKER,
	PROGRAM_WALKER2,
	PROGRAM_WALKER3 = 50,
	PROGRAM_WARDEN,
	PROGRAM_WARDEN2,
	PROGRAM_WARDEN3,
	PROGRAM_WATCHMAN,
	PROGRAM_WATCHMAN2,
	PROGRAM_WATCHMAN3,

	// number of programs
	PROGRAM_NUM_PROGTYPES,

	// special numerations
	PROGRAM_NONE,
	PROGRAM_CUSTOM
};

enum ITEM {
	ITEM_NONE = 0,
	ITEM_CREDIT,
	ITEM_BIGCREDIT,
	ITEM_FILES,
	ITEM_NUM_ITEMS
};

enum TILE {
	TILE_NONE = 0,
	TILE_PLAIN,
	TILE_PLAIN2,
	TILE_PLAIN3,
	TILE_PLAIN4,
	TILE_PLAIN5,
	TILE_PLAIN6,
	TILE_PLAIN7,
	TILE_PLAIN8,
	TILE_PLAIN9,
	TILE_SPAWN,
	TILE_SPAWN2,
	TILE_NUM_TILES
};

enum BRUSH {
	BRUSH_NONE = 0,
	BRUSH_TILES,
	BRUSH_PROGRAMS,
	BRUSH_DELETEPROGRAMS,
	BRUSH_ITEMS,
	BRUSH_NUM_BRUSHES
};

enum ACTIONTYPE {
	ACTIONTYPE_NONE = 0,
	ACTIONTYPE_DAMAGE,
	ACTIONTYPE_HEAL,
	ACTIONTYPE_SPEEDUP,
	ACTIONTYPE_SPEEDDOWN,
	ACTIONTYPE_MAXHEALTHUP,
	ACTIONTYPE_MAXHEALTHDOWN,
	ACTIONTYPE_TILEPLACE,
	ACTIONTYPE_TILEDELETE,
	ACTIONTYPE_NUM_ACTIONTYPES
};

enum BACKGROUND {
	BKG_NONE = 0,
	BKG_DONUT,
	BKG_PHARM,
	BKG_MONKEY,
	BKG_CELL,
	BKG_PED,
	BKG_NUM_BKGS
};

enum ANCHOR {
	ANCHOR_NORTHWEST,
	ANCHOR_NORTH,
	ANCHOR_NORTHEAST,
	ANCHOR_EAST,
	ANCHOR_SOUTHEAST,
	ANCHOR_SOUTH,
	ANCHOR_SOUTHWEST,
	ANCHOR_WEST,
	ANCHOR_CENTER
};

enum FONT {
	FONT_NORMAL,
	FONT_BOLD
};

enum CAMPAIGN {
	CAMPAIGN_CLASSIC,
	CAMPAIGN_NIGHTFALL,
	CAMPAIGN_CUSTOM
};

enum GAMESTATUS {
	GAMESTATUS_PREGAME,
	GAMESTATUS_PLAYING,
	GAMESTATUS_END
};

enum DEBUG {
	DEBUG_NONE = 0,
	DEBUG_MINIMAL,
	DEBUG_NORMAL,
	DEBUG_EXTRA
};

enum MOVEPRESET {
	MOVEPRESET_MEGAGROW,
	MOVEPRESET_GROW,
	MOVEPRESET_SLICE,
	MOVEPRESET_DATASLAM,
	MOVEPRESET_STING,
	MOVEPRESET_KAMIKAZEE,
	MOVEPRESET_ZERO,
	MOVEPRESET_ONE,
	MOVEPRESET_STONE,
	MOVEPRESET_POKE,
	MOVEPRESET_SEEK_AND_DESTROY,
	MOVEPRESET_TWIDDLE,
	MOVEPRESET_TWEAK,
	MOVEPRESET_TASER,
	MOVEPRESET_MEGABYTE,
	MOVEPRESET_CUT,
	MOVEPRESET_PONG,
	MOVEPRESET_BLIP,
	MOVEPRESET_ZAP,
	MOVEPRESET_FRY,
	MOVEPRESET_PEEK,
	MOVEPRESET_MUTILATE,
	MOVEPRESET_DICE,
	MOVEPRESET_GLITCH,
	MOVEPRESET_FRACTAL_GLITCH,
	MOVEPRESET_QUANTUM_GLITCH,
	MOVEPRESET_BASH,
	MOVEPRESET_CRASH,
	MOVEPRESET_LAUNCH,
	MOVEPRESET_SCRAMBLE,
	MOVEPRESET_FLING,
	MOVEPRESET_HYPO,
	MOVEPRESET_BURN,
	MOVEPRESET_LAG,
	MOVEPRESET_CHUG,
	MOVEPRESET_HANG,
	MOVEPRESET_BOOST,
	MOVEPRESET_MEGABOOST,
	MOVEPRESET_FIRE,
	MOVEPRESET_ICE,
	MOVEPRESET_SCORCH,
	MOVEPRESET_STRETCH,
	MOVEPRESET_PHASER,
	MOVEPRESET_BYTE,
	MOVEPRESET_DARKSTORM,
	MOVEPRESET_FLASHGROW,
	MOVEPRESET_OVERCLOCK,
	MOVEPRESET_CRUNCHTIME,
	MOVEPRESET_PING,
	MOVEPRESET_PHOTON,
	MOVEPRESET_THUMP,
	MOVEPRESET_PARALYZE,
	MOVEPRESET_SELFDESTRUCT,
	MOVEPRESET_MEGASCRAMBLE,
	MOVEPRESET_SHUTDOWN,
	MOVEPRESET_SURGERY,
	MOVEPRESET_CONSTRUCTOR,
	MOVEPRESET_DECONSTRUCTOR,
	MOVEPRESET_GIGABOOST,

	MOVEPRESET_NUM_MOVEPRESETS,
	MOVEPRESET_CUSTOM
};

enum MAPPRESET {
	MAPPRESET_CLASSIC,
	MAPPRESET_NIGHTFALL,
	MAPPRESET_PROCEDURAL
};

enum GAMEMODE {
	GAMEMODE_COOP,
	GAMEMODE_FFA,
	GAMEMODE_TEAMDM
};

enum ACHIEVEMENT {
	ACHIEVEMENT_FIRSTATTEMPTEDCYBERCRIME,
	ACHIEVEMENT_FIRSTCOMMITTEDCYBERCRIME,
	ACHIEVEMENT_FIRSTSUCCESSFULCYBERCRIME,

	ACHIEVEMENT_BABYHACKER,
	ACHIEVEMENT_CASUALHACKER,
	ACHIEVEMENT_AMATEURHACKER,
	ACHIEVEMENT_EXPERIENCEDHACKER,
	ACHIEVEMENT_EXPERTHACKER,
	ACHIEVEMENT_PROFESSIONALHACKER,
	ACHIEVEMENT_HACKERMAN,

	ACHIEVEMENT_BITCOIN,
	ACHIEVEMENT_BITCOINCASH,
	ACHIEVEMENT_RIPPLE,
	ACHIEVEMENT_ETHEREUM,
	ACHIEVEMENT_LITECOIN,
	ACHIEVEMENT_RAIBLOCKS,
	ACHIEVEMENT_DOGECOIN,
	ACHIEVEMENT_GARLICOIN,

	ACHIEVEMENT_OLDSCHOOL,
	ACHIEVEMENT_NIGHTFALL,
	ACHIEVEMENT_DIGITALWANDERER,
	ACHIEVEMENT_DIGITALEXPLORER,
	ACHIEVEMENT_DIGITALCARTOGRAPHER,
	ACHIEVEMENT_DIGITALPIONEER,
	ACHIEVEMENT_DIGITALMAGELLAN,

	ACHIEVEMENT_PARTICIPATION,
	ACHIEVEMENT_ONEMINUTE,
	ACHIEVEMENT_FIVEMINUTES,
	ACHIEVEMENT_ONEHOUR,
	ACHIEVEMENT_FIVEHOURS,
	ACHIEVEMENT_TENHOURS,
	ACHIEVEMENT_FIFTYHOURS,
	ACHIEVEMENT_CENTENNIAL,
	ACHIEVEMENT_CASUALGAMER,
	ACHIEVEMENT_SERIOUSCASUALGAMER,
	ACHIEVEMENT_GAMER,
	ACHIEVEMENT_DEDICATEDGAMER,
	ACHIEVEMENT_TAKEABREAK,
	ACHIEVEMENT_TIMETOSTOP,
	ACHIEVEMENT_WHY,

	ACHIEVEMENT_NUM_ACHIEVEMENTS,

	ACHIEVEMENT_NONE
};
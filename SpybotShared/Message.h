#ifndef MESSAGE_H_INCLUDED
#define MESSAGE_H_INCLUDED

#define DEFAULT_MSG_TEXTSIZE 50
#define DEFAULT_MSGSIZE (46 + DEFAULT_MSG_TEXTSIZE)

#include "Enums.h"
#include "Coord.h"

enum MSGTYPE
{
    MSGTYPE_NONE,
    MSGTYPE_MOVE,
    MSGTYPE_ACTION,
    MSGTYPE_SOUND,
    MSGTYPE_INQUIRY,
    MSGTYPE_JOIN,
    MSGTYPE_LEAVE,
    MSGTYPE_RESYNCGAME,
    MSGTYPE_SELECT,
    MSGTYPE_NEXTTURN,
    MSGTYPE_LOAD,
	MSGTYPE_DONELOAD,
    MSGTYPE_AISTEP,
    MSGTYPE_INFO,
    MSGTYPE_CLEAR,
	MSGTYPE_CONNECT,
	MSGTYPE_DISCONNECT,
	MSGTYPE_TEXT
};

enum MSGSOUNDNAME
{
    MSGSOUNDNAME_MOVE,
    MSGSOUNDNAME_DAMAGE,
    MSGSOUNDNAME_HEAL,
    MSGSOUNDNAME_ZERO,
    MSGSOUNDNAME_ONE,
    MSGSOUNDNAME_SIZEMOD,
    MSGSOUNDNAME_PICKUPCREDIT
};

enum MSGINQUIRYTYPE
{
    MSGINQUIRYTYPE_TILECONTENTS,
    MSGINQUIRYTYPE_SELECTEDTILE,
    MSGINQUIRYTYPE_SELECTEDPROG
};

enum MSGSELECTTYPE
{
    MSGSELECTTYPE_PROGRAM,
    MSGSELECTTYPE_TILE,
    MSGSELECTTYPE_ACTION
};

enum MSGLEVELTYPE
{
    MSGLEVELTYPE_LOAD,
    MSGLEVELTYPE_SAVE,
    MSGLEVELTYPE_CLEAR
};

enum MSGINFOTYPE
{
    MSGINFOTYPE_TILE,
    MSGINFOTYPE_BKG,
    MSGINFOTYPE_ITEM,
    MSGINFOTYPE_PROGRAM,
    MSGINFOTYPE_ACTION,
	MSGINFOTYPE_GAMESTATUS
};

struct Message
{
	// message type
	MSGTYPE type;

	// general-use position fields
	Coord pos; // to
	Coord pos2; // from

	// select
	MSGSELECTTYPE selectType;

	// sound
	MSGSOUNDNAME soundType;
	char numRepeats;

	// level load
	char levelNum;
	MSGLEVELTYPE levelType;

	// inquiry
	MSGINQUIRYTYPE inquiryType;

	// info
	MSGINFOTYPE infoType;
	GAMESTATUS statusType;
	TILE tileType;
	BACKGROUND bkgType;
	PROGRAM progType;
	ITEM itemType;
	char team;

	// ids
    int clientID;
	int playerID;
	int programID;
	int actionID;

	// text
	char text[DEFAULT_MSG_TEXTSIZE];
};

void serializeMessage(char* buffer, Message m);

Message deserializeMessage(char* in);

void printMessage(Message m);

#endif // MESSAGE_H_INCLUDED

#include "Message.h"

#include "MiscUtil.h"

void serializeMessage(char* buffer, Message m) {
	int currByte = 0;

	// write type
	buffer[currByte] = m.type;
	currByte++;

	// write general-purpose coords
	serializeInt(&buffer[currByte], m.pos.x);
	currByte += 4;
	serializeInt(&buffer[currByte], m.pos.y);
	currByte += 4;
	serializeInt(&buffer[currByte], m.pos2.x);
	currByte += 4;
	serializeInt(&buffer[currByte], m.pos2.y);
	currByte += 4;

	// write select
	buffer[currByte] = m.selectType;
	currByte++;

	// write sound
	buffer[currByte] = m.soundType;
	currByte++;
	buffer[currByte] = m.numRepeats;
	currByte++;

	// write level
	buffer[currByte] = m.levelNum;
	currByte++;
	buffer[currByte] = m.levelType;
	currByte++;

	// write inquiry
	buffer[currByte] = m.inquiryType;
	currByte++;

	// write info
	buffer[currByte] = m.infoType;
	currByte++;
	buffer[currByte] = m.statusType;
	currByte++;
	buffer[currByte] = m.tileType;
	currByte++;
	buffer[currByte] = m.bkgType;
	currByte++;
	buffer[currByte] = m.progType;
	currByte++;
	buffer[currByte] = m.itemType;
	currByte++;
	buffer[currByte] = m.team;
	currByte++;

	// write IDs
	serializeInt(&buffer[currByte], m.clientID);
	currByte += 4;
	serializeInt(&buffer[currByte], m.playerID);
	currByte += 4;
	serializeInt(&buffer[currByte], m.programID);
	currByte += 4;
	serializeInt(&buffer[currByte], m.actionID);
	currByte += 4;

	// write gameselect
	buffer[currByte] = m.gameConfigType;
	currByte++;

	// write text
	m.text[DEFAULT_MSG_TEXTSIZE - 1] = '\0';
	strcpy_s(&buffer[currByte], DEFAULT_MSG_TEXTSIZE, m.text);
	currByte += DEFAULT_MSG_TEXTSIZE;
}

Message deserializeMessage(char* in) {
	Message m;
	int currByte = 0;

	// read type
	m.type = (MSGTYPE)in[currByte];
	currByte++;

	// read general-purpose coords
	deserializeInt(&in[currByte], &m.pos.x);
	currByte += 4;
	deserializeInt(&in[currByte], &m.pos.y);
	currByte += 4;
	deserializeInt(&in[currByte], &m.pos2.x);
	currByte += 4;
	deserializeInt(&in[currByte], &m.pos2.y);
	currByte += 4;

	// read select
	m.selectType = (MSGSELECTTYPE)in[currByte];
	currByte++;

	// read sound
	m.soundType = (MSGSOUNDNAME)in[currByte];
	currByte++;
	m.numRepeats = in[currByte];
	currByte++;

	// read level
	m.levelNum = in[currByte];
	currByte++;
	m.levelType = (MSGLEVELTYPE)in[currByte];
	currByte++;

	// read inquiry
	m.inquiryType = (MSGINQUIRYTYPE)in[currByte];
	currByte++;

	// read info
	m.infoType = (MSGINFOTYPE)in[currByte];
	currByte++;
	m.statusType = (GAMESTATUS)in[currByte];
	currByte++;
	m.tileType = (TILE)in[currByte];
	currByte++;
	m.bkgType = (BACKGROUND)in[currByte];
	currByte++;
	m.progType = (PROGRAM)in[currByte];
	currByte++;
	m.itemType = (ITEM)in[currByte];
	currByte++;
	m.team = in[currByte];
	currByte++;

	// read IDs
	deserializeInt(&in[currByte], &m.clientID);
	currByte += 4;
	deserializeInt(&in[currByte], &m.playerID);
	currByte += 4;
	deserializeInt(&in[currByte], &m.programID);
	currByte += 4;
	deserializeInt(&in[currByte], &m.actionID);
	currByte += 4;

	// read gameconfig
	m.gameConfigType = (MSGGAMECONFIGTYPE)in[currByte];
	currByte++;

	// read text
	strcpy_s(m.text, DEFAULT_MSG_TEXTSIZE, &in[currByte]);
	currByte += DEFAULT_MSG_TEXTSIZE;

	// return it
	return m;
}

void printMessage(Message m) {
	printf("%i: ", m.clientID);
	switch (m.type) {
	case MSGTYPE_ACTION:
		printf("MSGTYPE_ACTION\n");
		break;
	case MSGTYPE_AISTEP:
		printf("MSGTYPE_AISTEP\n");
		break;
	case MSGTYPE_CLEAR:
		printf("MSGTYPE_CLEAR\n");
		break;
	case MSGTYPE_NEXTTURN:
		printf("MSGTYPE_NEXTTURN\n");
		break;
	case MSGTYPE_INFO:
		printf("MSGTYPE_INFO - ");
		switch (m.infoType) {
		case MSGINFOTYPE_ACTION:
			printf("MSGINFOTYPE_ACTION\n");
			break;
		case MSGINFOTYPE_BKG:
			printf("MSGINFOTYPE_BKG\n");
			break;
		case MSGINFOTYPE_GAMESTATUS:
			printf("MSGINFOTYPE_GAMESTATUS\n");
			break;
		case MSGINFOTYPE_ITEM:
			printf("MSGINFOTYPE_ITEM\n");
			break;
		case MSGINFOTYPE_PROGRAM:
			printf("MSGINFOTYPE_PROGRAM\n");
			break;
		case MSGINFOTYPE_TILE:
			printf("MSGINFOTYPE_TILE\n");
			break;
		case MSGINFOTYPE_PROGINVENTORY:
			printf("MSGINFOTYPE_PROGINVENTORY\n");
			break;
		}
		break;
	case MSGTYPE_INQUIRY:
		printf("MSGTYPE_INQUIRY\n");
		break;
	case MSGTYPE_JOIN:
		printf("MSGTYPE_JOIN\n");
		break;
	case MSGTYPE_LEAVE:
		printf("MSGTYPE_LEAVE\n");
		break;
	case MSGTYPE_LOAD:
		printf("MSGTYPE_LOAD - Level #%i\n", m.levelNum);
		break;
	case MSGTYPE_DONELOAD:
		printf("MSGTYPE_DONELOAD\n");
		break;
	case MSGTYPE_MOVE:
		printf("MSGTYPE_MOVE\n");
		break;
	case MSGTYPE_NONE:
		printf("MSGTYPE_NONE\n");
		break;
	case MSGTYPE_RESYNCGAME:
		printf("MSGTYPE_RESYNCGAME\n");
		break;
	case MSGTYPE_SELECT:
		printf("MSGTYPE_SELECT - ");
		switch (m.selectType) {
		case MSGSELECTTYPE_TILE:
			printf("TILE - %i,%i\n", m.pos.x, m.pos.y);
			break;
		case MSGSELECTTYPE_PROGRAM:
			printf("PROGRAM - ID %i\n", m.programID);
			break;
		case MSGSELECTTYPE_ACTION:
			printf("ACTION - ID %i\n", m.actionID);
			break;
		}
		break;
	case MSGTYPE_SOUND:
		printf("MSGTYPE_SOUND\n");
		break;
	case MSGTYPE_CONNECT:
		printf("MSGTYPE_CONNECT\n");
		break;
	case MSGTYPE_DISCONNECT:
		printf("MSGTYPE_DISCONNECT\n");
		break;
	case MSGTYPE_TEXT:
		printf("MSGTYPE_TEXT - %s\n", m.text);
		break;
	case MSGTYPE_GAMECONFIG:
		printf("MSGTYPE_GAMECONFIG\n");
		break;
	case MSGTYPE_LOGIN:
		printf("MSGTYPE_LOGIN\n");
		break;
	case MSGTYPE_CREATEUSER:
		printf("MSGTYPE_CREATEUSER\n");
		break;
	case MSGTYPE_ERROR:
		printf("MSGTYPE_ERROR - %s\n", m.text);
		break;
	case MSGTYPE_PLACEPROG:
		printf("MSGTYPE_PLACEPROG\n");
		break;
	}
}
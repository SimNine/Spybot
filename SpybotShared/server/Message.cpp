#include "Message.h"

#include "Logger.h"
#include "MiscUtil.h"

void _serializeMessage(char* buffer, Message m) {
	int currByte = 0;

	// write type
	buffer[currByte] = m.type;
	currByte++;

	// write IDs
	serializeInt(&buffer[currByte], m.clientID);
	currByte += 4;
	serializeInt(&buffer[currByte], m.teamID);
	currByte += 4;
	serializeInt(&buffer[currByte], m.playerID);
	currByte += 4;
	serializeInt(&buffer[currByte], m.programID);
	currByte += 4;
	serializeInt(&buffer[currByte], m.actionID);
	currByte += 4;

	if (m.type == MSGTYPE_CHAT) { // if this is a chat message, fill the entire message with text
		m.text[DEFAULT_CHATSIZE - 1] = '\0';
		strcpy_s(&buffer[currByte], DEFAULT_CHATSIZE, m.text);
		currByte += DEFAULT_CHATSIZE;
	} else { // treat it as a regular message
		// write general-purpose coords
		serializeInt(&buffer[currByte], m.pos.x);
		currByte += 4;
		serializeInt(&buffer[currByte], m.pos.y);
		currByte += 4;
		serializeInt(&buffer[currByte], m.pos2.x);
		currByte += 4;
		serializeInt(&buffer[currByte], m.pos2.y);
		currByte += 4;

		// write general-purpose ints
		buffer[currByte] = m.num;
		currByte += 4;
		buffer[currByte] = m.num2;
		currByte += 4;

		// write select
		buffer[currByte] = m.selectType;
		currByte++;

		// write sound
		buffer[currByte] = m.soundType;
		currByte++;

		// write level
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
		buffer[currByte] = m.animType;
		currByte++;

		// write gameselect
		buffer[currByte] = m.gameConfigType;
		currByte++;

		// write text
		m.text[DEFAULT_MSG_TEXTSIZE - 1] = '\0';
		strcpy_s(&buffer[currByte], DEFAULT_MSG_TEXTSIZE, m.text);
		currByte += DEFAULT_MSG_TEXTSIZE;
	}
}

Message _deserializeMessage(char* in) {
	Message m;
	int currByte = 0;

	// read type
	m.type = (MSGTYPE)in[currByte];
	currByte++;

	// read IDs
	deserializeInt(&in[currByte], &m.clientID);
	currByte += 4;
	deserializeInt(&in[currByte], &m.teamID);
	currByte += 4;
	deserializeInt(&in[currByte], &m.playerID);
	currByte += 4;
	deserializeInt(&in[currByte], &m.programID);
	currByte += 4;
	deserializeInt(&in[currByte], &m.actionID);
	currByte += 4;

	if (m.type == MSGTYPE_CHAT) { // if this is a chat message, the whole thing is text
		strcpy_s(m.text, DEFAULT_CHATSIZE, &in[currByte]);
		currByte += DEFAULT_CHATSIZE;
	} else {
		// read general-purpose coords
		deserializeInt(&in[currByte], &m.pos.x);
		currByte += 4;
		deserializeInt(&in[currByte], &m.pos.y);
		currByte += 4;
		deserializeInt(&in[currByte], &m.pos2.x);
		currByte += 4;
		deserializeInt(&in[currByte], &m.pos2.y);
		currByte += 4;

		// read general-purpose ints
		deserializeInt(&in[currByte], &m.num);
		currByte++;
		deserializeInt(&in[currByte], &m.num2);
		currByte++;

		// read select
		m.selectType = (MSGSELECTTYPE)in[currByte];
		currByte++;

		// read sound
		m.soundType = (MSGSOUNDNAME)in[currByte];
		currByte++;

		// read level
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
		m.animType = (ANIMTYPE)in[currByte];
		currByte++;

		// read gameconfig
		m.gameConfigType = (MSGGAMECONFIGTYPE)in[currByte];
		currByte++;

		// read text
		strcpy_s(m.text, DEFAULT_MSG_TEXTSIZE, &in[currByte]);
		currByte += DEFAULT_MSG_TEXTSIZE;
	}

	// return it
	return m;
}

void _printMessage(Message m) {
	log(to_string(m.clientID) + " - ");
	switch (m.type) {
	case MSGTYPE_ACTION:
		log("MSGTYPE_ACTION\n");
		break;
	case MSGTYPE_AISTEP:
		log("MSGTYPE_AISTEP\n");
		break;
	case MSGTYPE_CLEAR:
		log("MSGTYPE_CLEAR\n");
		break;
	case MSGTYPE_NEXTTURN:
		log("MSGTYPE_NEXTTURN\n");
		break;
	case MSGTYPE_INFO:
		log("MSGTYPE_INFO - ");
		switch (m.infoType) {
		case MSGINFOTYPE_ACTION:
			log("MSGINFOTYPE_ACTION\n");
			break;
		case MSGINFOTYPE_BKG:
			log("MSGINFOTYPE_BKG\n");
			break;
		case MSGINFOTYPE_GAMESTATUS:
			log("MSGINFOTYPE_GAMESTATUS\n");
			break;
		case MSGINFOTYPE_ITEM:
			log("MSGINFOTYPE_ITEM - ITEM " + to_string(m.itemType) + "\n");
			break;
		case MSGINFOTYPE_TILE:
			log("MSGINFOTYPE_TILE - TILE " + to_string(m.tileType) + "\n");
			break;
		case MSGINFOTYPE_PROGINVENTORY:
			log("MSGINFOTYPE_PROGINVENTORY\n");
			break;
		case MSGINFOTYPE_CREDITS:
			log("MSGINFOTYPE_CREDITS - CREDITS " + to_string(m.num) + "\n");
			break;
		case MSGINFOTYPE_ANIM:
			log("MSGiNFOTYPE_ANIM - ANIM " + to_string(m.animType) + "\n");
			break;

		case MSGINFOTYPE_TEAM:
			log("MSGINFOTYPE_TEAM - TEAM " + to_string(m.teamID) + "\n");
			break;
		case MSGINFOTYPE_PLAYER:
			log("MSGINFOTYPE_PLAYER - PLAYER " + to_string(m.playerID) + "\n");
			break;
		case MSGINFOTYPE_PROGRAM:
			log("MSGINFOTYPE_PROGRAM - PROGRAM " + to_string(m.programID) + "\n");
			break;
		case MSGINFOTYPE_TEAMDELETE:
			log("MSGINFOTYPE_TEAMDELETE - TEAM " + to_string(m.teamID) + "\n");
			break;
		case MSGINFOTYPE_PLAYERDELETE:
			log("MSGINFOTYPE_PLAYERDELETE - PLAYER " + to_string(m.playerID) + "\n");
			break;
		case MSGINFOTYPE_PROGRAMDELETE:
			log("MSGINFOTYPE_PROGRAMDELETE - PROGRAM " + to_string(m.programID) + "\n");
			break;

		case MSGINFOTYPE_PROGRAMADDHEAD:
			log("MSGINFOTYPE_PROGRAMADDHEAD - PROGRAM " + to_string(m.programID) + ", TILE " + to_string(m.pos.x) + "," + to_string(m.pos.y) + "\n");
			break;
		case MSGINFOTYPE_PROGRAMADDTAIL:
			log("MSGINFOTYPE_PROGRAMADDTAIL - PROGRAM " + to_string(m.programID) + ", TILE " + to_string(m.pos.x) + "," + to_string(m.pos.y) + "\n");
			break;
		case MSGINFOTYPE_PROGRAMREMOVETILE:
			log("MSGINFOTYPE_PROGRAMREMOVETILE - PROGRAM " + to_string(m.programID) + ", TILE " + to_string(m.pos.x) + "," + to_string(m.pos.y) + "\n");
			break;
		case MSGINFOTYPE_PROGRAMCHANGENUMMOVES:
			log("MSGINFOTYPE_PROGRAMCHANGENUMMOVES - PROGRAM " + to_string(m.programID) + ", NUMMOVES " + to_string(m.num) + "\n");
			break;
		case MSGINFOTYPE_PROGRAMCHANGENUMACTIONS:
			log("MSGINFOTYPE_PROGRAMCHANGENUMACTIONS - PROGRAM " + to_string(m.programID) + ", NUMACTIONS " + to_string(m.num) + "\n");
			break;
		case MSGINFOTYPE_PROGRAMCHANGEMAXHEALTH:
			log("MSGINFOTYPE_PROGRAMCHANGEMAXHEALTH - PROGRAM " + to_string(m.programID) + ", MAXHEALTH " + to_string(m.num) + "\n");
			break;
		case MSGINFOTYPE_PROGRAMCHANGEMAXMOVES:
			log("MSGINFOTYPE_PROGRAMCHANGEMAXMOVES - PROGRAM " + to_string(m.programID) + ", MAXMOVES " + to_string(m.num) + "\n");
			break;
		case MSGINFOTYPE_PROGRAMCHANGEMAXACTIONS:
			log("MSGINFOTYPE_PROGRAMCHANGEMAXACTIONS - PROGRAM " + to_string(m.programID) + ", MAXACTIONS " + to_string(m.num) + "\n");
			break;
		}
		break;
	case MSGTYPE_INQUIRY:
		log("MSGTYPE_INQUIRY\n");
		break;
	case MSGTYPE_JOIN:
		log("MSGTYPE_JOIN\n");
		break;
	case MSGTYPE_LEAVE:
		log("MSGTYPE_LEAVE\n");
		break;
	case MSGTYPE_LOAD:
		log("MSGTYPE_LOAD - LEVEL " + to_string(m.num) + "\n");
		break;
	case MSGTYPE_DONELOAD:
		log("MSGTYPE_DONELOAD\n");
		break;
	case MSGTYPE_MOVE:
		log("MSGTYPE_MOVE\n");
		break;
	case MSGTYPE_NONE:
		log("MSGTYPE_NONE\n");
		break;
	case MSGTYPE_RESYNCGAME:
		log("MSGTYPE_RESYNCGAME\n");
		break;
	case MSGTYPE_SELECT:
		log("MSGTYPE_SELECT - ");
		switch (m.selectType) {
		case MSGSELECTTYPE_TILE:
			log("TILE " + to_string(m.pos.x) + "," + to_string(m.pos.y) + "\n");
			break;
		case MSGSELECTTYPE_PROGRAM:
			log("PROGRAM " + to_string(m.programID) + "\n");
			break;
		case MSGSELECTTYPE_ACTION:
			log("ACTION " + to_string(m.actionID) + "\n");
			break;
		}
		break;
	case MSGTYPE_SOUND:
		log("MSGTYPE_SOUND\n");
		break;
	case MSGTYPE_CONNECT:
		log("MSGTYPE_CONNECT\n");
		break;
	case MSGTYPE_DISCONNECT:
		log("MSGTYPE_DISCONNECT\n");
		break;
	case MSGTYPE_TEXT:
		log("MSGTYPE_TEXT - " + std::string(m.text) + "\n");
		break;
	case MSGTYPE_GAMECONFIG:
		log("MSGTYPE_GAMECONFIG\n");
		break;
	case MSGTYPE_LOGIN:
		log("MSGTYPE_LOGIN - " + std::string(m.text) + "\n");
		break;
	case MSGTYPE_CREATEUSER:
		log("MSGTYPE_CREATEUSER - " + std::string(m.text) + "\n");
		break;
	case MSGTYPE_ERROR:
		log("MSGTYPE_ERROR - " + std::string(m.text) + "\n");
		break;
	case MSGTYPE_PLACEPROG:
		log("MSGTYPE_PLACEPROG\n");
		break;
	case MSGTYPE_PROGINVENTORY:
		log("MSGTYPE_PROGINVENTORY - TYPE " + to_string(m.progType) + ", COUNT " + to_string(m.num) + "\n");
		break;
	case MSGTYPE_CREDITPICKUP:
		log("MSGTYPE_CREDITPICKUP - CREDITS " + to_string(m.num) + "\n");
		break;
	case MSGTYPE_LEVELUNLOCK:
		log("MSGTYPE_LEVELUNLOCK - LEVEL " + to_string(m.num) + "\n");
		break;
	}
}
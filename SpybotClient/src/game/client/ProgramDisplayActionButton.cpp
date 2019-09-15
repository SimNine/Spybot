#include "Standard.h"
#include "ProgramDisplayActionButton.h"

#include "Global.h"
#include "Data.h"
#include "GameScreen.h"
#include "Client.h"
#include "Game.h"
#include "Player.h"
#include "Message.h"
#include "ProgramAction.h"
#include "Program.h"
#include "ResourceLoader.h"
#include "MiscUtil.h"

ProgramDisplayActionButton::ProgramDisplayActionButton(ANCHOR a, Coord disp, Coord dims, GUIContainer* p, ProgramAction* action, int actionNum)
	: GUIObject(a, disp, dims, p)
{
	action_ = action;
	actionNum_ = actionNum;
}

ProgramDisplayActionButton::~ProgramDisplayActionButton()
{
    // nada
}

void ProgramDisplayActionButton::setTransparency(int a)
{
    startAlpha = a;
    endAlpha = a;
    currAlpha = a;
}

bool ProgramDisplayActionButton::mouseDown()
{
	_client->getPlayer()->setSelectedAction(action_);

	Message m;
	m.type = MSGTYPE_SELECT;
	m.selectType = MSGSELECTTYPE_ACTION;
	m.playerID = _client->getPlayer()->getPlayerID();
	m.programID = _client->getPlayer()->getSelectedProgram()->getProgramID();
	m.actionID = actionNum_; // TODO: incorporate actual actionIDs
	_client->sendMessage(m);

    return true;
}

bool ProgramDisplayActionButton::mouseUp()
{
    return true;
}

void ProgramDisplayActionButton::draw()
{
	// draw border
	if (_debug >= DEBUG_NORMAL)
		GUIObject::drawBounds();

	// draw background
	if (isMouseOver())
		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 140);
	else
		SDL_SetRenderDrawColor(_renderer, 120, 120, 120, 140);
	SDL_RenderFillRect(_renderer, &bounds);
	
	// create clip rect
	SDL_Rect clipRect = bounds;
	clipRect.x += 5;
	clipRect.y += 5;
	clipRect.w = 50;
	clipRect.h = 50;

	// draw action icon
	SDL_Color col = { 0, 0, 0, 0 };
	SDL_Texture* tex = NULL;
	if (action_->type == ACTIONTYPE_DAMAGE)
	{
		tex = _game_icon_action_attack;
		col = _color_action_attack;
	}
	else if (action_->type == ACTIONTYPE_HEAL || action_->type == ACTIONTYPE_MAXHEALTHUP)
	{
		tex = _game_icon_action_medic;
		col = _color_action_medic;
	}
	else if (action_->type == ACTIONTYPE_MAXHEALTHDOWN)
	{
		tex = _game_icon_action_unmedic;
		col = _color_action_unmedic;
	}
	else if (action_->type == ACTIONTYPE_SPEEDDOWN)
	{
		tex = _game_icon_action_slow;
		col = _color_action_slow;
	}
	else if (action_->type == ACTIONTYPE_SPEEDUP)
	{
		tex = _game_icon_action_speedup;
		col = _color_action_speedup;
	}
	else if (action_->type == ACTIONTYPE_TILEPLACE)
	{
		tex = _game_icon_action_tilecreate;
		col = _color_action_tilecreate;
	}
	else if (action_->type == ACTIONTYPE_TILEDELETE)
	{
		tex = _game_icon_action_tiledestroy;
		col = _color_action_tiledestroy;
	}
	else
		printf("CLIENT ERR: a nonexistent action type was found\n");
	SDL_SetTextureColorMod(_program_core_50px, col.r, col.g, col.b);
	SDL_RenderCopy(_renderer, _program_core_50px, NULL, &clipRect);
	clipRect.h = 45;
	clipRect.w = 45;
	SDL_RenderCopy(_renderer, tex, NULL, &clipRect);

	// draw action name
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	clipRect.x = bounds.x + 60;
	clipRect.y = bounds.y + 10;
	drawString(action_->name, FONT_NORMAL, 30, _color_white, clipRect);

	// draw action specs
	clipRect.x = bounds.x + bounds.w - 80;
	clipRect.y = bounds.y;
	drawString("Range: " + to_string(action_->range), FONT_NORMAL, 20, _color_white, clipRect);
	clipRect.y += 20;
	drawString("Power: " + to_string(action_->power), FONT_NORMAL, 20, _color_white, clipRect);
	clipRect.y += 20;
	drawString("Min Size: " + to_string(action_->requiredSize), FONT_NORMAL, 20, _color_white, clipRect);
}

void ProgramDisplayActionButton::tick(int t)
{

}

void ProgramDisplayActionButton::resetBounds()
{
    GUIObject::recomputePosition();
}

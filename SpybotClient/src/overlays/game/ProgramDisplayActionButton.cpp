#include "Standard.h"
#include "ProgramDisplayActionButton.h"

#include "Global.h"
#include "Data.h"
#include "GameOverlay.h"
#include "Client.h"
#include "GameMirror.h"
#include "PlayerMirror.h"
#include "Message.h"
#include "ProgramActionMirror.h"
#include "ProgramMirror.h"
#include "ResourceLoader.h"
#include "MiscUtil.h"
#include "ConnectionManager.h"

ProgramDisplayActionButton::ProgramDisplayActionButton(GUIContainer* p, ANCHOR a, Coord disp, Coord dims, ProgramActionMirror action, int actionNum)
	: GUIObject(p, a, disp, dims) {
	action_ = action;
	actionNum_ = actionNum;
}

ProgramDisplayActionButton::~ProgramDisplayActionButton() {
	// nada
}

void ProgramDisplayActionButton::setTransparency(int a) {
	currAlpha_ = a;
}

bool ProgramDisplayActionButton::mouseDown() {
	Message m;
	m.type = MSGTYPE_SELECT;
	m.selectType = MSGSELECTTYPE_ACTION;
	m.playerID = _client->getPlayer()->getPlayerID();
	m.programID = _client->getPlayer()->getSelectedProgram()->getProgramID();
	m.actionID = actionNum_; // TODO: incorporate actual actionIDs
	_connectionManager->sendMessage(m);

	return true;
}

bool ProgramDisplayActionButton::mouseUp() {
	return true;
}

void ProgramDisplayActionButton::draw() {
	// draw border
	if (_debug >= DEBUG_NORMAL)
		GUIObject::drawBounds();

	// draw background
	if (isMouseOver())
		SDL_SetRenderDrawColor(_renderer, 255, 0, 0, (int)(140 * (currAlpha_ / 255.0)));
	else
		SDL_SetRenderDrawColor(_renderer, 120, 120, 120, (int)(140 * (currAlpha_ / 255.0)));
	SDL_RenderFillRect(_renderer, &bounds_);

	// create transparent white color
	SDL_Color whiteMod = _color_white;
	whiteMod.a = currAlpha_;

	// create clip rect
	SDL_Rect clipRect = bounds_;
	clipRect.x += 5;
	clipRect.y += 5;
	clipRect.w = 50;
	clipRect.h = 50;

	// draw action icon
	SDL_Color col = { 0, 0, 0, 0 };
	SDL_Texture* tex = NULL;
	if (action_.type_ == ACTIONTYPE_DAMAGE) {
		tex = _game_icon_action_attack;
		col = _color_action_attack;
	} else if (action_.type_ == ACTIONTYPE_HEAL || action_.type_ == ACTIONTYPE_MAXHEALTHUP) {
		tex = _game_icon_action_medic;
		col = _color_action_medic;
	} else if (action_.type_ == ACTIONTYPE_MAXHEALTHDOWN) {
		tex = _game_icon_action_unmedic;
		col = _color_action_unmedic;
	} else if (action_.type_ == ACTIONTYPE_SPEEDDOWN) {
		tex = _game_icon_action_slow;
		col = _color_action_slow;
	} else if (action_.type_ == ACTIONTYPE_SPEEDUP) {
		tex = _game_icon_action_speedup;
		col = _color_action_speedup;
	} else if (action_.type_ == ACTIONTYPE_TILEPLACE) {
		tex = _game_icon_action_tilecreate;
		col = _color_action_tilecreate;
	} else if (action_.type_ == ACTIONTYPE_TILEDELETE) {
		tex = _game_icon_action_tiledestroy;
		col = _color_action_tiledestroy;
	} else if (action_.type_ == ACTIONTYPE_MAXACTIONSUP) {
		tex = _game_icon_action_maxactionsup;
		col = _color_action_maxactionsup;
	} else if (action_.type_ == ACTIONTYPE_MAXACTIONSDOWN) {
		tex = _game_icon_action_maxactionsdown;
		col = _color_action_maxactionsdown;
	} else
		log("CLIENT ERR: a nonexistent action type was found\n");
	SDL_SetTextureColorMod(_program_core_50px, col.r, col.g, col.b);
	SDL_SetTextureAlphaMod(_program_core_50px, currAlpha_);
	SDL_RenderCopy(_renderer, _program_core_50px, NULL, &clipRect);
	clipRect.h = 45;
	clipRect.w = 45;
	SDL_SetTextureAlphaMod(tex, currAlpha_);
	SDL_RenderCopy(_renderer, tex, NULL, &clipRect);

	// draw action name
	clipRect.x = bounds_.x + 60;
	clipRect.y = bounds_.y + 10;
	drawString(action_.name_, FONT_NORMAL, 30, whiteMod, clipRect);

	// draw action specs
	clipRect.x = bounds_.x + bounds_.w - 80;
	clipRect.y = bounds_.y;
	drawString("Range: " + to_string(action_.range_), FONT_NORMAL, 20, whiteMod, clipRect);
	clipRect.y += 20;
	drawString("Power: " + to_string(action_.power_), FONT_NORMAL, 20, whiteMod, clipRect);
	clipRect.y += 20;
	drawString("Min Size: " + to_string(action_.requiredSize_), FONT_NORMAL, 20, whiteMod, clipRect);

	// draw tooltip
	if (isMouseOver()) {
		SDL_Texture* descText = loadString(action_.description_, FONT_NORMAL, 20, whiteMod);
		SDL_QueryTexture(descText, NULL, NULL, &clipRect.w, &clipRect.h);
		clipRect.x = _mousePos.x - clipRect.w;
		clipRect.y = _mousePos.y;

		clipRect.x -= 5;
		clipRect.y -= 5;
		clipRect.w += 10;
		clipRect.h += 10;
		SDL_SetRenderDrawColor(_renderer, _color_bkg_standard.r, _color_bkg_standard.g, _color_bkg_standard.b, (int)((double)_color_bkg_standard.a * (currAlpha_ / 255.0)));
		SDL_RenderFillRect(_renderer, &clipRect);

		clipRect.x += 5;
		clipRect.y += 5;
		clipRect.w -= 10;
		clipRect.h -= 10;
		SDL_RenderCopy(_renderer, descText, NULL, &clipRect);
		SDL_DestroyTexture(descText);
	}
}

void ProgramDisplayActionButton::tick(int t) {

}

void ProgramDisplayActionButton::resetBounds() {
	GUIObject::recomputePosition();
}

#include "Standard.h"
#include "ProgramInventoryDisplay.h"

#include "ResourceLoader.h"
#include "GUITexture.h"
#include "Global.h"
#include "MiscUtil.h"
#include "Client.h"
#include "ClientMirror.h"
#include "GUIButtonParamaterized.h"
#include "GameOverlay.h"
#include "Message.h"
#include "PlayerMirror.h"
#include "ConnectionManager.h"

ProgramInventoryDisplay::ProgramInventoryDisplay(ANCHOR a, Coord disp, Coord dims, GUIContainer* parent)
	: GUIContainer(parent, a, disp, dims, _color_bkg_standard) {
	resetBounds();
}

ProgramInventoryDisplay::~ProgramInventoryDisplay() {
	//dtor
}

void ProgramInventoryDisplay::updateContents() {
	// clear contents
	while (contents_->getLength() > 0) {
		GUIObject* curr = contents_->removeFirst();
		delete curr;
	}

	// create new contents
	int w, h;
	GUITexture* label = new GUITexture(this, ANCHOR_NORTHEAST, { -20, 20 }, "My Programs", 40);
	addObject(label);

	int col = 0;
	int row = 0;
	for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
		if (i == PROGRAM_NONE || i == PROGRAM_CUSTOM) continue;

		if (_client->getMyClientMirror()->ownedProgs_[i] != 0) {
			// create the counter
			std::string str = "x" + to_string(_client->getMyClientMirror()->ownedProgs_[i]);
			SDL_Texture* tex = loadString(str, FONT_BOLD, 24, { 255, 255, 255, 255 });
			SDL_QueryTexture(tex, NULL, NULL, &w, &h);
			GUITexture* progTex = new GUITexture(this, ANCHOR_NORTHWEST, { 100 * col + 60, 100 + row*h }, { w, h }, tex, true);
			this->addObject(progTex);

			GUIButtonParamaterized<PROGRAM>* progButton = new GUIButtonParamaterized<PROGRAM>(this, ANCHOR_NORTHWEST, { 100 * col + 20, 100 + row*h }, { 30, 30 },
				_program_icons_paths[i], [] (PROGRAM p) {
				// check for associated player
				if (_client->getPlayer() == NULL) {
					log("CLIENT ERR: clicked programselectbutton without a player\n");
					return;
				}

				Message m;
				m.type = MSGTYPE_PLACEPROG;
				m.progType = p;
				m.pos = _client->getPlayer()->getSelectedTile();
				m.playerID = _client->getPlayer()->getPlayerID();
				_connectionManager->sendMessage(m);
			}, (PROGRAM)i);
			this->addObject(progButton);

			row++;
			if (100 + (row + 3)*h > _screenHeight) {
				col++;
				row = 0;
			}
		}
	}

	setDimensions({ 100 * (col + 1) + 40, _screenHeight - 40 });
}

void ProgramInventoryDisplay::resetBounds() {
	GUIContainer::resetBounds();
}

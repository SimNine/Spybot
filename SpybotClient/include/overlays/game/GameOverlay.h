#pragma once

#include "Standard.h"
#include "GUIContainer.h"

class ProgramDisplayContainer;
class GUIButton;
class GUITexture;
class ProgramInventoryDisplay;
class PlayerDisplayContainer;
class ChatDisplay;
class Animation;

class GameOverlay : public GUIContainer {
public:
	GameOverlay();
	virtual ~GameOverlay();

	// GUIContainer method overrides
	void draw();
	void tick(int);
	bool mouseDown();
	bool mouseUp();
	void resetBounds();
	void shiftBkg(Coord);
	void toggleViewTeams();
	void centerScreen();

	// gameplay methods
	void setPlayerTurnDisplay(std::string name);

	void setBackgroundImg(BACKGROUND);

	// game loading/saving methods
	void resetScreen();
	void changeGameStatus(GAMESTATUS gs);

	// misc
	void pauseMenuShow();
	void pauseMenuHide();
	void toggleTurnButtonShown(bool b);
	ChatDisplay* getChatDisplay();
	void addAnimation(Animation* a);
	void updateProgramInventoryDisplay();

	// win container
	void showWinContainer(int teamID);
	void hideWinContainer();

	// credit displays
	void showCreditPickup(int numCredits);
	void refreshCreditCounter();
protected:
private:
	SDL_Texture* bkgImg_;
	Coord bkgPos_;
	int textureTickCount_;
	bool canShiftScreen_;
	double shiftSpeed_;
	void drawBkg();
	void drawGrid();
	void checkShiftable();

	void buildGUI();

	bool programViewTeams_;
	bool programViewPlayers_;

	// containers
	GUIContainer* debugOptions_;
	GUIContainer* pauseMenu_;
	GUIButton* startGameButton_;
	GUIButton* turnButton_;

	// displays
	GUITexture* currTurn_;
	ProgramInventoryDisplay* progInv_;
	ProgramDisplayContainer* progDisp_;
	PlayerDisplayContainer* playerDisp_;
	ChatDisplay* chatDisplay_;

	// endgame container
	GUIContainer* winMenu_;
	GUITexture* winningTeam_;

	// credit pickup container
	GUIContainer* creditPickupContainer_;
	GUITexture* creditPickupTexture_;

	// credit counter container
	GUIContainer* creditCounterContainer_;
	GUITexture* creditCounterIcon_;
	GUITexture* creditCounterText_;

	// effects queue
	LinkedList<Animation*>* animList_;
};
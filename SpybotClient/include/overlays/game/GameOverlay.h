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
	void toggleViewTeams();

	void shiftBkg(Coord disp);
	void centerScreen();

	void setPlayerTurnDisplay(std::string name);
	void setBackgroundImg(BACKGROUND);

	// game loading/saving methods
	void changeGameStatus(GAMESTATUS gs);

	// misc
	void toggleTurnButtonShown(bool b);
	ChatDisplay* getChatDisplay();
	void addAnimation(Animation* a);
	void updateProgramInventoryDisplay();
	ProgramDisplayContainer* getProgramDisplayContainer();

	// container shows/hides
	void showWinContainer(int teamID);
	void hideWinContainer();

	void showPauseMenu();
	void hidePauseMenu();

	void showProgInv();
	void hideProgInv();

	void showPlayerDisp();
	void hidePlayerDisp();

	void startGameContainers();

	// credit displays
	void showCreditPickup(int numCredits);
	void refreshCreditCounter();
protected:
private:
	SDL_Texture* bkgImg_;
	Coord bkgPos_;
	int textureTickCount_;
	double shiftSpeed_;
	void drawBkg();
	void drawGrid();
	bool canShift();

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
	ProgramDisplayContainer* progDisp_;
	ChatDisplay* chatDisplay_;

	PlayerDisplayContainer* playerDisp_;
	GUIContainer* playerTab_;

	ProgramInventoryDisplay* progInv_;
	GUIContainer* progInvTab_;

	// endgame container
	GUIContainer* winMenu_;
	GUITexture* winningTeam_;

	// credit pickup container
	GUIContainer* creditPickupContainer_;
	GUITexture* creditPickupTexture_;

	// credit counter container
	GUIContainer* creditCounterContainer_;
	GUITexture* creditCounterText_;

	// effects queue
	LinkedList<Animation*>* animList_;
};
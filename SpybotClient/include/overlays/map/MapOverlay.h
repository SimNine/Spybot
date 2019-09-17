#pragma once

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class Node;
class GUIButton;
class ProgramInventoryDisplay;

class MapOverlay : public GUIContainer {
public:
	MapOverlay();
	virtual ~MapOverlay();

	void shiftBkg(double, double);
	void draw();
	void tick(int);
	bool mouseDown();
	void shiftTo(Node*);
	Node* getSelectedNode();
	void clearSelectedNode();

	void unlockAllLevels();
	void winNode(int nodeID);

	void saveMap(std::string);
	void loadMap(std::string);
	void switchMap(MAPPRESET);

	void hidePauseMenu();
	void showPauseMenu();

	void hideProgInv();
	void showProgInv();
	void updateProgramInvDisplay();
protected:
private:
	SDL_Texture* bkgImg_;
	double bkgX_, bkgY_;
	double shiftSpeed_;

	LinkedList<Node*>* nodeList_;
	bool isAnimOccurring_;
	Node* selectedNode_;
	GUIContainer* levelConfirm_;

	GUIContainer* progInvTab_;
	ProgramInventoryDisplay* progInv_;

	GUIContainer* pauseMenu_;

	void drawBkg();
	void drawNodes();

	void generateDefaultMaps();
};
#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include "Standard.h"
#include "GUIContainer.h"
#include "LinkedList.h"

class Node;
class GUIButton;
class ProgramInventoryDisplay;

class MapScreen : public GUIContainer
{
    public:
        MapScreen();
        virtual ~MapScreen();
        void shiftBkg(double, double);
        void draw();
        void tick(int);
        bool mouseDown();
        bool isBusy();
        void shiftTo(Node*);
        Node* getSelectedNode();
        void clearSelectedNode();
        void resetProgramInvDisplay();
        void toggleInvDisplay();
        void unlockAllLevels();

        void saveMap(std::string);
        void loadMap(std::string);
        void switchMap(MAPPRESET);

        void togglePauseMenu();
    protected:
    private:
        double bkgX;
        double bkgY;
        double shiftSpeed;
        LinkedList<Node*>* nodeList;
        bool isAnimOccurring;
        Node* selectedNode;
        void drawBkg();
        void drawNodes();
        GUIContainer* levelConfirm;
        GUIButton* invToggleButton;
        ProgramInventoryDisplay* invDisplay;
        GUIContainer* pauseMenu;

        void generateDefaultMaps();
};

#endif // MAPSCREEN_H

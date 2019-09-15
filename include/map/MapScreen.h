#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include "GUIContainer.h"
#include "Node.h"
#include "Enums.h"
#include "ProgramInventoryDisplay.h"
#include "GUIButton.h"

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
};

#endif // MAPSCREEN_H

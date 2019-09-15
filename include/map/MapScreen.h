#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include "GUIContainer.h"
#include "Node.h"

class MapScreen : public GUIContainer
{
    public:
        MapScreen();
        virtual ~MapScreen();
        void shiftBkg(int, int);
        void draw();
        bool click();
        bool isBusy();
        void shiftTo(Node*);
        Node* getSelectedNode();
        void clearSelectedNode();
    protected:
    private:
        int bkgX;
        int bkgY;
        LinkedList<Node*>* nodeList;
        bool isAnimOccurring;
        Node* selectedNode;
        void drawBkg();
        void drawNodes();
        GUIContainer* levelConfirm;
};

#endif // MAPSCREEN_H

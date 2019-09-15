#ifndef MAPSCREEN_H
#define MAPSCREEN_H

#include "GUIContainer.h"
#include "Node.h"

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
};

#endif // MAPSCREEN_H

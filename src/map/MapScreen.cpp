#include "MapScreen.h"
#include "Global.h"
#include "Main.h"
#include "Node.h"
#include "GUIButton.h"

#include <stdio.h>
#include <stdlib.h>

MapScreen::MapScreen()
    : GUIContainer(ANCHOR_TOP_LEFT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, loadTexture("resources/map/map.png"))
{
    // zone 1 nodes
    Node* baseNode = new Node(558, 881, 0, 1);
    nodeList = new LinkedList<Node*>(baseNode);

    Node* n1 = new Node(706, 882, 4, 1, "levels/n1.urf");
    LL_addObject(nodeList, n1);
    baseNode->addChild(n1);
    Node* n2 = new Node(462, 1025, 3, 1, "levels/n2.urf");
    LL_addObject(nodeList, n2);
    baseNode->addChild(n2);
    Node* n3 = new Node(464, 690, 7, 1);
    LL_addObject(nodeList, n3);
    baseNode->addChild(n3);
    Node* n5 = new Node(750, 786, 2, 1, "levels/n5.urf");
    LL_addObject(nodeList, n5);
    n1->addChild(n5);
    Node* n4 = new Node(607, 617, 2, 1, "levels/n4.urf");
    LL_addObject(nodeList, n4);
    n5->addChild(n4);
    Node* n6 = new Node(847, 667, 4, 1, "levels/n6.urf");
    LL_addObject(nodeList, n6);
    n5->addChild(n6);
    Node* n7 = new Node(800, 1025, 4, 1, "levels/n7.urf");
    LL_addObject(nodeList, n7);
    n1->addChild(n7);

    // zone 2 nodes
    Node* n8 = new Node(704, 666, 3, 2, "levels/n8.urf");
    LL_addObject(nodeList, n8);
    n5->addChild(n8);
    Node* n9 = new Node(749, 570, 4, 2, "levels/n9.urf");
    LL_addObject(nodeList, n9);
    n8->addChild(n9);
    Node* n10 = new Node(702, 1002, 3, 2, "levels/n10.urf");
    LL_addObject(nodeList, n10);
    n2->addChild(n10);
    Node* n11 = new Node(605, 1194, 2, 2, "levels/n11.urf");
    LL_addObject(nodeList, n11);
    n10->addChild(n11);
    Node* n12 = new Node(655, 1099, 7, 2);
    LL_addObject(nodeList, n12);
    n11->addChild(n12);
    Node* n13 = new Node(895, 1098, 4, 2, "levels/n13.urf");
    LL_addObject(nodeList, n13);
    n10->addChild(n13);
    Node* n14 = new Node(751, 1266, 3, 2, "levels/n14.urf");
    LL_addObject(nodeList, n14);
    n13->addChild(n14);
    Node* n15 = new Node(511, 1288, 3, 2, "levels/n15.urf");
    LL_addObject(nodeList, n15);
    n14->addChild(n15);
    Node* n16 = new Node(990, 978, 1, 2, "levels/n16.urf");
    LL_addObject(nodeList, n16);
    n13->addChild(n16);
    Node* n17 = new Node(1185, 882, 1, 2, "levels/n17.urf");
    LL_addObject(nodeList, n17);
    n16->addChild(n17);

    // zone 3 nodes
    Node* n18 = new Node(848, 450, 3, 3, "levels/n18.urf");
    LL_addObject(nodeList, n18);
    n9->addChild(n18);
    Node* n19 = new Node(1039, 425, 7, 3);
    LL_addObject(nodeList, n19);
    n18->addChild(n19);
    Node* n20 = new Node(989, 642, 5, 3, "levels/n20.urf");
    LL_addObject(nodeList, n20);
    n18->addChild(n20);
    Node* n21 = new Node(1089, 784, 4, 3, "levels/n21.urf");
    LL_addObject(nodeList, n21);
    n20->addChild(n21);
    Node* n22 = new Node(847, 809, 4, 3, "levels/n22.urf");
    LL_addObject(nodeList, n22);
    n21->addChild(n22);
    Node* n23 = new Node(1422, 931, 5, 3, "levels/n23.urf");
    LL_addObject(nodeList, n23);
    n17->addChild(n23);
    Node* n24 = new Node(1663, 954, 5, 3, "levels/n24.urf");
    LL_addObject(nodeList, n24);
    n23->addChild(n24);
    Node* n25 = new Node(1327, 785, 2, 3, "levels/n25.urf");
    LL_addObject(nodeList, n25);
    n23->addChild(n25);
    Node* n26 = new Node(1282, 689, 2, 3, "levels/n26.urf");
    LL_addObject(nodeList, n26);
    n25->addChild(n26);
    Node* n27 = new Node(1327, 1050, 1, 3, "levels/n27.urf");
    LL_addObject(nodeList, n27);
    n17->addChild(n27);
    Node* n28 = new Node(1566, 1074, 3, 3, "levels/n28.urf");
    LL_addObject(nodeList, n28);
    n27->addChild(n28);
    Node* n29 = new Node(1087, 1075, 1, 3, "levels/n29.urf");
    LL_addObject(nodeList, n29);
    n27->addChild(n29);
    Node* n30 = new Node(1232, 1240, 1, 3, "levels/n30.urf");
    LL_addObject(nodeList, n30);
    n27->addChild(n30);

    // zone 4 nodes
    Node* n31 = new Node(1471, 1218, 3, 4, "levels/n31.urf");
    LL_addObject(nodeList, n31);
    n30->addChild(n31);
    Node* n32 = new Node(1664, 1314, 7, 4);
    LL_addObject(nodeList, n32);
    n31->addChild(n32);
    Node* n34 = new Node(1807, 1051, 1, 4, "levels/n34.urf");
    LL_addObject(nodeList, n34);
    n31->addChild(n34);
    Node* n33 = new Node(1712, 1242, 3, 4, "levels/n33.urf");
    LL_addObject(nodeList, n33);
    n34->addChild(n33);
    Node* n35 = new Node(1902, 930, 5, 4, "levels/n35.urf");
    LL_addObject(nodeList, n35);
    n34->addChild(n35);
    Node* n36 = new Node(1712, 834, 4, 4, "levels/n36.urf");
    LL_addObject(nodeList, n36);
    n35->addChild(n36);
    Node* n37 = new Node(1570, 664, 5, 4, "levels/n37.urf");
    LL_addObject(nodeList, n37);
    n36->addChild(n37);
    Node* n38 = new Node(1424, 475, 5, 4, "levels/n38.urf");
    LL_addObject(nodeList, n38);
    n37->addChild(n38);
    Node* n39 = new Node(1352, 654, 5, 4, "levels/n39.urf");
    LL_addObject(nodeList, n39);
    n38->addChild(n39);
    Node* n40 = new Node(1232, 569, 4, 4, "levels/n40.urf");
    LL_addObject(nodeList, n40);
    n38->addChild(n40);

    // zone 5 nodes
    Node* n41 = new Node(1808, 642, 6, 5, "levels/n41.urf");
    LL_addObject(nodeList, n41);
    n36->addChild(n41);

    isAnimOccurring = false;
    selectedNode = NULL;
    baseNode->setNodeStatus(NODESTATUS_OWNED);

    bkgX = 0;
    bkgY = 0;

    levelConfirm = new GUIContainer(ANCHOR_TOP_LEFT, 20, 20, 252, 194, this, dataContainer->level_confirm_window);
    levelConfirm->setVisible(false);

    GUIButton* battleButton = new GUIButton(ANCHOR_TOP_LEFT, 130, 169, 115, 14, levelConfirm,
                                            []()
    {
        if (mapScreen->getSelectedNode()->getLevelStr().compare("undefined") != 0)
        {
            gameScreen->loadLevel(mapScreen->getSelectedNode()->getLevelStr());
            currScreen = gameScreen;
        }
        mapScreen->getSelectedNode()->winNode();
        mapScreen->clearSelectedNode();
    },
    dataContainer->begin_databattle_button_normal,
    dataContainer->begin_databattle_button_over,
    NULL);
    levelConfirm->addObject(battleButton);
    GUIButton* cancelButton = new GUIButton(ANCHOR_TOP_LEFT, 7, 169, 115, 14, levelConfirm,
                                            []()
    {
        mapScreen->clearSelectedNode();
    },
    dataContainer->cancel_button_normal,
    dataContainer->cancel_button_over,
    NULL);
    levelConfirm->addObject(cancelButton);

    addObject(levelConfirm);
}

MapScreen::~MapScreen()
{
    //dtor
}

void MapScreen::shiftBkg(int x, int y)
{
    int bkgWidth;
    int bkgHeight;
    SDL_QueryTexture(bkgImg, NULL, NULL, &bkgWidth, &bkgHeight);

    if (bkgX + x < 0)
    {
        bkgX = 0;
    }
    else if (bkgX + x + SCREEN_WIDTH > bkgWidth)
    {
        bkgX = bkgWidth - SCREEN_WIDTH;
    }
    else
    {
        bkgX += x;
    }

    if (bkgY + y < 0)
    {
        bkgY = 0;
    }
    else if (bkgY + y + SCREEN_HEIGHT > bkgHeight)
    {
        bkgY = bkgHeight - SCREEN_HEIGHT;
    }
    else
    {
        bkgY += y;
    }
}

void MapScreen::drawBkg()
{
    SDL_Rect bkgRect;
    bkgRect.x = bkgX;
    bkgRect.y = bkgY;
    bkgRect.w = SCREEN_WIDTH;
    bkgRect.h = SCREEN_HEIGHT;

    // draw background image
    SDL_RenderCopy(gRenderer, bkgImg, &bkgRect, NULL);
}

void MapScreen::drawNodes()
{
    LinkedList<Node*>* currNode = nodeList;
    while (currNode != NULL)
    {
        Node* currIcon = currNode->getContents();
        if (currIcon->getX() > bkgX - 200 &&
                currIcon->getY() > bkgY - 200 &&
                currIcon->getX() < bkgX + SCREEN_WIDTH + 200 &&
                currIcon->getY() < bkgY + SCREEN_HEIGHT + 200)
        {
            currIcon->getLevelStr();
            currIcon->draw(bkgX, bkgY);
        }
        currNode = currNode->getNext();
    }
}

void MapScreen::draw()
{
    drawBkg();
    drawNodes();
    drawContents();
}

bool MapScreen::mouseDown()
{
    GUIContainer::mouseDown();

    // if mapscreen animation is occurring, don't take input
    if (selectedNode != NULL)
    {
        return false;
    }

    // try clicking on node icons
    bool ret = false;
    LinkedList<Node*>* currNode = nodeList;
    while (currNode != NULL)
    {
        Node* n = currNode->getContents();
        if (n->getNodeStatus() != NODESTATUS_HIDDEN && n->isMouseOver(bkgX, bkgY))
        {
            ret = true;
            n->mouseDown();
            shiftTo(n);
        }
        currNode = currNode->getNext();
    }

    return ret;
}

void MapScreen::shiftTo(Node* n)
{
    // if mapscreen is already animating, don't do this shit
    if (isBusy())
    {
        return;
    }
    else
    {
        isAnimOccurring = true;
        selectedNode = n;
    }
}

bool MapScreen::isBusy()
{
    return (selectedNode != NULL);
}

Node* MapScreen::getSelectedNode()
{
    return selectedNode;
}

void MapScreen::clearSelectedNode()
{
    if (selectedNode->getNodeStatus() == NODESTATUS_UNOWNED_SELECTED)
    {
        selectedNode->setNodeStatus(NODESTATUS_UNOWNED);
    }
    else if (selectedNode->getNodeStatus() == NODESTATUS_OWNED_SELECTED)
    {
        selectedNode->setNodeStatus(NODESTATUS_OWNED);
    }
    levelConfirm->setVisible(false);
    selectedNode = NULL;
}

void MapScreen::tick()
{
    // tick all GUIObjects
    GUIContainer::tick();

    // if the mapscreen is currently animated, don't take shift input
    if (isAnimOccurring)
    {
        // get the width and height of the background
        int bkgWidth;
        int bkgHeight;
        SDL_QueryTexture(bkgImg, NULL, NULL, &bkgWidth, &bkgHeight);

        // variables representing if the map has shifted sufficiently
        bool xShift = false;
        bool yShift = false;
        int shiftSpeed = 2;

        // shift on the x-axis
        int xDis = selectedNode->getX() - bkgX - SCREEN_WIDTH/2;
        // if this node is left of center
        if (xDis < 0 && bkgX > 0)
        {
            if (xDis < -shiftSpeed) shiftBkg(-shiftSpeed, 0);
            else bkgX = selectedNode->getX() - SCREEN_WIDTH/2;
        }
        // if the node is right of center
        else if (xDis > 0 && bkgX < bkgWidth - SCREEN_WIDTH)
        {
            if (xDis > shiftSpeed) shiftBkg(shiftSpeed, 0);
            else bkgX = selectedNode->getX() - SCREEN_WIDTH/2;
        }
        else
        {
            xShift = true;
        }

        // shift on the y-axis
        int yDis = selectedNode->getY() - bkgY - SCREEN_HEIGHT/2;
        // if this node is above center
        if (yDis < 0 && bkgY > 0)
        {
            if (yDis < -shiftSpeed) shiftBkg(0, -shiftSpeed);
            else bkgY = selectedNode->getY() - SCREEN_HEIGHT/2;
        }
        // if this node is below center
        else if (yDis > 0 && bkgY < bkgHeight - SCREEN_HEIGHT)
        {
            if (yDis > shiftSpeed) shiftBkg(0, shiftSpeed);
            else bkgY = selectedNode->getY() - SCREEN_HEIGHT/2;
        }
        else
        {
            yShift = true;
        }

        // if done shifting
        if (xShift && yShift)
        {
            isAnimOccurring = false;
            levelConfirm->setVisible(true);
        }
        return;
    }
}
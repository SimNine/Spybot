#include "Standard.h"
#include "MapScreen.h"

#include "Global.h"
#include "DataContainer.h"
#include "GUIButton.h"
#include "Message.h"
#include "Node.h"
#include "Client.h"
#include "ProgramInventoryDisplay.h"
#include "MainScreen.h"
#include "ResourceLoader.h"

MapScreen::MapScreen()
    : GUIContainer(ANCHOR_NORTHWEST, {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT}, NULL, NULL)
{
    // check to make sure default maps are generated
    nodeList = new LinkedList<Node*>();
    loadMap("levels/classic.urf");
    if (nodeList->getLength() == 0)
        generateDefaultMaps();

    isAnimOccurring = false;
    selectedNode = NULL;
    bkgX = 200;
    bkgY = 500;
    shiftSpeed = 0.25;

    // create level confirm / cancel dialog box
    levelConfirm = new GUIContainer(ANCHOR_NORTHWEST, {20, 20}, {252, 194}, this, dataContainer->map_window_levelConfirm);
    levelConfirm->setTransparency(0);
    GUIButton* battleButton = new GUIButton(ANCHOR_NORTHWEST, {130, 169}, {115, 14}, levelConfirm,
                                            []()
    {
        if (mapScreen->getSelectedNode()->getNodeType() != 7)
        {
			Message msg;
			msg.type = MSGTYPE_LOAD;
			msg.levelNum = mapScreen->getSelectedNode()->getLevelId();
			client->sendMessage(msg);
        }
    },
    dataContainer->map_button_beginDatabattle_normal,
    dataContainer->map_button_beginDatabattle_over,
    NULL);
    levelConfirm->addObject(battleButton);
    GUIButton* cancelButton = new GUIButton(ANCHOR_NORTHWEST, {7, 169}, {115, 14}, levelConfirm,
                                            []()
    {
        mapScreen->clearSelectedNode();
    },
    dataContainer->map_button_cancel_normal,
    dataContainer->map_button_cancel_over,
    NULL);
    levelConfirm->addObject(cancelButton);

    addObject(levelConfirm);

    // add program display object
    invDisplay = new ProgramInventoryDisplay(ANCHOR_NORTHEAST, {-320, 20}, {300, 500}, this);
    addObject(invDisplay);

    // add pause menu
    pauseMenu = new GUIContainer(ANCHOR_CENTER, {-110, -95}, {220, 3*60 + 10}, this, {120, 120, 120, 140});
    GUIButton* resumeButton = new GUIButton(ANCHOR_NORTHWEST, {10, 10}, {200, 50}, pauseMenu,
                                                [](){mapScreen->togglePauseMenu();},
                                                dataContainer->game_button_resume);
    pauseMenu->addObject(resumeButton);
    GUIButton* exitToMainButton = new GUIButton(ANCHOR_NORTHWEST, {10, 70}, {200, 50}, pauseMenu,
                                                [](){currScreen = mainScreen;
                                                     mapScreen->togglePauseMenu();},
                                                dataContainer->game_button_quitToMain);
    pauseMenu->addObject(exitToMainButton);
    GUIButton* exitToDesktopButton = new GUIButton(ANCHOR_NORTHWEST, {10, 130}, {200, 50}, pauseMenu,
                                                   [](){quit = true;},
                                                   dataContainer->game_button_quitToDesktop);
    pauseMenu->addObject(exitToDesktopButton);
    pauseMenu->setTransparency(0);
    pauseMenu->setMovable(false);
    addObject(pauseMenu);
}

MapScreen::~MapScreen()
{
    //dtor
}

void MapScreen::shiftBkg(double x, double y)
{
    //printf("%04f,%04f\n", x, y);

    int bkgWidth;
    int bkgHeight;
    SDL_QueryTexture(bkgImg, NULL, NULL, &bkgWidth, &bkgHeight);

    if (bkgX + x < 0)
        bkgX = 0;
    else if (bkgX + x + SCREEN_WIDTH > bkgWidth)
        bkgX = bkgWidth - SCREEN_WIDTH;
    else
        bkgX += x;

    if (bkgY + y < 0)
        bkgY = 0;
    else if (bkgY + y + SCREEN_HEIGHT > bkgHeight)
        bkgY = bkgHeight - SCREEN_HEIGHT;
    else
        bkgY += y;
}

void MapScreen::drawBkg()
{
    SDL_Rect bkgRect;
    bkgRect.x = (int)bkgX;
    bkgRect.y = (int)bkgY;
    bkgRect.w = SCREEN_WIDTH;
    bkgRect.h = SCREEN_HEIGHT;

    // draw background image
    SDL_RenderCopy(gRenderer, bkgImg, &bkgRect, NULL);
}

void MapScreen::drawNodes()
{
    Iterator<Node*> it = nodeList->getIterator();
    while (it.hasNext())
    {
        Node* currIcon = it.next();
        if (currIcon->getPos().x > bkgX - 200 &&
            currIcon->getPos().y > bkgY - 200 &&
            currIcon->getPos().x < bkgX + SCREEN_WIDTH + 200 &&
            currIcon->getPos().y < bkgY + SCREEN_HEIGHT + 200)
        {
            currIcon->draw({(int)bkgX, (int)bkgY});
        }
    }
}

void MapScreen::draw()
{
    drawBkg();
    drawNodes();
    drawContents();

    if (pauseMenu->isVisible())
    {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 140);
        SDL_RenderFillRect(gRenderer, &bounds);

        SDL_Texture* pause = loadString("PAUSED", FONT_NORMAL, 200, {255, 255, 255, 0});
        SDL_Rect pauseRect = {50, 50, 0, 0};
        SDL_QueryTexture(pause, NULL, NULL, &pauseRect.w, &pauseRect.h);
        SDL_RenderCopy(gRenderer, pause, NULL, &pauseRect);
        SDL_DestroyTexture(pause);

        pauseMenu->draw();
    }
}

bool MapScreen::mouseDown()
{
    if (pauseMenu->isVisible())
        return pauseMenu->mouseDown();

    if (GUIContainer::mouseDown()) return true;

    // spits out the location (on the map bkg) of this click
    if (debug >= DEBUG_NORMAL) printf("%i,%i\n", (int)bkgX + mousePos.x, (int)bkgY + mousePos.y);

    // if mapscreen animation is occurring, don't take input
    if (selectedNode != NULL)
        return false;

    // try clicking on node icons
    bool ret = false;
    Iterator<Node*> it = nodeList->getIterator();
    while (it.hasNext())
    {
        Node* n = it.next();
        if (n->getNodeStatus() != NODESTATUS_HIDDEN && n->isMouseOver({(int)bkgX, (int)bkgY}))
        {
            ret = true;
            n->mouseDown();
            shiftTo(n);
        }
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
    if (selectedNode == NULL)
        return;

    if (selectedNode->getNodeStatus() == NODESTATUS_UNOWNED_SELECTED)
        selectedNode->setNodeStatus(NODESTATUS_UNOWNED);
    else if (selectedNode->getNodeStatus() == NODESTATUS_OWNED_SELECTED)
        selectedNode->setNodeStatus(NODESTATUS_OWNED);

    levelConfirm->setTransparency(0);
    selectedNode = NULL;
}

void MapScreen::tick(int ms)
{
    if (pauseMenu->isVisible())
        return;

    // tick all GUIObjects
    GUIContainer::tick(ms);
    double shiftAmt = shiftSpeed*ms;

    // if the mapscreen is currently animated, don't take shift input
    if (isAnimOccurring || selectedNode != NULL)
    {
        // get the width and height of the background
        int bkgWidth;
        int bkgHeight;
        SDL_QueryTexture(bkgImg, NULL, NULL, &bkgWidth, &bkgHeight);

        // variables representing if the map has shifted sufficiently
        bool xShift = false;
        bool yShift = false;

        // shift on the x-axis
        int xDis = (int)selectedNode->getPos().x - (int)bkgX - (int)(SCREEN_WIDTH/2);
        // if this node is left of center
        if (xDis < 0 && bkgX > 0)
        {
            if (xDis < -shiftAmt) shiftBkg(-shiftAmt, 0);
            else bkgX = selectedNode->getPos().x - SCREEN_WIDTH/2;
        }
        // if the node is right of center
        else if (xDis > 0 && bkgX < bkgWidth - SCREEN_WIDTH)
        {
            if (xDis > shiftAmt) shiftBkg(shiftAmt, 0);
            else bkgX = selectedNode->getPos().x - SCREEN_WIDTH/2;
        }
        else
        {
            xShift = true;
        }

        // shift on the y-axis
        int yDis = (int)selectedNode->getPos().y - (int)bkgY - (int)(SCREEN_HEIGHT/2);
        // if this node is above center
        if (yDis < 0 && bkgY > 0)
        {
            if (yDis < -shiftAmt) shiftBkg(0, -shiftAmt);
            else bkgY = selectedNode->getPos().y - SCREEN_HEIGHT/2;
        }
        // if this node is below center
        else if (yDis > 0 && bkgY < bkgHeight - SCREEN_HEIGHT)
        {
            if (yDis > shiftAmt) shiftBkg(0, shiftAmt);
            else bkgY = selectedNode->getPos().y - SCREEN_HEIGHT/2;
        }
        else
        {
            yShift = true;
        }

        // if done shifting
        if (xShift && yShift)
        {
            isAnimOccurring = false;
            levelConfirm->setTransparency(255);
        }
        return;
    }
    else
    {
        // scan for keys currently pressed
        const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
        if( currentKeyStates[ SDL_SCANCODE_UP ] )
        {
            shiftBkg(0, -shiftAmt);
        }
        else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
        {
            shiftBkg(0, shiftAmt);
        }

        if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
        {
            shiftBkg(-shiftAmt, 0);
        }
        else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
        {
            shiftBkg(shiftAmt, 0);
        }

        // if the mouse is at an edge, try to shift the background
        if (mousePos.x < 20)
        {
            shiftBkg(-shiftAmt, 0);
        }
        else if (mousePos.x > SCREEN_WIDTH - 20)
        {
            shiftBkg(shiftAmt, 0);
        }

        if (mousePos.y < 20)
        {
            shiftBkg(0, -shiftAmt);
        }
        else if (mousePos.y > SCREEN_HEIGHT - 20)
        {
            shiftBkg(0, shiftAmt);
        }
    }
}

void MapScreen::resetProgramInvDisplay()
{
    invDisplay->updateContents();
}

void MapScreen::toggleInvDisplay()
{
    if (invDisplay->isVisible()) invDisplay->setTransparency(0);
    else invDisplay->setTransparency(255);
}

void MapScreen::unlockAllLevels()
{
    nodeList->forEach([](Node* n){n->winNode();});
}

void MapScreen::generateDefaultMaps()
{
    // begin generating classic map
    if (nodeList == NULL)
        nodeList = new LinkedList<Node*>();
    else
        while (nodeList->getLength() > 0)
            delete nodeList->poll();

    // zone 1 nodes
    Node* baseNode = new Node({558, 881}, 0, 1, 0);
    nodeList->addLast(baseNode);
    Node* n1 = new Node({706, 882}, 4, 1, 1);
    nodeList->addLast(n1);
    baseNode->addNeighbor(n1);
    Node* n2 = new Node({462, 1025}, 3, 1, 2);
    nodeList->addLast(n2);
    baseNode->addNeighbor(n2);
    Node* n3 = new Node({464, 690}, 7, 1, 3);
    nodeList->addLast(n3);
    baseNode->addNeighbor(n3);
    Node* n5 = new Node({750, 786}, 2, 1, 5);
    nodeList->addLast(n5);
    n1->addNeighbor(n5);
    Node* n4 = new Node({607, 617}, 2, 1, 4);
    nodeList->addLast(n4);
    n5->addNeighbor(n4);
    Node* n6 = new Node({847, 667}, 4, 1, 6);
    nodeList->addLast(n6);
    n5->addNeighbor(n6);
    Node* n7 = new Node({800, 1025}, 4, 1, 7);
    nodeList->addLast(n7);
    n1->addNeighbor(n7);

    // zone 2 nodes
    Node* n8 = new Node({704, 666}, 3, 2, 8);
    nodeList->addLast(n8);
    n5->addNeighbor(n8);
    Node* n9 = new Node({749, 570}, 4, 2, 9);
    nodeList->addLast(n9);
    n8->addNeighbor(n9);
    Node* n10 = new Node({702, 1002}, 3, 2, 10);
    nodeList->addLast(n10);
    n2->addNeighbor(n10);
    Node* n11 = new Node({605, 1194}, 2, 2, 11);
    nodeList->addLast(n11);
    n10->addNeighbor(n11);
    Node* n12 = new Node({655, 1099}, 7, 2, 12);
    nodeList->addLast(n12);
    n11->addNeighbor(n12);
    Node* n13 = new Node({895, 1098}, 4, 2, 13);
    nodeList->addLast(n13);
    n10->addNeighbor(n13);
    Node* n14 = new Node({751, 1266}, 3, 2, 14);
    nodeList->addLast(n14);
    n13->addNeighbor(n14);
    Node* n15 = new Node({511, 1288}, 3, 2, 15);
    nodeList->addLast(n15);
    n14->addNeighbor(n15);
    Node* n16 = new Node({990, 978}, 1, 2, 16);
    nodeList->addLast(n16);
    n13->addNeighbor(n16);
    Node* n17 = new Node({1185, 882}, 1, 2, 17);
    nodeList->addLast(n17);
    n16->addNeighbor(n17);

    // zone 3 nodes
    Node* n18 = new Node({848, 450}, 3, 3, 18);
    nodeList->addLast(n18);
    n9->addNeighbor(n18);
    Node* n19 = new Node({1039, 425}, 7, 3, 19);
    nodeList->addLast(n19);
    n18->addNeighbor(n19);
    Node* n20 = new Node({989, 642}, 5, 3, 20);
    nodeList->addLast(n20);
    n18->addNeighbor(n20);
    Node* n21 = new Node({1089, 784}, 4, 3, 21);
    nodeList->addLast(n21);
    n20->addNeighbor(n21);
    Node* n22 = new Node({847, 809}, 4, 3, 22);
    nodeList->addLast(n22);
    n21->addNeighbor(n22);
    Node* n23 = new Node({1422, 931}, 5, 3, 23);
    nodeList->addLast(n23);
    n17->addNeighbor(n23);
    Node* n24 = new Node({1663, 954}, 5, 3, 24);
    nodeList->addLast(n24);
    n23->addNeighbor(n24);
    Node* n25 = new Node({1327, 785}, 2, 3, 25);
    nodeList->addLast(n25);
    n23->addNeighbor(n25);
    Node* n26 = new Node({1282, 689}, 2, 3, 26);
    nodeList->addLast(n26);
    n25->addNeighbor(n26);
    Node* n27 = new Node({1327, 1050}, 1, 3, 27);
    nodeList->addLast(n27);
    n17->addNeighbor(n27);
    Node* n28 = new Node({1566, 1074}, 3, 3, 28);
    nodeList->addLast(n28);
    n27->addNeighbor(n28);
    Node* n29 = new Node({1087, 1075}, 1, 3, 29);
    nodeList->addLast(n29);
    n27->addNeighbor(n29);
    Node* n30 = new Node({1232, 1240}, 1, 3, 30);
    nodeList->addLast(n30);
    n27->addNeighbor(n30);

    // zone 4 nodes
    Node* n31 = new Node({1471, 1218}, 3, 4, 31);
    nodeList->addLast(n31);
    n30->addNeighbor(n31);
    Node* n32 = new Node({1664, 1314}, 7, 4, 32);
    nodeList->addLast(n32);
    n31->addNeighbor(n32);
    Node* n34 = new Node({1807, 1051}, 1, 4, 34);
    nodeList->addLast(n34);
    n31->addNeighbor(n34);
    Node* n33 = new Node({1712, 1242}, 3, 4, 33);
    nodeList->addLast(n33);
    n34->addNeighbor(n33);
    Node* n35 = new Node({1902, 930}, 5, 4, 35);
    nodeList->addLast(n35);
    n34->addNeighbor(n35);
    Node* n36 = new Node({1712, 834}, 4, 4, 36);
    nodeList->addLast(n36);
    n35->addNeighbor(n36);
    Node* n37 = new Node({1570, 664}, 5, 4, 37);
    nodeList->addLast(n37);
    n36->addNeighbor(n37);
    Node* n38 = new Node({1424, 475}, 5, 4, 38);
    nodeList->addLast(n38);
    n37->addNeighbor(n38);
    Node* n39 = new Node({1352, 654}, 5, 4, 39);
    nodeList->addLast(n39);
    n38->addNeighbor(n39);
    Node* n40 = new Node({1232, 569}, 4, 4, 40);
    nodeList->addLast(n40);
    n38->addNeighbor(n40);

    // zone 5 nodes
    Node* n41 = new Node({1808, 642}, 6, 5, 41);
    nodeList->addLast(n41);
    n36->addNeighbor(n41);

    // save classic map
    saveMap("levels/classic.urf");
}

void MapScreen::saveMap(std::string url)
{
    std::ofstream m;
    m.open(url, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!m.is_open())
    {
        if (debug >= DEBUG_MINIMAL) printf("err opening file for saving map\n");
    }
    else
    {
        if (debug >= DEBUG_MINIMAL) printf("saving map %s...\n", url.c_str());

        // begin by writing the sizes of various data types
        int8_t sizeOfInt = sizeof(int);
        int8_t sizeOfChar = sizeof(char);
        int8_t sizeOfDouble = sizeof(double);
        int8_t sizeOfBool = sizeof(bool);
        if (debug >= DEBUG_NORMAL) printf("saving constants... int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);
        m.write((char*) &sizeOfInt, 1);
        m.write((char*) &sizeOfChar, 1);
        m.write((char*) &sizeOfDouble, 1);
        m.write((char*) &sizeOfBool, 1);

        // begin writing the map to file
        int numNodes = nodeList->getLength();
        m.write((char*) &numNodes, sizeOfInt);
        if (debug >= DEBUG_NORMAL) printf("saving %i nodes...\n", numNodes);

        // save each node's data
        for (int i = 0; i < numNodes; i++)
        {
            Node* curr = nodeList->getObjectAt(i);
            int xCoord = curr->getPos().x;
            int yCoord = curr->getPos().y;
            int type = curr->getNodeType();
            int zone = curr->getZone();
            int id = curr->getLevelId();
            m.write((char*) &xCoord, sizeOfInt);
            m.write((char*) &yCoord, sizeOfInt);
            m.write((char*) &type, sizeOfInt);
            m.write((char*) &zone, sizeOfInt);
            m.write((char*) &id, sizeOfInt);
        }

        // save each node's children
        if (debug >= DEBUG_NORMAL) printf("saving nodes' children...\n");
        for (int i = 0; i < numNodes; i++)
        {
            Node* curr = nodeList->getObjectAt(i);
            int numChildren = curr->getNeighbors()->getLength();
            m.write((char*) &numChildren, sizeOfInt);

            for (int j = 0; j < numChildren; j++)
            {
                int child = nodeList->getIndexOf(curr->getNeighbors()->getObjectAt(j));
                m.write((char*) &child, sizeOfInt);
            }
        }

        // flush and close the file
        if (debug >= DEBUG_MINIMAL) printf("flushing and closing map file... ");
        m.flush();
        m.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

void MapScreen::loadMap(std::string url)
{
    std::ifstream m;
    m.open(url, std::ios::in | std::ios::binary);
    if (!m.is_open())
    {
        if (debug >= DEBUG_MINIMAL) printf("err opening map %s\n", url.c_str());
    }
    else
    {
        if (debug >= DEBUG_MINIMAL) printf("loading map %s...\n", url.c_str());

        // read the sizes of various data types
        if (debug >= DEBUG_NORMAL) printf("loading constants...\n");
        int8_t sizeOfInt;
        m.read((char*) &sizeOfInt, 1);
        int8_t sizeOfChar;
        m.read((char*) &sizeOfChar, 1);
        int8_t sizeOfDouble;
        m.read((char*) &sizeOfDouble, 1);
        int8_t sizeOfBool;
        m.read((char*) &sizeOfBool, 1);

        // clear previous nodes
        if (nodeList == NULL)
            nodeList = new LinkedList<Node*>();
        else
            while (nodeList->getLength() > 0)
                delete nodeList->poll();

        // begin reading map data
        int numNodes;
        m.read((char*) &numNodes, sizeOfInt);

        // build and add the nodes
        for (int i = 0; i < numNodes; i++)
        {
            int xCoord, yCoord, type, zone, id;
            m.read((char*) &xCoord, sizeOfInt);
            m.read((char*) &yCoord, sizeOfInt);
            m.read((char*) &type, sizeOfInt);
            m.read((char*) &zone, sizeOfInt);
            m.read((char*) &id, sizeOfInt);

            nodeList->addLast(new Node({xCoord, yCoord}, type, zone, id));
        }

        // add nodes' children
        for (int i = 0; i < numNodes; i++)
        {
            Node* curr = nodeList->getObjectAt(i);

            int numChildren;
            m.read((char*) &numChildren, sizeOfInt);

            for (int j = 0; j < numChildren; j++)
            {
                int childIndex;
                m.read((char*) &childIndex, sizeOfInt);
                curr->addNeighbor(nodeList->getObjectAt(childIndex));
            }
        }

        // unlock the first node
        nodeList->getFirst()->winNode();

        // close the file
        m.close();
        if (debug >= DEBUG_MINIMAL) printf("done\n");
    }
}

void MapScreen::switchMap(MAPPRESET pre)
{
    switch (pre)
    {
    case MAPPRESET_CLASSIC:
        bkgImg = loadTexture("resources/map/map.png");
        loadMap("levels/classic.urf");
        progListCurrent = progListClassic;
        break;
    case MAPPRESET_NIGHTFALL:
        bkgImg = loadTexture("resources/map/nightfall/map.png");
        loadMap("levels/nightfall.urf");
        progListCurrent = progListNightfall;
        break;
    case MAPPRESET_PROCEDURAL:
        printf("placeholder: load procedural map\n");
        progListCurrent = progListCustom;
        break;
    default:
        break;
    }

    invDisplay->updateContents();
}

void MapScreen::togglePauseMenu()
{
    if (pauseMenu->isVisible()) pauseMenu->setTransparency(0);
    else pauseMenu->setTransparency(255);
}

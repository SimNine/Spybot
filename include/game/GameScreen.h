#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <string>

#include "GUIContainer.h"
#include "ProgramDisplayContainer.h"
#include "DataContainer.h"
#include "Program.h"
#include "Enums.h"
#include "Pair.h"
#include "Game.h"

class GameScreen : public GUIContainer
{
public:
    GameScreen();
    virtual ~GameScreen();

    // GUIContainer method overrides
    void draw();
    void tick(int);
    bool mouseDown();
    bool mouseUp();
    void resetBounds();
    void shiftBkg(int, int);
    void toggleViewTeams();

    // Game play methods
    Program* getSelectedProgram();
    void setSelectedProgram(Program*);
    void endTurn();

    // Game editing methods
    void setBrushMode(BRUSH);
    void setBrushTileType(TILE);
    void setBrushItem(ITEM);
    void setBrushProgramType(PROGRAM);
    void setBrushProgramTeam(int);
    void setBackground(BACKGROUND);

    // Game loading/saving methods
    void saveLevel();
    void loadLevel(std::string);
    void clearLevel();
protected:
private:
    int bkgX;
    int bkgY;
    int tickCount;
    bool canShiftScreen;
    double shiftSpeed;
    void drawBkg();
    void drawGrid();
    void checkShiftable();
    void centerScreen();
    int selectedTileX, selectedTileY;

    void buildGUI();
    ProgramDisplayContainer* progDisp;
    GUIContainer* gridSelectBrushPanel;
    BRUSH brushMode;
    GUIContainer* gridEditPanel;
    TILE brushTileType;
    GUIContainer* gridProgramEditPanel;
    PROGRAM brushProgramType;
    GUIContainer* gridItemEditPanel;
    ITEM brushItemType;
    GUIContainer* gridBkgPanel;
    int brushProgramTeam;
    bool programViewTeams;

    Game* game;
};

#endif // GAMESCREEN_H

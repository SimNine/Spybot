#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <string>

#include "GUIButton.h"
#include "GUIContainer.h"
#include "ProgramDisplayContainer.h"
#include "ProgramInventoryDisplay.h"
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
    void shiftBkg(Coord);
    void toggleViewTeams();

    // Game play methods
    Program* getSelectedProgram();
    void setSelectedProgram(Program*);
    void endTurn();
    void stepAI();
    void tryPlacingProgram(PROGRAM);
    void setGameStatus(GAMESTATUS);

    // Game editing methods
    void toggleEditorMode();
    void setBrushMode(BRUSH);
    void setBrushTileType(TILE);
    void setBrushItem(ITEM);
    void setBrushProgramType(PROGRAM);
    void setBrushProgramTeam(int);
    void setBackground(BACKGROUND);

    // Game loading/saving methods
    void saveGame();
    void loadGame(std::string);
    void clearGame();
protected:
private:
    Coord bkgPos;
    int textureTickCount;
    int turnTickCount;
    bool tickingAI;
    bool canShiftScreen;
    double shiftSpeed;
    void drawBkg();
    void drawGrid();
    void checkShiftable();
    void centerScreen();

    void buildGUI();

    // for use of the level editor
    bool editorMode;
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

    // buttons and interactables
    GUIButton* endTurnButton;
    GUIButton* stepButton;
    GUIButton* abandonGameButton;
    GUIButton* winGameButton;
    GUIButton* resetGameButton;
    GUIButton* startGameButton;
    GUIButton* backToMapButton;
    GUIButton* teamViewButton;

    ProgramInventoryDisplay* progInv;

    GAMESTATUS gameStatus;

    Game* game;
    Player* currPlayer;
};

#endif // GAMESCREEN_H

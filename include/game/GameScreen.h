#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <string>

#include "GUIContainer.h"
#include "ProgramDisplayContainer.h"
#include "DataContainer.h"
#include "Program.h"
#include "Enums.h"

class GameScreen : public GUIContainer
{
public:
    GameScreen();
    virtual ~GameScreen();

    void shiftBkg(int, int);
    void draw();
    bool mouseDown();
    void saveLevel();
    void loadLevel(std::string);
    void setSelectedTile(int, int);
    void setSelectedProgram(Program*);
    Program* getSelectedProgram();
    void setBrushMode(BRUSH);
    void setBrushTileType(TILE);
    void setBrushProgramType(PROGRAM);
    void setBrushProgramTeam(int);
    void toggleViewTeams();
    void setBrushItem(ITEM);
    void setBackground(BACKGROUND);
    void resetBounds();
protected:
private:
    int bkgX;
    int bkgY;
    void drawBkg();
    void drawContents();
    void checkShiftable();
    void calculateProgramDist(Program*);

    int gridLeftBound;
    int gridRightBound;
    int gridTopBound;
    int gridBottomBound;

    TILE gridTiles[200][200];
    ITEM gridItems[200][200];
    Program* gridPrograms[200][200];
    int gridProgramDist[200][200];

    void initBoard();
    void clearLevel();
    void deleteProgram(Program*);
    void setTileAt(int, int, TILE);

    void drawRectInBoard(TILE, int, int, int, int);
    void fillRectInBoard(TILE, int, int, int, int);
    void drawOvalInBoard(TILE, int, int, int, int);
    void fillOvalInBoard(TILE, int, int, int, int);
    bool isDrawValid(int, int, int, int);

    void buildGUI();

    ProgramDisplayContainer* progDisp;
    Program* selectedProgram;

    GUIContainer* gridSelectBrushPanel;
    BRUSH brushMode;
    GUIContainer* gridEditPanel;
    TILE brushTileType;
    GUIContainer* gridProgramEditPanel;
    PROGRAM brushProgramType;
    GUIContainer* gridItemEditPanel;
    ITEM brushItemType;
    GUIContainer* gridBkgPanel;
    BACKGROUND bkgTex;
    int brushProgramTeam;
    bool programViewTeams;
    int selectedTileX;
    int selectedTileY;
    bool canPanScreen;
};

#endif // GAMESCREEN_H

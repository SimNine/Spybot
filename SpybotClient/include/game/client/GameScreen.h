#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "Standard.h"
#include "GUIContainer.h"

class Program;
class Player;
class ProgramDisplayContainer;
class GUIButton;
class GUITexture;
class ProgramInventoryDisplay;
class PlayerDisplayContainer;

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
	void centerScreen();

    // gameplay methods
    void endTurn();
	void resumeTurn();
    void tryPlacingProgram(PROGRAM);

    // editor methods
    void toggleEditorMode();
    void setBrushMode(BRUSH);
    void setBrushTileType(TILE);
    void setBrushItem(ITEM);
    void setBrushProgramType(PROGRAM);
    void setBrushProgramTeam(int);
    void setBackgroundImg(BACKGROUND);

    // Game loading/saving methods
    void saveGame();
    void resetScreen();
	void changeGameStatus(GAMESTATUS gs);

    // misc
    void togglePauseMenu();
protected:
private:
    Coord bkgPos_;
    int textureTickCount_;
    int turnTickCount_;
    bool canShiftScreen_;
    double shiftSpeed_;
    void drawBkg();
    void drawGrid();
    void checkShiftable();

    void buildGUI();

    // for use of the level editor
    bool editorMode_;
    GUIContainer* gridSelectBrushPanel_;
    BRUSH brushMode_;
    GUIContainer* gridEditPanel_;
    TILE brushTileType_;
    GUIContainer* gridProgramEditPanel_;
    PROGRAM brushProgramType_;
    GUIContainer* gridItemEditPanel_;
    ITEM brushItemType_;
    GUIContainer* gridBkgPanel_;
    int brushProgramTeam_;
    bool programViewTeams_;
	bool programViewPlayers_;

    // containers
    GUIContainer* preGameOptions_;
    GUIContainer* debugOptions_;
    GUIContainer* pauseMenu_;
    GUIButton* turnButton_;

    // displays
    GUITexture* playerTurn_;
    GUITexture* aiTurn_;
    ProgramInventoryDisplay* progInv_;
	ProgramDisplayContainer* progDisp_;
	PlayerDisplayContainer* playerDisp_;
};

#endif // GAMESCREEN_H

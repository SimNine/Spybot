#include "GameScreen.h"
#include "GUIContainer.h"
#include "Global.h"
#include "Program.h"
#include "GUIButton.h"
#include "LinkedList.h"
#include "ResourceLoader.h"
#include "Pair.h"
#include "GUITexture.h"
#include "MiscUtil.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>

GameScreen::GameScreen()
    : GUIContainer(ANCHOR_NORTHWEST, {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT}, NULL, loadTexture("resources/company_4.png"))
{
    buildGUI();

    editorMode = false;
    brushMode = BRUSH_NONE;
    brushTileType = TILE_NONE;
    brushProgramType = PROGRAM_BALLISTA;
    brushProgramTeam = 0;
    brushItemType = ITEM_NONE;
    programViewTeams = false;
    textureTickCount = 0;
    turnTickCount = 0;
    tickingAI = false;
    bkgPos = {0, 0};
    shiftSpeed = 0.1;
    canShiftScreen = true;
    gameStatus = GAMESTATUS_NO_GAME;
    currPlayer = NULL;
    game = new Game();
}

GameScreen::~GameScreen()
{
    //dtor
}

void GameScreen::buildGUI()
{
    int ln = 0;
    int col = 0;
    gridEditPanel = new GUIContainer(ANCHOR_NORTHWEST, {20, 20}, {8 + 32*12, 36}, this, NULL);
    GUIButton* emptyButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_NONE);
    },
    dataContainer->tile_images[TILE_NONE]);
    GUIButton* plainButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN);
    },
    dataContainer->tile_images[TILE_PLAIN]);
    GUIButton* plain2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN2);
    },
    dataContainer->tile_images[TILE_PLAIN2]);
    GUIButton* plain3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN3);
    },
    dataContainer->tile_images[TILE_PLAIN3]);
    GUIButton* plain4Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN4);
    },
    dataContainer->tile_images[TILE_PLAIN4]);
    GUIButton* plain5Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN5);
    },
    dataContainer->tile_images[TILE_PLAIN5]);
    GUIButton* plain6Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN6);
    },
    dataContainer->tile_images[TILE_PLAIN6]);
    GUIButton* plain7Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN7);
    },
    dataContainer->tile_images[TILE_PLAIN7]);
    GUIButton* plain8Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN8);
    },
    dataContainer->tile_images[TILE_PLAIN9]);
    GUIButton* plain9Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN9);
    },
    dataContainer->tile_images[TILE_PLAIN9]);
    GUIButton* spawnButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_SPAWN);
    },
    dataContainer->tile_images[TILE_SPAWN]);
    GUIButton* spawn2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_SPAWN2);
    },
    dataContainer->tile_images[TILE_SPAWN2]);

    gridEditPanel->addObject(emptyButton);
    gridEditPanel->addObject(plainButton);
    gridEditPanel->addObject(plain2Button);
    gridEditPanel->addObject(plain3Button);
    gridEditPanel->addObject(plain4Button);
    gridEditPanel->addObject(plain5Button);
    gridEditPanel->addObject(plain6Button);
    gridEditPanel->addObject(plain7Button);
    gridEditPanel->addObject(plain8Button);
    gridEditPanel->addObject(plain9Button);
    gridEditPanel->addObject(spawnButton);
    gridEditPanel->addObject(spawn2Button);

    ln = 0;
    col = 0;
    gridProgramEditPanel = new GUIContainer(ANCHOR_NORTHWEST, {20, 60}, {32*15 + 8, 136}, this, NULL);
    GUIButton* ballistaButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BALLISTA);
    },
    dataContainer->program_icons[PROGRAM_BALLISTA],
    dataContainer->program_icons[PROGRAM_BALLISTA],
    dataContainer->program_icons[PROGRAM_BALLISTA]);
    gridProgramEditPanel->addObject(ballistaButton);
    GUIButton* bitmanButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BITMAN);
    },
    dataContainer->program_icons[PROGRAM_BITMAN],
    dataContainer->program_icons[PROGRAM_BITMAN],
    dataContainer->program_icons[PROGRAM_BITMAN]);
    gridProgramEditPanel->addObject(bitmanButton);
    GUIButton* bitman2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BITMAN2);
    },
    dataContainer->program_icons[PROGRAM_BITMAN2],
    dataContainer->program_icons[PROGRAM_BITMAN2],
    dataContainer->program_icons[PROGRAM_BITMAN2]);
    gridProgramEditPanel->addObject(bitman2Button);
    GUIButton* blackwidowButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BLACKWIDOW);
    },
    dataContainer->program_icons[PROGRAM_BLACKWIDOW],
    dataContainer->program_icons[PROGRAM_BLACKWIDOW],
    dataContainer->program_icons[PROGRAM_BLACKWIDOW]);
    gridProgramEditPanel->addObject(blackwidowButton);
    GUIButton* bossButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BOSS);
    },
    dataContainer->program_icons[PROGRAM_BOSS],
    dataContainer->program_icons[PROGRAM_BOSS],
    dataContainer->program_icons[PROGRAM_BOSS]);
    gridProgramEditPanel->addObject(bossButton);
    GUIButton* bugButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                         []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG);
    },
    dataContainer->program_icons[PROGRAM_BUG],
    dataContainer->program_icons[PROGRAM_BUG],
    dataContainer->program_icons[PROGRAM_BUG]);
    gridProgramEditPanel->addObject(bugButton);
    GUIButton* bug2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG2);
    },
    dataContainer->program_icons[PROGRAM_BUG2],
    dataContainer->program_icons[PROGRAM_BUG2],
    dataContainer->program_icons[PROGRAM_BUG2]);
    gridProgramEditPanel->addObject(bug2Button);
    GUIButton* bug3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG3);
    },
    dataContainer->program_icons[PROGRAM_BUG3],
    dataContainer->program_icons[PROGRAM_BUG3],
    dataContainer->program_icons[PROGRAM_BUG3]);
    gridProgramEditPanel->addObject(bug3Button);
    GUIButton* catapultButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CATAPULT);
    },
    dataContainer->program_icons[PROGRAM_CATAPULT],
    dataContainer->program_icons[PROGRAM_CATAPULT],
    dataContainer->program_icons[PROGRAM_CATAPULT]);
    gridProgramEditPanel->addObject(catapultButton);
    GUIButton* clogButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG);
    },
    dataContainer->program_icons[PROGRAM_CLOG],
    dataContainer->program_icons[PROGRAM_CLOG],
    dataContainer->program_icons[PROGRAM_CLOG]);
    gridProgramEditPanel->addObject(clogButton);
    GUIButton* clog2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG2);
    },
    dataContainer->program_icons[PROGRAM_CLOG2],
    dataContainer->program_icons[PROGRAM_CLOG2],
    dataContainer->program_icons[PROGRAM_CLOG2]);
    gridProgramEditPanel->addObject(clog2Button);
    GUIButton* clog3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG3);
    },
    dataContainer->program_icons[PROGRAM_CLOG3],
    dataContainer->program_icons[PROGRAM_CLOG3],
    dataContainer->program_icons[PROGRAM_CLOG3]);
    gridProgramEditPanel->addObject(clog3Button);
    GUIButton* databombButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DATABOMB);
    },
    dataContainer->program_icons[PROGRAM_DATABOMB],
    dataContainer->program_icons[PROGRAM_DATABOMB],
    dataContainer->program_icons[PROGRAM_DATABOMB]);
    gridProgramEditPanel->addObject(databombButton);
    GUIButton* datadoctorButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DATADOCTOR);
    },
    dataContainer->program_icons[PROGRAM_DATADOCTOR],
    dataContainer->program_icons[PROGRAM_DATADOCTOR],
    dataContainer->program_icons[PROGRAM_DATADOCTOR]);
    gridProgramEditPanel->addObject(datadoctorButton);
    ln++;
    col = 0;
    GUIButton* datadoctor2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DATADOCTOR2);
    },
    dataContainer->program_icons[PROGRAM_DATADOCTOR2],
    dataContainer->program_icons[PROGRAM_DATADOCTOR2],
    dataContainer->program_icons[PROGRAM_DATADOCTOR2]);
    gridProgramEditPanel->addObject(datadoctor2Button);
    GUIButton* dogButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                         []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG);
    },
    dataContainer->program_icons[PROGRAM_DOG],
    dataContainer->program_icons[PROGRAM_DOG],
    dataContainer->program_icons[PROGRAM_DOG]);
    gridProgramEditPanel->addObject(dogButton);
    GUIButton* dog2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG2);
    },
    dataContainer->program_icons[PROGRAM_DOG2],
    dataContainer->program_icons[PROGRAM_DOG2],
    dataContainer->program_icons[PROGRAM_DOG2]);
    gridProgramEditPanel->addObject(dog2Button);
    GUIButton* dog3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG3);
    },
    dataContainer->program_icons[PROGRAM_DOG3],
    dataContainer->program_icons[PROGRAM_DOG3],
    dataContainer->program_icons[PROGRAM_DOG3]);
    gridProgramEditPanel->addObject(dog3Button);
    GUIButton* fiddleButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_FIDDLE);
    },
    dataContainer->program_icons[PROGRAM_FIDDLE],
    dataContainer->program_icons[PROGRAM_FIDDLE],
    dataContainer->program_icons[PROGRAM_FIDDLE]);
    gridProgramEditPanel->addObject(fiddleButton);
    GUIButton* firewallButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_FIREWALL);
    },
    dataContainer->program_icons[PROGRAM_FIREWALL],
    dataContainer->program_icons[PROGRAM_FIREWALL],
    dataContainer->program_icons[PROGRAM_FIREWALL]);
    gridProgramEditPanel->addObject(firewallButton);
    GUIButton* golemButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM);
    },
    dataContainer->program_icons[PROGRAM_GOLEM],
    dataContainer->program_icons[PROGRAM_GOLEM],
    dataContainer->program_icons[PROGRAM_GOLEM]);
    gridProgramEditPanel->addObject(golemButton);
    GUIButton* golem2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM2);
    },
    dataContainer->program_icons[PROGRAM_GOLEM2],
    dataContainer->program_icons[PROGRAM_GOLEM2],
    dataContainer->program_icons[PROGRAM_GOLEM2]);
    gridProgramEditPanel->addObject(golem2Button);
    GUIButton* golem3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM3);
    },
    dataContainer->program_icons[PROGRAM_GOLEM3],
    dataContainer->program_icons[PROGRAM_GOLEM3],
    dataContainer->program_icons[PROGRAM_GOLEM3]);
    gridProgramEditPanel->addObject(golem3Button);
    GUIButton* hackButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK);
    },
    dataContainer->program_icons[PROGRAM_HACK],
    dataContainer->program_icons[PROGRAM_HACK],
    dataContainer->program_icons[PROGRAM_HACK]);
    gridProgramEditPanel->addObject(hackButton);
    GUIButton* hack2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK2);
    },
    dataContainer->program_icons[PROGRAM_HACK2],
    dataContainer->program_icons[PROGRAM_HACK2],
    dataContainer->program_icons[PROGRAM_HACK2]);
    gridProgramEditPanel->addObject(hack2Button);
    GUIButton* hack3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK3);
    },
    dataContainer->program_icons[PROGRAM_HACK3],
    dataContainer->program_icons[PROGRAM_HACK3],
    dataContainer->program_icons[PROGRAM_HACK3]);
    gridProgramEditPanel->addObject(hack3Button);
    GUIButton* kamikazeeButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_KAMIKAZEE);
    },
    dataContainer->program_icons[PROGRAM_KAMIKAZEE],
    dataContainer->program_icons[PROGRAM_KAMIKAZEE],
    dataContainer->program_icons[PROGRAM_KAMIKAZEE]);
    gridProgramEditPanel->addObject(kamikazeeButton);
    GUIButton* medicButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_MEDIC);
    },
    dataContainer->program_icons[PROGRAM_MEDIC],
    dataContainer->program_icons[PROGRAM_MEDIC],
    dataContainer->program_icons[PROGRAM_MEDIC]);
    gridProgramEditPanel->addObject(medicButton);
    GUIButton* memhogButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_MEMHOG);
    },
    dataContainer->program_icons[PROGRAM_MEMHOG],
    dataContainer->program_icons[PROGRAM_MEMHOG],
    dataContainer->program_icons[PROGRAM_MEMHOG]);
    gridProgramEditPanel->addObject(memhogButton);
    col = 0;
    ln++;
    GUIButton* mobiletowerButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_MOBILETOWER);
    },
    dataContainer->program_icons[PROGRAM_MOBILETOWER],
    dataContainer->program_icons[PROGRAM_MOBILETOWER],
    dataContainer->program_icons[PROGRAM_MOBILETOWER]);
    gridProgramEditPanel->addObject(mobiletowerButton);
    GUIButton* satelliteButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SATELLITE);
    },
    dataContainer->program_icons[PROGRAM_SATELLITE],
    dataContainer->program_icons[PROGRAM_SATELLITE],
    dataContainer->program_icons[PROGRAM_SATELLITE]);
    gridProgramEditPanel->addObject(satelliteButton);
    GUIButton* satellite2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SATELLITE2);
    },
    dataContainer->program_icons[PROGRAM_SATELLITE2],
    dataContainer->program_icons[PROGRAM_SATELLITE2],
    dataContainer->program_icons[PROGRAM_SATELLITE2]);
    gridProgramEditPanel->addObject(satellite2Button);
    GUIButton* seekerButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER);
    },
    dataContainer->program_icons[PROGRAM_SEEKER],
    dataContainer->program_icons[PROGRAM_SEEKER],
    dataContainer->program_icons[PROGRAM_SEEKER]);
    gridProgramEditPanel->addObject(seekerButton);
    GUIButton* seeker2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER2);
    },
    dataContainer->program_icons[PROGRAM_SEEKER2],
    dataContainer->program_icons[PROGRAM_SEEKER2],
    dataContainer->program_icons[PROGRAM_SEEKER2]);
    gridProgramEditPanel->addObject(seeker2Button);
    GUIButton* seeker3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER3);
    },
    dataContainer->program_icons[PROGRAM_SEEKER3],
    dataContainer->program_icons[PROGRAM_SEEKER3],
    dataContainer->program_icons[PROGRAM_SEEKER3]);
    gridProgramEditPanel->addObject(seeker3Button);
    GUIButton* slingshotButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SLINGSHOT);
    },
    dataContainer->program_icons[PROGRAM_SLINGSHOT],
    dataContainer->program_icons[PROGRAM_SLINGSHOT],
    dataContainer->program_icons[PROGRAM_SLINGSHOT]);
    gridProgramEditPanel->addObject(slingshotButton);
    GUIButton* sonarButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR);
    },
    dataContainer->program_icons[PROGRAM_SONAR],
    dataContainer->program_icons[PROGRAM_SONAR],
    dataContainer->program_icons[PROGRAM_SONAR]);
    gridProgramEditPanel->addObject(sonarButton);
    GUIButton* sonar2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR2);
    },
    dataContainer->program_icons[PROGRAM_SONAR2],
    dataContainer->program_icons[PROGRAM_SONAR2],
    dataContainer->program_icons[PROGRAM_SONAR2]);
    gridProgramEditPanel->addObject(sonar2Button);
    GUIButton* sonar3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR3);
    },
    dataContainer->program_icons[PROGRAM_SONAR3],
    dataContainer->program_icons[PROGRAM_SONAR3],
    dataContainer->program_icons[PROGRAM_SONAR3]);
    gridProgramEditPanel->addObject(sonar3Button);
    GUIButton* specsButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SPECS);
    },
    dataContainer->program_icons[PROGRAM_SPECS],
    dataContainer->program_icons[PROGRAM_SPECS],
    dataContainer->program_icons[PROGRAM_SPECS]);
    gridProgramEditPanel->addObject(specsButton);
    GUIButton* sumoButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SUMO);
    },
    dataContainer->program_icons[PROGRAM_SUMO],
    dataContainer->program_icons[PROGRAM_SUMO],
    dataContainer->program_icons[PROGRAM_SUMO]);
    gridProgramEditPanel->addObject(sumoButton);
    GUIButton* tarantulaButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TARANTULA);
    },
    dataContainer->program_icons[PROGRAM_TARANTULA],
    dataContainer->program_icons[PROGRAM_TARANTULA],
    dataContainer->program_icons[PROGRAM_TARANTULA]);
    gridProgramEditPanel->addObject(tarantulaButton);
    GUIButton* towerButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TOWER);
    },
    dataContainer->program_icons[PROGRAM_TOWER],
    dataContainer->program_icons[PROGRAM_TOWER],
    dataContainer->program_icons[PROGRAM_TOWER]);
    gridProgramEditPanel->addObject(towerButton);
    GUIButton* turboButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TURBO);
    },
    dataContainer->program_icons[PROGRAM_TURBO],
    dataContainer->program_icons[PROGRAM_TURBO],
    dataContainer->program_icons[PROGRAM_TURBO]);
    gridProgramEditPanel->addObject(turboButton);
    col = 0;
    ln++;
    GUIButton* turbo2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TURBO2);
    },
    dataContainer->program_icons[PROGRAM_TURBO2],
    dataContainer->program_icons[PROGRAM_TURBO2],
    dataContainer->program_icons[PROGRAM_TURBO2]);
    gridProgramEditPanel->addObject(turbo2Button);
    GUIButton* turbo3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TURBO3);
    },
    dataContainer->program_icons[PROGRAM_TURBO3],
    dataContainer->program_icons[PROGRAM_TURBO3],
    dataContainer->program_icons[PROGRAM_TURBO3]);
    gridProgramEditPanel->addObject(turbo3Button);
    GUIButton* walkerButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER);
    },
    dataContainer->program_icons[PROGRAM_WALKER],
    dataContainer->program_icons[PROGRAM_WALKER],
    dataContainer->program_icons[PROGRAM_WALKER]);
    gridProgramEditPanel->addObject(walkerButton);
    GUIButton* walker2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER2);
    },
    dataContainer->program_icons[PROGRAM_WALKER2],
    dataContainer->program_icons[PROGRAM_WALKER2],
    dataContainer->program_icons[PROGRAM_WALKER2]);
    gridProgramEditPanel->addObject(walker2Button);
    GUIButton* walker3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER3);
    },
    dataContainer->program_icons[PROGRAM_WALKER3],
    dataContainer->program_icons[PROGRAM_WALKER3],
    dataContainer->program_icons[PROGRAM_WALKER3]);
    gridProgramEditPanel->addObject(walker3Button);
    GUIButton* wardenButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN);
    },
    dataContainer->program_icons[PROGRAM_WARDEN],
    dataContainer->program_icons[PROGRAM_WARDEN],
    dataContainer->program_icons[PROGRAM_WARDEN]);
    gridProgramEditPanel->addObject(wardenButton);
    GUIButton* warden2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN2);
    },
    dataContainer->program_icons[PROGRAM_WARDEN2],
    dataContainer->program_icons[PROGRAM_WARDEN2],
    dataContainer->program_icons[PROGRAM_WARDEN2]);
    gridProgramEditPanel->addObject(warden2Button);
    GUIButton* warden3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN3);
    },
    dataContainer->program_icons[PROGRAM_WARDEN3],
    dataContainer->program_icons[PROGRAM_WARDEN3],
    dataContainer->program_icons[PROGRAM_WARDEN3]);
    gridProgramEditPanel->addObject(warden3Button);
    GUIButton* watchmanButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN],
    dataContainer->program_icons[PROGRAM_WATCHMAN],
    dataContainer->program_icons[PROGRAM_WATCHMAN]);
    gridProgramEditPanel->addObject(watchmanButton);
    GUIButton* watchman2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN2);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN2],
    dataContainer->program_icons[PROGRAM_WATCHMAN2],
    dataContainer->program_icons[PROGRAM_WATCHMAN2]);
    gridProgramEditPanel->addObject(watchman2Button);
    GUIButton* watchman3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN3);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN3],
    dataContainer->program_icons[PROGRAM_WATCHMAN3],
    dataContainer->program_icons[PROGRAM_WATCHMAN3]);
    gridProgramEditPanel->addObject(watchman3Button);
    GUIButton* wizardButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WIZARD);
    },
    dataContainer->program_icons[PROGRAM_WIZARD],
    dataContainer->program_icons[PROGRAM_WIZARD],
    dataContainer->program_icons[PROGRAM_WIZARD]);
    gridProgramEditPanel->addObject(wizardButton);
    GUIButton* wolfspiderButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WOLFSPIDER);
    },
    dataContainer->program_icons[PROGRAM_WOLFSPIDER],
    dataContainer->program_icons[PROGRAM_WOLFSPIDER],
    dataContainer->program_icons[PROGRAM_WOLFSPIDER]);
    gridProgramEditPanel->addObject(wolfspiderButton);

    ln = 0;
    col = 0;
    gridItemEditPanel = new GUIContainer(ANCHOR_NORTHWEST, {20, 200}, {4 + 36*4, 36}, this, NULL);
    GUIButton* creditButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridItemEditPanel,
                                            []()
    {
        gameScreen->setBrushItem(ITEM_CREDIT);
    },
    dataContainer->item_icons[ITEM_CREDIT],
    dataContainer->item_icons[ITEM_CREDIT],
    dataContainer->item_icons[ITEM_CREDIT]);
    GUIButton* bigCreditButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridItemEditPanel,
            []()
    {
        gameScreen->setBrushItem(ITEM_BIGCREDIT);
    },
    dataContainer->item_icons[ITEM_BIGCREDIT],
    dataContainer->item_icons[ITEM_BIGCREDIT],
    dataContainer->item_icons[ITEM_BIGCREDIT]);
    GUIButton* filesButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridItemEditPanel,
                                           []()
    {
        gameScreen->setBrushItem(ITEM_FILES);
    },
    dataContainer->item_icons[ITEM_FILES],
    dataContainer->item_icons[ITEM_FILES],
    dataContainer->item_icons[ITEM_FILES]);

    gridItemEditPanel->addObject(creditButton);
    gridItemEditPanel->addObject(bigCreditButton);
    gridItemEditPanel->addObject(filesButton);

    col = 0;
    gridSelectBrushPanel = new GUIContainer(ANCHOR_NORTHWEST, {20, 240}, {100, 200}, this, NULL);
    GUIButton* brushNoneButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {45, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_NONE);
    },
    dataContainer->game_editor_brush_none);
    GUIButton* brushTileButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {45, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_TILES);
    },
    dataContainer->game_editor_brush_tiles);
    GUIButton* brushProgramButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_PROGRAMS);
    },
    dataContainer->game_editor_brush_programs);
    GUIButton* brushClearProgramButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_DELETEPROGRAMS);
    },
    dataContainer->game_editor_brush_deletePrograms);
    GUIButton* brushItemButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_ITEMS);
    },
    dataContainer->game_editor_brush_items);
    col++;
    GUIButton* brushTeamPlayerButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushProgramTeam(0);
    },
    dataContainer->game_editor_toggle_teamPlayer);
    GUIButton* brushTeamComputerButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushProgramTeam(1);
    },
    dataContainer->game_editor_toggle_teamComputer);
    GUIButton* brushTeamViewButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->toggleViewTeams();
    },
    dataContainer->game_editor_toggle_teamView);
    GUIButton* brushClearGridButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {44, 12}, gridSelectBrushPanel,
            []()
    {
        gameScreen->clearGame();
    },
    dataContainer->game_editor_button_clearGrid);

    gridSelectBrushPanel->addObject(brushNoneButton);
    gridSelectBrushPanel->addObject(brushTileButton);
    gridSelectBrushPanel->addObject(brushProgramButton);
    gridSelectBrushPanel->addObject(brushClearProgramButton);
    gridSelectBrushPanel->addObject(brushItemButton);
    gridSelectBrushPanel->addObject(brushTeamPlayerButton);
    gridSelectBrushPanel->addObject(brushTeamComputerButton);
    gridSelectBrushPanel->addObject(brushTeamViewButton);
    gridSelectBrushPanel->addObject(brushClearGridButton);

    col = 0;
    gridBkgPanel = new GUIContainer(ANCHOR_NORTHWEST, {124, 240}, {100, 200}, this, NULL);
    GUIButton* bkgDonutButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel,
            []()
    {
        gameScreen->setBackgroundImg(BKG_DONUT);
    },
    dataContainer->game_editor_button_bkg[BKG_DONUT],
    dataContainer->game_editor_button_bkg[BKG_DONUT],
    dataContainer->game_editor_button_bkg[BKG_DONUT]);
    gridBkgPanel->addObject(bkgDonutButton);
    GUIButton* bkgPharmButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel,
            []()
    {
        gameScreen->setBackgroundImg(BKG_PHARM);
    },
    dataContainer->game_editor_button_bkg[BKG_PHARM],
    dataContainer->game_editor_button_bkg[BKG_PHARM],
    dataContainer->game_editor_button_bkg[BKG_PHARM]);
    gridBkgPanel->addObject(bkgPharmButton);
    GUIButton* bkgPedButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel,
                                            []()
    {
        gameScreen->setBackgroundImg(BKG_PED);
    },
    dataContainer->game_editor_button_bkg[BKG_PED],
    dataContainer->game_editor_button_bkg[BKG_PED],
    dataContainer->game_editor_button_bkg[BKG_PED]);
    gridBkgPanel->addObject(bkgPedButton);
    GUIButton* bkgMonkeyButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel,
            []()
    {
        gameScreen->setBackgroundImg(BKG_MONKEY);
    },
    dataContainer->game_editor_button_bkg[BKG_MONKEY],
    dataContainer->game_editor_button_bkg[BKG_MONKEY],
    dataContainer->game_editor_button_bkg[BKG_MONKEY]);
    gridBkgPanel->addObject(bkgMonkeyButton);
    GUIButton* bkgCellButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel,
            []()
    {
        gameScreen->setBackgroundImg(BKG_CELL);
    },
    dataContainer->game_editor_button_bkg[BKG_CELL],
    dataContainer->game_editor_button_bkg[BKG_CELL],
    dataContainer->game_editor_button_bkg[BKG_CELL]);
    gridBkgPanel->addObject(bkgCellButton);

    // turn display items
    playerTurn = new GUITexture(ANCHOR_NORTH, {-200, 50},
                                dataContainer->game_disp_playerturn,
                                {400, 100}, this);
    playerTurn->setTransparency(0);
    addObject(playerTurn);
    aiTurn = new GUITexture(ANCHOR_NORTH, {-200, 50},
                            dataContainer->game_disp_compturn,
                            {400, 100}, this);
    aiTurn->setTransparency(0);
    addObject(aiTurn);

    // debug options
    debugOptions = new GUIContainer(ANCHOR_NORTHWEST, {10, 10}, {220, 190}, this, {120, 120, 120, 140});
    GUIButton* teamViewButton = new GUIButton(ANCHOR_NORTHWEST, {10, 10}, {200, 50}, debugOptions,
                                   [](){gameScreen->toggleViewTeams();},
                                   dataContainer->game_button_viewTeams);
    debugOptions->addObject(teamViewButton);
    GUIButton* winGameButton = new GUIButton(ANCHOR_NORTHWEST, {10, 70}, {200, 50}, debugOptions,
                                  [](){gameScreen->setGameStatus(GAMESTATUS_WON);},
                                  dataContainer->game_button_win);
    debugOptions->addObject(winGameButton);
    GUIButton* stepButton = new GUIButton(ANCHOR_NORTHWEST, {10, 130}, {200, 50}, debugOptions,
                               [](){gameScreen->stepAI();},
                               dataContainer->game_button_aiStep);
    debugOptions->addObject(stepButton);
    debugOptions->setTransparency(0);
    addObject(debugOptions);

    // pre-game options
    preGameOptions = new GUIContainer(ANCHOR_SOUTHWEST, {10, -150}, {220, 130}, this, {120, 120, 120, 140});
    GUIButton* startGameButton = new GUIButton(ANCHOR_NORTHWEST, {10, 10}, {200, 50}, preGameOptions,
                                    [](){gameScreen->setGameStatus(GAMESTATUS_PLAYING);},
                                    dataContainer->game_button_start);
    preGameOptions->addObject(startGameButton);
    GUIButton* backToMapButton = new GUIButton(ANCHOR_NORTHWEST, {10, 70}, {200, 50}, preGameOptions,
                                    [](){gameScreen->setGameStatus(GAMESTATUS_LOST);},
                                    dataContainer->game_button_quitToMap);
    preGameOptions->addObject(backToMapButton);
    addObject(preGameOptions);

    // pause menu
    pauseMenu = new GUIContainer(ANCHOR_CENTER, {-110, -200}, {220, 5*60 + 10}, this, {120, 120, 120, 140});
    GUIButton* resumeGameButton = new GUIButton(ANCHOR_NORTHWEST, {10, 10}, {200, 50}, pauseMenu,
                                                [](){gameScreen->togglePauseMenu();},
                                                dataContainer->game_button_resume);
    pauseMenu->addObject(resumeGameButton);
    GUIButton* resetGameButton = new GUIButton(ANCHOR_NORTHWEST, {10, 70}, {200, 50}, pauseMenu,
                                    [](){gameScreen->loadGame(mapScreen->getSelectedNode()->getLevelId());},
                                    dataContainer->game_button_reset);
    pauseMenu->addObject(resetGameButton);
    GUIButton* exitToMapButton = new GUIButton(ANCHOR_NORTHWEST, {10, 130}, {200, 50}, pauseMenu,
                                      [](){gameScreen->setGameStatus(GAMESTATUS_LOST);},
                                      dataContainer->game_button_quitToMap);
    pauseMenu->addObject(exitToMapButton);
    GUIButton* exitToMainButton = new GUIButton(ANCHOR_NORTHWEST, {10, 190}, {200, 50}, pauseMenu,
                                                [](){gameScreen->setGameStatus(GAMESTATUS_LOST);
                                                     currScreen = mainScreen;},
                                                dataContainer->game_button_quitToMain);
    pauseMenu->addObject(exitToMainButton);
    GUIButton* exitToDesktopButton = new GUIButton(ANCHOR_NORTHWEST, {10, 250}, {200, 50}, pauseMenu,
                                                   [](){quit = true;},
                                                   dataContainer->game_button_quitToDesktop);
    pauseMenu->addObject(exitToDesktopButton);
    pauseMenu->setTransparency(0);
    pauseMenu->setMovable(false);
    addObject(pauseMenu);

    // end turn button
    turnButton = new GUIButton(ANCHOR_SOUTHEAST, {-220, -60}, {200, 50}, this,
                               [](){gameScreen->endTurn();},
                               dataContainer->game_button_endTurn);
    turnButton->setTransparency(0);
    addObject(turnButton);

    // program display window
    progDisp = new ProgramDisplayContainer(ANCHOR_SOUTHWEST, {10, -210}, {200, 200}, this);
    progDisp->setTransparency(0);
    addObject(progDisp);

    // add the program inventory display
    progInv = new ProgramInventoryDisplay(ANCHOR_NORTHEAST, {0, 0}, {0, 0}, this);
    addObject(progInv);
}

void GameScreen::resetBounds()
{
    GUIContainer::resetBounds();
    checkShiftable();
    if (!canShiftScreen) centerScreen();
}

bool GameScreen::mouseDown()
{
    if (pauseMenu->isVisible())
        return pauseMenu->mouseDown();

    if (GUIContainer::mouseDown()) return true;

    if (editorMode)
    {
        if (gridEditPanel->isMouseOver()) // click the tile pallette
        {
            return gridEditPanel->mouseDown();
        }
        else if (gridProgramEditPanel->isMouseOver()) // click the program pallette
        {
            return gridProgramEditPanel->mouseDown();
        }
        else if (gridSelectBrushPanel->isMouseOver())
        {
            return gridSelectBrushPanel->mouseDown();
        }
        else if (gridItemEditPanel->isMouseOver())
        {
            return gridItemEditPanel->mouseDown();
        }
        else if (gridBkgPanel->isMouseOver())
        {
            return gridBkgPanel->mouseDown();
        }
        else // click the grid
        {
            // find the clicked tile
            Coord click;
            click.x = (bkgPos.x + mousePos.x)/32;
            click.y = (bkgPos.y + mousePos.y)/32;

            if (brushMode == BRUSH_PROGRAMS)
            {
                // TODO
            }
            else if (brushMode == BRUSH_TILES)
            {
                game->setTileAt(click, brushTileType);
            }
            else if (brushMode == BRUSH_DELETEPROGRAMS)
            {
                game->setProgramAt(click, NULL);
            }
            else if (brushMode == BRUSH_ITEMS)
            {
                game->setItemAt(click, brushItemType);
            }
            else
            {
                return false;
            }
            return true;
        }
    }
    else // if not in editor mode
    {
        Coord click = {(bkgPos.x + mousePos.x)/32, (bkgPos.y + mousePos.y)/32};

        if (gameStatus == GAMESTATUS_PLACING_PROGRAMS)
        {
            game->getHumanPlayer()->setSelectedTile(click);
        }
        else if (gameStatus == GAMESTATUS_PLAYING)
        {
            if ((game->getHumanPlayer()->getSelectedProgram() == NULL || !game->getHumanPlayer()->canSelectedProgramMoveTo(click)) &&
                (game->getHumanPlayer()->getSelectedAction() == NULL || !game->getHumanPlayer()->getSelectedActionDist(click) > 0))
                game->getHumanPlayer()->setSelectedTile(click);
            else //if (game->getPlayer()->canSelectedProgramMoveTo(x, y))
            {
                if (game->getHumanPlayer()->getSelectedAction() == NULL)
                {
                    game->getHumanPlayer()->moveSelectedProgram(click);
                    if (game->getHumanPlayer()->getSelectedProgram()->getMoves() == 0)
                    {
                        game->getHumanPlayer()->setSelectedAction(game->getHumanPlayer()->getSelectedProgram()->getActions()->getFirst());
                    }
                }
                else if (game->getHumanPlayer()->getSelectedActionDist(click) > 0)
                {
                    if (debug >= DEBUG_NORMAL) printf("player using move\n");
                    game->getHumanPlayer()->useSelectedActionAt(click);
                    game->getHumanPlayer()->setSelectedProgram(NULL);
                }
            }
        }

        return false;
    }
}

bool GameScreen::mouseUp()
{
    if (pauseMenu->isVisible())
        return pauseMenu->mouseUp();

    GUIContainer::mouseUp();

    bool r = false;

    if (editorMode)
    {
        if (gridEditPanel->isMouseOver()) // click the tile pallette
        {
            r = gridEditPanel->mouseUp();
        }
        else if (gridProgramEditPanel->isMouseOver()) // click the program pallette
        {
            r = gridProgramEditPanel->mouseUp();
        }
        else if (gridSelectBrushPanel->isMouseOver())
        {
            r = gridSelectBrushPanel->mouseUp();
        }
        else if (gridItemEditPanel->isMouseOver())
        {
            r = gridItemEditPanel->mouseUp();
        }
        else if (gridBkgPanel->isMouseOver())
        {
            r = gridBkgPanel->mouseUp();
        }

        gridEditPanel->setPressed(false);
        gridProgramEditPanel->setPressed(false);
        gridSelectBrushPanel->setPressed(false);
        gridItemEditPanel->setPressed(false);
        gridBkgPanel->setPressed(false);
    }

    return r;
}

void GameScreen::endTurn()
{
    game->getHumanPlayer()->endTurn();
    aiTurn->fade(255, 500);
    tickingAI = true;
}

void GameScreen::stepAI()
{
    bool done = true;
    for (int i = 0; i < game->getAIPlayers()->getLength(); i++)
    {
        AICore* core = game->getAIPlayers()->getObjectAt(i);
        if (!core->getDoneTurn())
        {
            currPlayer = core;
            core->act(1);
            done = false;
            break;
        }
    }

    if (done)
    {
        game->getAIPlayers()->forEach([](AICore* a){a->endTurn();});
        currPlayer = game->getHumanPlayer();
    }
}

void GameScreen::drawBkg()
{
    // draw background image
    SDL_RenderCopy(gRenderer, bkgImg, NULL, NULL);
}

void GameScreen::drawGrid()
{
    SDL_Rect tileRect;
    // set temp variables
    Coord topLeft = {bkgPos.x / 32, bkgPos.y / 32};
    Coord bottomRight;
    bottomRight.x = topLeft.x + SCREEN_WIDTH / 32 + 1;
    bottomRight.y = topLeft.y + SCREEN_HEIGHT / 32 + 1;

    // check for overflows
    if (bottomRight.x >= 200)
        bottomRight.x = 200;
    if (bottomRight.y >= 200)
        bottomRight.y = 200;
    if (topLeft.x < 0)
        topLeft.x = 0;
    if (topLeft.y < 0)
        topLeft.y = 0;

    // draw grid
    for (int x = topLeft.x; x < bottomRight.x; x++)
    {
        for (int y = topLeft.y; y < bottomRight.y; y++)
        {
            Coord curr = {x, y};

            // if there's no tile to be drawn here
            if (game->getTileAt(curr) == TILE_NONE && !editorMode)
                continue;

            // default position of a tile,
            // assuming it is 28x28 px
            int xDefault = 4 + 32*curr.x - bkgPos.x;
            int yDefault = 4 + 32*curr.y - bkgPos.y;
            int sizeDefault = 28;
            tileRect.x = xDefault;
            tileRect.y = yDefault;
            tileRect.w = sizeDefault;
            tileRect.h = sizeDefault;

            // if there's a program at this tile
            if (game->getProgramAt(curr) != NULL)
            {
                // get this program
                Program* prog = game->getProgramAt(curr);

                // draw this program's tile
                tileRect.x = xDefault - 1;
                tileRect.y = yDefault - 1;
                tileRect.w = sizeDefault + 2;
                tileRect.h = sizeDefault + 2;
                if (programViewTeams)
                {
                    if (prog->getTeam() == 0)
                        SDL_SetTextureColorMod(dataContainer->program_core, 0, 0, prog->getColor(2));
                    else if (prog->getTeam() == 1)
                        SDL_SetTextureColorMod(dataContainer->program_core, prog->getColor(0), 0, 0);
                    else if (prog->getTeam() == 2)
                        SDL_SetTextureColorMod(dataContainer->program_core, 0, prog->getColor(1), 0);
                    else if (prog->getTeam() == 3)
                        SDL_SetTextureColorMod(dataContainer->program_core, prog->getColor(0), prog->getColor(1), 0);
                    else
                        SDL_SetTextureColorMod(dataContainer->program_core, 0, prog->getColor(1), prog->getColor(2));
                }
                else
                    SDL_SetTextureColorMod(dataContainer->program_core, prog->getColor(0), prog->getColor(1), prog->getColor(2));

                // if this is the farthest chunk of this program
                if (prog == currPlayer->getSelectedProgram() &&
                    prog->getHealth() == prog->getMaxHealth() &&
                    prog->getMoves() > 0)
                {
                    Coord currTail = prog->getTail();
                    if (curr.x == currTail.x && curr.y == currTail.y)
                    {
                        if (textureTickCount % 100 < 50)
                            SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);
                    }
                    else
                        SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);
                }
                else SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);

                // if this is part of the selected program, indicate it
                if (prog == currPlayer->getSelectedProgram() && debug >= DEBUG_NORMAL)
                {
                    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
                    SDL_RenderDrawLine(gRenderer, tileRect.x, tileRect.y, tileRect.x + tileRect.w, tileRect.y + tileRect.h);
                    SDL_RenderDrawLine(gRenderer, tileRect.x + tileRect.w, tileRect.y, tileRect.x, tileRect.y + tileRect.h);
                }

                // draw the bridges from this program's tile to adjacent tiles
                if (game->getProgramAt({curr.x, curr.y+1}) == prog)
                {
                    tileRect.x = xDefault + 9;
                    tileRect.y = yDefault + 26;
                    tileRect.w = 10;
                    tileRect.h = 5;
                    if (programViewTeams)
                    {
                        if (prog->getTeam() == 0)
                            SDL_SetTextureColorMod(dataContainer->program_core_vertical, 0, 0, prog->getColor(2));
                        else if (prog->getTeam() == 1)
                            SDL_SetTextureColorMod(dataContainer->program_core_vertical, prog->getColor(0), 0, 0);
                        else if (prog->getTeam() == 2)
                            SDL_SetTextureColorMod(dataContainer->program_core_vertical, 0, prog->getColor(1), 0);
                        else if (prog->getTeam() == 3)
                            SDL_SetTextureColorMod(dataContainer->program_core_vertical, prog->getColor(0), prog->getColor(1), 0);
                        else
                            SDL_SetTextureColorMod(dataContainer->program_core_vertical, 0, prog->getColor(1), prog->getColor(2));
                    }
                    else
                        SDL_SetTextureColorMod(dataContainer->program_core_vertical, prog->getColor(0), prog->getColor(1), prog->getColor(2));
                    SDL_RenderCopy(gRenderer, dataContainer->program_core_vertical, NULL, &tileRect);
                }
                if (game->getProgramAt({curr.x+1, curr.y}) == prog)
                {
                    tileRect.x = xDefault + 26;
                    tileRect.y = yDefault + 8;
                    tileRect.w = 5;
                    tileRect.h = 12;
                    if (programViewTeams)
                    {
                        if (prog->getTeam() == 0)
                            SDL_SetTextureColorMod(dataContainer->program_core_horizontal, 0, 0, prog->getColor(2));
                        else if (prog->getTeam() == 1)
                            SDL_SetTextureColorMod(dataContainer->program_core_horizontal, prog->getColor(0), 0, 0);
                        else if (prog->getTeam() == 2)
                            SDL_SetTextureColorMod(dataContainer->program_core_horizontal, 0, prog->getColor(1), 0);
                        else if (prog->getTeam() == 3)
                            SDL_SetTextureColorMod(dataContainer->program_core_horizontal, prog->getColor(0), prog->getColor(1), 0);
                        else
                            SDL_SetTextureColorMod(dataContainer->program_core_horizontal, 0, prog->getColor(1), prog->getColor(2));
                    }
                    else
                        SDL_SetTextureColorMod(dataContainer->program_core_horizontal, prog->getColor(0), prog->getColor(1), prog->getColor(2));
                    SDL_RenderCopy(gRenderer, dataContainer->program_core_horizontal, NULL, &tileRect);
                }

                // draw the icon IF this is the core tile
                if (prog->getCore().x == curr.x && prog->getCore().y == curr.y)
                {
                    tileRect.x = xDefault - 1;
                    tileRect.y = yDefault - 1;
                    tileRect.w = 27;
                    tileRect.h = 27;
                    SDL_RenderCopy(gRenderer, prog->getIcon(), NULL, &tileRect);

                    // draw the highlight rectangle if this program is selected
                    if (prog == currPlayer->getSelectedProgram())
                    {
                        tileRect.x = xDefault - 2;
                        tileRect.y = yDefault - 2;
                        tileRect.w = 32;
                        tileRect.h = 32;
                        SDL_SetTextureAlphaMod(dataContainer->tile_selected, ((double)-textureTickCount/1000.0)*255 + 255);
                        SDL_RenderCopy(gRenderer, dataContainer->tile_selected, NULL, &tileRect);
                    }

                    // draw the checkmark if this program is done for the turn
                    if (prog->isDone())
                    {
                        tileRect.x = xDefault + 17;
                        tileRect.y = yDefault;
                        tileRect.w = 10;
                        tileRect.h = 10;
                        SDL_RenderCopy(gRenderer, dataContainer->game_icon_checkmark, NULL, &tileRect);
                    }
                }
            }
            else // if there is no program on this tile
            {
                SDL_Texture* tileImg = dataContainer->tile_images[game->getTileAt(curr)];
                if (game->getTileAt(curr) == TILE_NONE && !editorMode)
                    continue;

                SDL_QueryTexture(tileImg, NULL, NULL, &tileRect.w, &tileRect.h);
                tileRect.x = xDefault - (tileRect.w - 28)/2;
                tileRect.y = yDefault - (tileRect.h - 28)/2;
                SDL_RenderCopy(gRenderer, tileImg, NULL, &tileRect);
            }

            // if the mouse is over this tile
            if (mousePos.x - tileRect.x > 0 &&
                mousePos.x - (tileRect.x + tileRect.w) < 0 &&
                mousePos.y - tileRect.y > 0 &&
                mousePos.y - (tileRect.y + tileRect.h) < 0 &&
                !pauseMenu->isVisible())
            {
                tileRect.x = xDefault;
                tileRect.y = yDefault;
                tileRect.w = sizeDefault;
                tileRect.h = sizeDefault;
                SDL_SetTextureAlphaMod(dataContainer->tile_over, 125);
                SDL_RenderCopy(gRenderer, dataContainer->tile_over, NULL, &tileRect);
            }

            // if there is an item on this tile
            if (game->getItemAt(curr) != ITEM_NONE)
            {
                SDL_QueryTexture(dataContainer->item_icons[game->getItemAt(curr)], NULL, NULL, &tileRect.w, &tileRect.h);
                tileRect.x = xDefault - (tileRect.w - 28)/2;
                tileRect.y = yDefault - (tileRect.h - 28)/2;
                SDL_RenderCopy(gRenderer, dataContainer->item_icons[game->getItemAt(curr)], NULL, &tileRect);
            }

            // if this is the selected tile
            if (curr == currPlayer->getSelectedTile())
            {
                tileRect.x = xDefault - 2;
                tileRect.y = yDefault - 2;
                tileRect.w = 32;
                tileRect.h = 32;
                SDL_SetTextureAlphaMod(dataContainer->tile_selected, ((double)-textureTickCount/1000.0)*255 + 255);
                SDL_RenderCopy(gRenderer, dataContainer->tile_selected, NULL, &tileRect);
            }

            // if this tile is movable-to
            if (currPlayer->getSelectedProgramDist(curr) > 0 && currPlayer->getSelectedProgram()->getOwner() == currPlayer)
            {
                tileRect.x = xDefault;
                tileRect.y = yDefault;
                tileRect.w = sizeDefault;
                tileRect.h = sizeDefault;

                if (curr == currPlayer->getSelectedProgram()->getCore() + Coord{0, 1})
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveSouth, NULL, &tileRect);
                else if (curr == currPlayer->getSelectedProgram()->getCore() + Coord{1, 0})
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveEast, NULL, &tileRect);
                else if (curr == currPlayer->getSelectedProgram()->getCore() + Coord{0, -1})
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveNorth, NULL, &tileRect);
                else if (curr == currPlayer->getSelectedProgram()->getCore() + Coord{-1, 0})
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveWest, NULL, &tileRect);
                else
                    SDL_RenderCopy(gRenderer, dataContainer->tile_movePossible, NULL, &tileRect);
            }

            // if this tile is in the range of a programAction
            if (currPlayer->getSelectedActionDist(curr) > 0 && game->getProgramAt(curr) != currPlayer->getSelectedProgram())
            {
                tileRect.x = xDefault - 2;
                tileRect.y = yDefault - 2;
                tileRect.w = sizeDefault + 4;
                tileRect.h = sizeDefault + 4;

                switch (currPlayer->getSelectedAction()->type)
                {
                case MOVETYPE_DAMAGE:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionDamage, NULL, &tileRect);
                    break;
                case MOVETYPE_HEAL:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case MOVETYPE_MAXHEALTHDOWN:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case MOVETYPE_MAXHEALTHUP:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case MOVETYPE_SPEEDDOWN:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionSpeed, NULL, &tileRect);
                    break;
                case MOVETYPE_SPEEDUP:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case MOVETYPE_TILEDELETE:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case MOVETYPE_TILEPLACE:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                default:
                    break;
                }
            }
        }
    }

    // draw board bounding rectangle
    if (debug >= DEBUG_NORMAL)
    {
        tileRect.x = -bkgPos.x + 4;
        tileRect.y = -bkgPos.y + 4;
        tileRect.w = 32 * 200 - 4;
        tileRect.h = 32 * 200 - 4;
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 0);
        SDL_RenderDrawRect(gRenderer, &tileRect);

        SDL_RenderDrawLine(gRenderer, -bkgPos.x + 100*32, -bkgPos.y + 4, -bkgPos.x + 100*32, -bkgPos.y + 200*32); // vert
        SDL_RenderDrawLine(gRenderer, -bkgPos.x + 4, -bkgPos.y + 100*32, -bkgPos.x + 200*32, -bkgPos.y + 100*32); // horiz

        tileRect.x = -bkgPos.x + game->getLeftBound()*32;
        tileRect.y = -bkgPos.y + game->getTopBound()*32;
        tileRect.w = (game->getRightBound() - game->getLeftBound())*32;
        tileRect.h = (game->getBottomBound() - game->getTopBound())*32;
        SDL_RenderDrawRect(gRenderer, &tileRect);
    }
}

void GameScreen::draw()
{
    drawBkg();
    drawGrid();
    GUIContainer::drawContents();

    if (editorMode)
    {
        gridEditPanel->draw();
        gridProgramEditPanel->draw();
        gridSelectBrushPanel->draw();
        gridItemEditPanel->draw();
        gridBkgPanel->draw();
    }

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

void GameScreen::shiftBkg(Coord disp)
{
    if (!canShiftScreen) return;

    if (bkgPos.x + disp.x + SCREEN_WIDTH/2 < game->getLeftBound()*32)
        bkgPos.x = game->getLeftBound()*32 - SCREEN_WIDTH/2;
    else if (bkgPos.x + disp.x + SCREEN_WIDTH/2 > game->getRightBound()*32)
        bkgPos.x = game->getRightBound()*32 - SCREEN_WIDTH/2;
    else
        bkgPos.x += disp.x;

    if (bkgPos.y + disp.y + SCREEN_HEIGHT/2 < game->getTopBound()*32)
        bkgPos.y = game->getTopBound()*32 - SCREEN_HEIGHT/2;
    else if (bkgPos.y + disp.y + SCREEN_HEIGHT/2 > game->getBottomBound()*32)
        bkgPos.y = game->getBottomBound()*32 - SCREEN_HEIGHT/2;
    else
        bkgPos.y += disp.y;
}

void GameScreen::setBrushTileType(TILE t)
{
    brushTileType = t;
}

void GameScreen::setBrushProgramType(PROGRAM p)
{
    brushProgramType = p;
    game->getHumanPlayer()->setSelectedProgram(NULL);
}

void GameScreen::setBrushMode(BRUSH b)
{
    brushMode = b;
}

void GameScreen::setBrushProgramTeam(int t)
{
    game->getHumanPlayer()->setSelectedProgram(NULL);
    brushProgramTeam = t;
}

void GameScreen::toggleViewTeams()
{
    programViewTeams = !programViewTeams;
}

void GameScreen::setBrushItem(ITEM i)
{
    brushItemType = i;
}

void GameScreen::setBackgroundImg(BACKGROUND b)
{
    bkgImg = dataContainer->game_backgrounds[b];
    game->setBackground(b);
}

void GameScreen::checkShiftable()
{
    if ((game->getRightBound() - game->getLeftBound())*32 < SCREEN_WIDTH - 200 && (game->getBottomBound() - game->getTopBound())*32 < SCREEN_HEIGHT - 200)
        canShiftScreen = false;
    else
        canShiftScreen = true;
}

Program* GameScreen::getSelectedProgram()
{
    return currPlayer->getSelectedProgram();
}

void GameScreen::tick(int ms)
{
    // tick gui objects
    GUIContainer::tick(ms);

    // don't do anything if pause is visible
    if (pauseMenu->isVisible())
        return;

    // test to activate/deactivate the debug options
    if (debug >= DEBUG_MINIMAL)
        debugOptions->setTransparency(255);
    else
        debugOptions->setTransparency(0);

    // adjust time-dependent textures
    textureTickCount += ms;
    if (textureTickCount >= 1000) textureTickCount = 0;

    // fade out the playerTurn display if it is visible
    if (playerTurn->isVisible() && playerTurn->getTransparency() == 255)
    {
        playerTurn->fade(0, 500);
    }

    // if currently doing the AI's moves
    if (tickingAI && gameStatus == GAMESTATUS_PLAYING)
    {
        turnTickCount += ms;
        if (aiTurn->isVisible() && aiTurn->getTransparency() != 255)
        {
            // do nothing
        }
        else if (aiTurn->isVisible() && aiTurn->getTransparency() == 255)
        {
            aiTurn->fade(0, 500);
        }
        else if (turnTickCount >= 200)
        {
            turnTickCount = 0;

            bool done = true;
            for (int i = 0; i < game->getAIPlayers()->getLength(); i++)
            {
                AICore* core = game->getAIPlayers()->getObjectAt(i);
                if (!core->getDoneTurn())
                {
                    currPlayer = core;
                    core->act(1);
                    done = false;
                    break;
                }
            }

            if (done)
            {
                game->getAIPlayers()->forEach([](AICore* a){a->endTurn();});
                tickingAI = false;
                playerTurn->fade(255, 500);
                currPlayer = game->getHumanPlayer();
            }
        }
    }

    // check if the current music track is done, if so, pick a new one
    if (Mix_PlayingMusic() == 0)
    {
        int rand_1 = rand()%4;
        int rand_2 = rand()%2;
        switch (rand_1)
        {
        case 0:
            Mix_PlayMusic(dataContainer->music_game1, rand_2);
            break;
        case 1:
            Mix_PlayMusic(dataContainer->music_game2, rand_2);
            break;
        case 2:
            Mix_PlayMusic(dataContainer->music_game3, rand_2);
            break;
        case 3:
            Mix_PlayMusic(dataContainer->music_game4, rand_2);
            break;
        }
    }

    // scan for keys currently pressed
    const Uint8* currentKeyStates = SDL_GetKeyboardState( NULL );
    int shiftAmt = shiftSpeed*ms;
    if( currentKeyStates[ SDL_SCANCODE_UP ] )
        shiftBkg({0, -shiftAmt});
    else if( currentKeyStates[ SDL_SCANCODE_DOWN ] )
        shiftBkg({0, shiftAmt});

    if( currentKeyStates[ SDL_SCANCODE_LEFT ] )
        shiftBkg({-shiftAmt, 0});
    else if( currentKeyStates[ SDL_SCANCODE_RIGHT ] )
        shiftBkg({shiftAmt, 0});

    // if the mouse is at an edge, try to shift the background
    if (mousePos.x < 20)
        shiftBkg({-shiftAmt, 0});
    else if (mousePos.x > SCREEN_WIDTH - 20)
        shiftBkg({shiftAmt, 0});

    if (mousePos.y < 20)
        shiftBkg({0, -shiftAmt});
    else if (mousePos.y > SCREEN_HEIGHT - 20)
        shiftBkg({0, shiftAmt});
}

void GameScreen::saveGame()
{
    game->saveLevel();
}

void GameScreen::loadGame(int id)
{
    delete game;
    game = new Game("levels/classic/" + to_string(id) + ".urf");
    currPlayer = game->getHumanPlayer();
    setBackgroundImg(game->getBackground());
    setGameStatus(GAMESTATUS_PLACING_PROGRAMS);
    checkShiftable();
    centerScreen();
}

void GameScreen::clearGame()
{
    delete game;
    game = new Game();
    currPlayer = NULL;
    setGameStatus(GAMESTATUS_NO_GAME);
    canShiftScreen = false;
    centerScreen();
}

void GameScreen::centerScreen()
{
    bkgPos.x = (game->getRightBound() + game->getLeftBound())*32/2 - SCREEN_WIDTH/2;
    bkgPos.y = (game->getBottomBound() + game->getTopBound())*32/2 - SCREEN_HEIGHT/2;
}

void GameScreen::setGameStatus(GAMESTATUS g)
{
    switch (g)
    {
    case GAMESTATUS_NO_GAME:
        break;
    case GAMESTATUS_PLACING_PROGRAMS:
        for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
            usedPrograms[i] = 0;

        preGameOptions->setTransparency(255);
        progDisp->setTransparency(0);
        turnButton->setTransparency(0);

        progInv->setTransparency(255);
        progInv->updateContents();
        break;
    case GAMESTATUS_PLAYING:
        preGameOptions->setTransparency(0);
        progDisp->setTransparency(255);
        turnButton->setTransparency(255);

        progInv->setTransparency(0);

        for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++)
            if (game->getTileAt({x, y}) == TILE_SPAWN || game->getTileAt({x, y}) == TILE_SPAWN2)
                game->setTileAt({x, y}, TILE_PLAIN);

        game->getHumanPlayer()->setSelectedTile({-1, -1});
        playerTurn->fade(255, 500);
        break;
    case GAMESTATUS_WON:
        for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
            progListCurrent[i] += usedPrograms[i];

        mapScreen->getSelectedNode()->winNode();
        mapScreen->clearSelectedNode();
        currScreen = mapScreen;
        break;
    case GAMESTATUS_LOST:
        for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
            progListCurrent[i] += usedPrograms[i];

        pauseMenu->setTransparency(0);
        mapScreen->clearSelectedNode();
        currScreen = mapScreen;
        break;
    }

    gameStatus = g;
}

void GameScreen::tryPlacingProgram(PROGRAM p)
{
    Coord selectedTile = game->getHumanPlayer()->getSelectedTile();

    // check for the correct game state
    if (gameStatus != GAMESTATUS_PLACING_PROGRAMS)
        return;

    // check for existence of a game
    if (game == NULL)
        return;

    // check for a valid value of p
    if (p == PROGRAM_NONE || p == PROGRAM_CUSTOM || p == PROGRAM_NUM_PROGTYPES)
        return;

    // check for a valid spawn tile in this location
    if (game->getTileAt(selectedTile) != TILE_SPAWN && game->getTileAt(selectedTile) != TILE_SPAWN2)
        return;

    // remove any program that already exists here
    Program* prog = game->getProgramAt(selectedTile);
    if (prog != NULL)
    {
        usedPrograms[prog->getType()]--;
        progListCurrent[prog->getType()]++;
        game->setProgramAt(selectedTile, NULL);
    }

    // place the new program
    Program* pr = new Program(p, 0, selectedTile);
    game->getHumanPlayer()->addProgram(pr);
    game->setProgramAt(selectedTile, pr);
    printf("program placed\n");

    usedPrograms[p]++;
    progListCurrent[p]--;
    progInv->updateContents();
}

void GameScreen::toggleEditorMode()
{
    editorMode = !editorMode;
}

void GameScreen::togglePauseMenu()
{
    if (pauseMenu->isVisible()) pauseMenu->setTransparency(0);
    else pauseMenu->setTransparency(255);
}

void GameScreen::triggerDamageAt(Coord c)
{
    // TODO
}

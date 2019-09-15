#include "GameScreen.h"
#include "GUIContainer.h"
#include "Global.h"
#include "Program.h"
#include "GUIButton.h"
#include "LinkedList.h"
#include "ResourceLoader.h"
#include "Pair.h"
#include "GUITexture.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>

GameScreen::GameScreen()
    : GUIContainer(ANCHOR_NORTHWEST, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, loadTexture("resources/company_4.png"))
{
    buildGUI();

    brushMode = BRUSH_NONE;
    brushTileType = TILE_NONE;
    brushProgramType = PROGRAM_BALLISTA;
    brushProgramTeam = 0;
    brushItemType = ITEM_NONE;
    programViewTeams = false;
    tickCount = 0;
    bkgX = 0;
    bkgY = 0;
    selectedTileX = 0;
    selectedTileY = 0;
    shiftSpeed = 0.1;
    canShiftScreen = true;

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
    gridEditPanel = new GUIContainer(ANCHOR_NORTHWEST, 20, 20, 8 + 32*12, 36, this, NULL);
    GUIButton* emptyButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_NONE);
    },
    dataContainer->tile_images[TILE_NONE]);
    GUIButton* plainButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN);
    },
    dataContainer->tile_images[TILE_PLAIN]);
    GUIButton* plain2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN2);
    },
    dataContainer->tile_images[TILE_PLAIN2]);
    GUIButton* plain3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN3);
    },
    dataContainer->tile_images[TILE_PLAIN3],
    dataContainer->tile_images[TILE_PLAIN3],
    dataContainer->tile_images[TILE_PLAIN3]);
    GUIButton* plain4Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN4);
    },
    dataContainer->tile_images[TILE_PLAIN4],
    dataContainer->tile_images[TILE_PLAIN4],
    dataContainer->tile_images[TILE_PLAIN4]);
    GUIButton* plain5Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN5);
    },
    dataContainer->tile_images[TILE_PLAIN5],
    dataContainer->tile_images[TILE_PLAIN5],
    dataContainer->tile_images[TILE_PLAIN5]);
    GUIButton* plain6Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN6);
    },
    dataContainer->tile_images[TILE_PLAIN6],
    dataContainer->tile_images[TILE_PLAIN6],
    dataContainer->tile_images[TILE_PLAIN6]);
    GUIButton* plain7Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN7);
    },
    dataContainer->tile_images[TILE_PLAIN7],
    dataContainer->tile_images[TILE_PLAIN7],
    dataContainer->tile_images[TILE_PLAIN7]);
    GUIButton* plain8Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN8);
    },
    dataContainer->tile_images[TILE_PLAIN9],
    dataContainer->tile_images[TILE_PLAIN9],
    dataContainer->tile_images[TILE_PLAIN9]);
    GUIButton* plain9Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN9);
    },
    dataContainer->tile_images[TILE_PLAIN9],
    dataContainer->tile_images[TILE_PLAIN9],
    dataContainer->tile_images[TILE_PLAIN9]);
    GUIButton* spawnButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_SPAWN);
    },
    dataContainer->tile_images[TILE_SPAWN],
    dataContainer->tile_images[TILE_SPAWN],
    dataContainer->tile_images[TILE_SPAWN]);
    GUIButton* spawn2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_SPAWN2);
    },
    dataContainer->tile_images[TILE_SPAWN2],
    dataContainer->tile_images[TILE_SPAWN2],
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
    gridProgramEditPanel = new GUIContainer(ANCHOR_NORTHWEST, 20, 60, 32*15 + 8, 136, this, NULL);
    GUIButton* ballistaButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BALLISTA);
    },
    dataContainer->program_icons[PROGRAM_BALLISTA],
    dataContainer->program_icons[PROGRAM_BALLISTA],
    dataContainer->program_icons[PROGRAM_BALLISTA]);
    gridProgramEditPanel->addObject(ballistaButton);
    GUIButton* bitmanButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BITMAN);
    },
    dataContainer->program_icons[PROGRAM_BITMAN],
    dataContainer->program_icons[PROGRAM_BITMAN],
    dataContainer->program_icons[PROGRAM_BITMAN]);
    gridProgramEditPanel->addObject(bitmanButton);
    GUIButton* bitman2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BITMAN2);
    },
    dataContainer->program_icons[PROGRAM_BITMAN2],
    dataContainer->program_icons[PROGRAM_BITMAN2],
    dataContainer->program_icons[PROGRAM_BITMAN2]);
    gridProgramEditPanel->addObject(bitman2Button);
    GUIButton* blackwidowButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BLACKWIDOW);
    },
    dataContainer->program_icons[PROGRAM_BLACKWIDOW],
    dataContainer->program_icons[PROGRAM_BLACKWIDOW],
    dataContainer->program_icons[PROGRAM_BLACKWIDOW]);
    gridProgramEditPanel->addObject(blackwidowButton);
    GUIButton* bossButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BOSS);
    },
    dataContainer->program_icons[PROGRAM_BOSS],
    dataContainer->program_icons[PROGRAM_BOSS],
    dataContainer->program_icons[PROGRAM_BOSS]);
    gridProgramEditPanel->addObject(bossButton);
    GUIButton* bugButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                         []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG);
    },
    dataContainer->program_icons[PROGRAM_BUG],
    dataContainer->program_icons[PROGRAM_BUG],
    dataContainer->program_icons[PROGRAM_BUG]);
    gridProgramEditPanel->addObject(bugButton);
    GUIButton* bug2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG2);
    },
    dataContainer->program_icons[PROGRAM_BUG2],
    dataContainer->program_icons[PROGRAM_BUG2],
    dataContainer->program_icons[PROGRAM_BUG2]);
    gridProgramEditPanel->addObject(bug2Button);
    GUIButton* bug3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG3);
    },
    dataContainer->program_icons[PROGRAM_BUG3],
    dataContainer->program_icons[PROGRAM_BUG3],
    dataContainer->program_icons[PROGRAM_BUG3]);
    gridProgramEditPanel->addObject(bug3Button);
    GUIButton* catapultButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CATAPULT);
    },
    dataContainer->program_icons[PROGRAM_CATAPULT],
    dataContainer->program_icons[PROGRAM_CATAPULT],
    dataContainer->program_icons[PROGRAM_CATAPULT]);
    gridProgramEditPanel->addObject(catapultButton);
    GUIButton* clogButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG);
    },
    dataContainer->program_icons[PROGRAM_CLOG],
    dataContainer->program_icons[PROGRAM_CLOG],
    dataContainer->program_icons[PROGRAM_CLOG]);
    gridProgramEditPanel->addObject(clogButton);
    GUIButton* clog2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG2);
    },
    dataContainer->program_icons[PROGRAM_CLOG2],
    dataContainer->program_icons[PROGRAM_CLOG2],
    dataContainer->program_icons[PROGRAM_CLOG2]);
    gridProgramEditPanel->addObject(clog2Button);
    GUIButton* clog3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG3);
    },
    dataContainer->program_icons[PROGRAM_CLOG3],
    dataContainer->program_icons[PROGRAM_CLOG3],
    dataContainer->program_icons[PROGRAM_CLOG3]);
    gridProgramEditPanel->addObject(clog3Button);
    GUIButton* databombButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DATABOMB);
    },
    dataContainer->program_icons[PROGRAM_DATABOMB],
    dataContainer->program_icons[PROGRAM_DATABOMB],
    dataContainer->program_icons[PROGRAM_DATABOMB]);
    gridProgramEditPanel->addObject(databombButton);
    GUIButton* datadoctorButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
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
    GUIButton* datadoctor2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DATADOCTOR2);
    },
    dataContainer->program_icons[PROGRAM_DATADOCTOR2],
    dataContainer->program_icons[PROGRAM_DATADOCTOR2],
    dataContainer->program_icons[PROGRAM_DATADOCTOR2]);
    gridProgramEditPanel->addObject(datadoctor2Button);
    GUIButton* dogButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                         []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG);
    },
    dataContainer->program_icons[PROGRAM_DOG],
    dataContainer->program_icons[PROGRAM_DOG],
    dataContainer->program_icons[PROGRAM_DOG]);
    gridProgramEditPanel->addObject(dogButton);
    GUIButton* dog2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG2);
    },
    dataContainer->program_icons[PROGRAM_DOG2],
    dataContainer->program_icons[PROGRAM_DOG2],
    dataContainer->program_icons[PROGRAM_DOG2]);
    gridProgramEditPanel->addObject(dog2Button);
    GUIButton* dog3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG3);
    },
    dataContainer->program_icons[PROGRAM_DOG3],
    dataContainer->program_icons[PROGRAM_DOG3],
    dataContainer->program_icons[PROGRAM_DOG3]);
    gridProgramEditPanel->addObject(dog3Button);
    GUIButton* fiddleButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_FIDDLE);
    },
    dataContainer->program_icons[PROGRAM_FIDDLE],
    dataContainer->program_icons[PROGRAM_FIDDLE],
    dataContainer->program_icons[PROGRAM_FIDDLE]);
    gridProgramEditPanel->addObject(fiddleButton);
    GUIButton* firewallButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_FIREWALL);
    },
    dataContainer->program_icons[PROGRAM_FIREWALL],
    dataContainer->program_icons[PROGRAM_FIREWALL],
    dataContainer->program_icons[PROGRAM_FIREWALL]);
    gridProgramEditPanel->addObject(firewallButton);
    GUIButton* golemButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM);
    },
    dataContainer->program_icons[PROGRAM_GOLEM],
    dataContainer->program_icons[PROGRAM_GOLEM],
    dataContainer->program_icons[PROGRAM_GOLEM]);
    gridProgramEditPanel->addObject(golemButton);
    GUIButton* golem2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM2);
    },
    dataContainer->program_icons[PROGRAM_GOLEM2],
    dataContainer->program_icons[PROGRAM_GOLEM2],
    dataContainer->program_icons[PROGRAM_GOLEM2]);
    gridProgramEditPanel->addObject(golem2Button);
    GUIButton* golem3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM3);
    },
    dataContainer->program_icons[PROGRAM_GOLEM3],
    dataContainer->program_icons[PROGRAM_GOLEM3],
    dataContainer->program_icons[PROGRAM_GOLEM3]);
    gridProgramEditPanel->addObject(golem3Button);
    GUIButton* hackButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK);
    },
    dataContainer->program_icons[PROGRAM_HACK],
    dataContainer->program_icons[PROGRAM_HACK],
    dataContainer->program_icons[PROGRAM_HACK]);
    gridProgramEditPanel->addObject(hackButton);
    GUIButton* hack2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK2);
    },
    dataContainer->program_icons[PROGRAM_HACK2],
    dataContainer->program_icons[PROGRAM_HACK2],
    dataContainer->program_icons[PROGRAM_HACK2]);
    gridProgramEditPanel->addObject(hack2Button);
    GUIButton* hack3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK3);
    },
    dataContainer->program_icons[PROGRAM_HACK3],
    dataContainer->program_icons[PROGRAM_HACK3],
    dataContainer->program_icons[PROGRAM_HACK3]);
    gridProgramEditPanel->addObject(hack3Button);
    GUIButton* kamikazeeButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_KAMIKAZEE);
    },
    dataContainer->program_icons[PROGRAM_KAMIKAZEE],
    dataContainer->program_icons[PROGRAM_KAMIKAZEE],
    dataContainer->program_icons[PROGRAM_KAMIKAZEE]);
    gridProgramEditPanel->addObject(kamikazeeButton);
    GUIButton* medicButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_MEDIC);
    },
    dataContainer->program_icons[PROGRAM_MEDIC],
    dataContainer->program_icons[PROGRAM_MEDIC],
    dataContainer->program_icons[PROGRAM_MEDIC]);
    gridProgramEditPanel->addObject(medicButton);
    GUIButton* memhogButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
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
    GUIButton* mobiletowerButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_MOBILETOWER);
    },
    dataContainer->program_icons[PROGRAM_MOBILETOWER],
    dataContainer->program_icons[PROGRAM_MOBILETOWER],
    dataContainer->program_icons[PROGRAM_MOBILETOWER]);
    gridProgramEditPanel->addObject(mobiletowerButton);
    GUIButton* satelliteButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SATELLITE);
    },
    dataContainer->program_icons[PROGRAM_SATELLITE],
    dataContainer->program_icons[PROGRAM_SATELLITE],
    dataContainer->program_icons[PROGRAM_SATELLITE]);
    gridProgramEditPanel->addObject(satelliteButton);
    GUIButton* satellite2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SATELLITE2);
    },
    dataContainer->program_icons[PROGRAM_SATELLITE2],
    dataContainer->program_icons[PROGRAM_SATELLITE2],
    dataContainer->program_icons[PROGRAM_SATELLITE2]);
    gridProgramEditPanel->addObject(satellite2Button);
    GUIButton* seekerButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER);
    },
    dataContainer->program_icons[PROGRAM_SEEKER],
    dataContainer->program_icons[PROGRAM_SEEKER],
    dataContainer->program_icons[PROGRAM_SEEKER]);
    gridProgramEditPanel->addObject(seekerButton);
    GUIButton* seeker2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER2);
    },
    dataContainer->program_icons[PROGRAM_SEEKER2],
    dataContainer->program_icons[PROGRAM_SEEKER2],
    dataContainer->program_icons[PROGRAM_SEEKER2]);
    gridProgramEditPanel->addObject(seeker2Button);
    GUIButton* seeker3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER3);
    },
    dataContainer->program_icons[PROGRAM_SEEKER3],
    dataContainer->program_icons[PROGRAM_SEEKER3],
    dataContainer->program_icons[PROGRAM_SEEKER3]);
    gridProgramEditPanel->addObject(seeker3Button);
    GUIButton* slingshotButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SLINGSHOT);
    },
    dataContainer->program_icons[PROGRAM_SLINGSHOT],
    dataContainer->program_icons[PROGRAM_SLINGSHOT],
    dataContainer->program_icons[PROGRAM_SLINGSHOT]);
    gridProgramEditPanel->addObject(slingshotButton);
    GUIButton* sonarButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR);
    },
    dataContainer->program_icons[PROGRAM_SONAR],
    dataContainer->program_icons[PROGRAM_SONAR],
    dataContainer->program_icons[PROGRAM_SONAR]);
    gridProgramEditPanel->addObject(sonarButton);
    GUIButton* sonar2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR2);
    },
    dataContainer->program_icons[PROGRAM_SONAR2],
    dataContainer->program_icons[PROGRAM_SONAR2],
    dataContainer->program_icons[PROGRAM_SONAR2]);
    gridProgramEditPanel->addObject(sonar2Button);
    GUIButton* sonar3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR3);
    },
    dataContainer->program_icons[PROGRAM_SONAR3],
    dataContainer->program_icons[PROGRAM_SONAR3],
    dataContainer->program_icons[PROGRAM_SONAR3]);
    gridProgramEditPanel->addObject(sonar3Button);
    GUIButton* specsButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SPECS);
    },
    dataContainer->program_icons[PROGRAM_SPECS],
    dataContainer->program_icons[PROGRAM_SPECS],
    dataContainer->program_icons[PROGRAM_SPECS]);
    gridProgramEditPanel->addObject(specsButton);
    GUIButton* sumoButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SUMO);
    },
    dataContainer->program_icons[PROGRAM_SUMO],
    dataContainer->program_icons[PROGRAM_SUMO],
    dataContainer->program_icons[PROGRAM_SUMO]);
    gridProgramEditPanel->addObject(sumoButton);
    GUIButton* tarantulaButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TARANTULA);
    },
    dataContainer->program_icons[PROGRAM_TARANTULA],
    dataContainer->program_icons[PROGRAM_TARANTULA],
    dataContainer->program_icons[PROGRAM_TARANTULA]);
    gridProgramEditPanel->addObject(tarantulaButton);
    GUIButton* towerButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TOWER);
    },
    dataContainer->program_icons[PROGRAM_TOWER],
    dataContainer->program_icons[PROGRAM_TOWER],
    dataContainer->program_icons[PROGRAM_TOWER]);
    gridProgramEditPanel->addObject(towerButton);
    GUIButton* turboButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
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
    GUIButton* turbo2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TURBO2);
    },
    dataContainer->program_icons[PROGRAM_TURBO2],
    dataContainer->program_icons[PROGRAM_TURBO2],
    dataContainer->program_icons[PROGRAM_TURBO2]);
    gridProgramEditPanel->addObject(turbo2Button);
    GUIButton* turbo3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TURBO3);
    },
    dataContainer->program_icons[PROGRAM_TURBO3],
    dataContainer->program_icons[PROGRAM_TURBO3],
    dataContainer->program_icons[PROGRAM_TURBO3]);
    gridProgramEditPanel->addObject(turbo3Button);
    GUIButton* walkerButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER);
    },
    dataContainer->program_icons[PROGRAM_WALKER],
    dataContainer->program_icons[PROGRAM_WALKER],
    dataContainer->program_icons[PROGRAM_WALKER]);
    gridProgramEditPanel->addObject(walkerButton);
    GUIButton* walker2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER2);
    },
    dataContainer->program_icons[PROGRAM_WALKER2],
    dataContainer->program_icons[PROGRAM_WALKER2],
    dataContainer->program_icons[PROGRAM_WALKER2]);
    gridProgramEditPanel->addObject(walker2Button);
    GUIButton* walker3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER3);
    },
    dataContainer->program_icons[PROGRAM_WALKER3],
    dataContainer->program_icons[PROGRAM_WALKER3],
    dataContainer->program_icons[PROGRAM_WALKER3]);
    gridProgramEditPanel->addObject(walker3Button);
    GUIButton* wardenButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN);
    },
    dataContainer->program_icons[PROGRAM_WARDEN],
    dataContainer->program_icons[PROGRAM_WARDEN],
    dataContainer->program_icons[PROGRAM_WARDEN]);
    gridProgramEditPanel->addObject(wardenButton);
    GUIButton* warden2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN2);
    },
    dataContainer->program_icons[PROGRAM_WARDEN2],
    dataContainer->program_icons[PROGRAM_WARDEN2],
    dataContainer->program_icons[PROGRAM_WARDEN2]);
    gridProgramEditPanel->addObject(warden2Button);
    GUIButton* warden3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN3);
    },
    dataContainer->program_icons[PROGRAM_WARDEN3],
    dataContainer->program_icons[PROGRAM_WARDEN3],
    dataContainer->program_icons[PROGRAM_WARDEN3]);
    gridProgramEditPanel->addObject(warden3Button);
    GUIButton* watchmanButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN],
    dataContainer->program_icons[PROGRAM_WATCHMAN],
    dataContainer->program_icons[PROGRAM_WATCHMAN]);
    gridProgramEditPanel->addObject(watchmanButton);
    GUIButton* watchman2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN2);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN2],
    dataContainer->program_icons[PROGRAM_WATCHMAN2],
    dataContainer->program_icons[PROGRAM_WATCHMAN2]);
    gridProgramEditPanel->addObject(watchman2Button);
    GUIButton* watchman3Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN3);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN3],
    dataContainer->program_icons[PROGRAM_WATCHMAN3],
    dataContainer->program_icons[PROGRAM_WATCHMAN3]);
    gridProgramEditPanel->addObject(watchman3Button);
    GUIButton* wizardButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WIZARD);
    },
    dataContainer->program_icons[PROGRAM_WIZARD],
    dataContainer->program_icons[PROGRAM_WIZARD],
    dataContainer->program_icons[PROGRAM_WIZARD]);
    gridProgramEditPanel->addObject(wizardButton);
    GUIButton* wolfspiderButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
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
    gridItemEditPanel = new GUIContainer(ANCHOR_NORTHWEST, 20, 200, 4 + 36*4, 36, this, NULL);
    GUIButton* creditButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridItemEditPanel,
                                            []()
    {
        gameScreen->setBrushItem(ITEM_CREDIT);
    },
    dataContainer->item_icons[ITEM_CREDIT],
    dataContainer->item_icons[ITEM_CREDIT],
    dataContainer->item_icons[ITEM_CREDIT]);
    GUIButton* bigCreditButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridItemEditPanel,
            []()
    {
        gameScreen->setBrushItem(ITEM_BIGCREDIT);
    },
    dataContainer->item_icons[ITEM_BIGCREDIT],
    dataContainer->item_icons[ITEM_BIGCREDIT],
    dataContainer->item_icons[ITEM_BIGCREDIT]);
    GUIButton* filesButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridItemEditPanel,
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
    gridSelectBrushPanel = new GUIContainer(ANCHOR_NORTHWEST, 20, 240, 100, 200, this, NULL);
    GUIButton* brushNoneButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 45, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_NONE);
    },
    dataContainer->editorBrushNone,
    dataContainer->editorBrushNone,
    dataContainer->editorBrushNone);
    GUIButton* brushTileButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 45, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_TILES);
    },
    dataContainer->editorBrushTiles,
    dataContainer->editorBrushTiles,
    dataContainer->editorBrushTiles);
    GUIButton* brushProgramButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 73, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_PROGRAMS);
    },
    dataContainer->editorBrushPrograms,
    dataContainer->editorBrushPrograms,
    dataContainer->editorBrushPrograms);
    GUIButton* brushClearProgramButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 73, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_DELETEPROGRAMS);
    },
    dataContainer->editorBrushDeletePrograms,
    dataContainer->editorBrushDeletePrograms,
    dataContainer->editorBrushDeletePrograms);
    GUIButton* brushItemButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 73, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushMode(BRUSH_ITEMS);
    },
    dataContainer->editorBrushItems,
    dataContainer->editorBrushItems,
    dataContainer->editorBrushItems);
    col++;
    GUIButton* brushTeamPlayerButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 73, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushProgramTeam(0);
    },
    dataContainer->editorTeamPlayer,
    dataContainer->editorTeamPlayer,
    dataContainer->editorTeamPlayer);
    GUIButton* brushTeamComputerButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 73, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->setBrushProgramTeam(1);
    },
    dataContainer->editorTeamComputer,
    dataContainer->editorTeamComputer,
    dataContainer->editorTeamComputer);
    GUIButton* brushTeamViewButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 73, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->toggleViewTeams();
    },
    dataContainer->editorTeamViewToggle,
    dataContainer->editorTeamViewToggle,
    dataContainer->editorTeamViewToggle);
    GUIButton* brushClearGridButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 44, 12, gridSelectBrushPanel,
            []()
    {
        gameScreen->clearLevel();
    },
    dataContainer->editorClearGrid,
    dataContainer->editorClearGrid,
    dataContainer->editorClearGrid);

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
    gridBkgPanel = new GUIContainer(ANCHOR_NORTHWEST, 124, 240, 100, 200, this, NULL);
    GUIButton* bkgDonutButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 60, 12, gridBkgPanel,
            []()
    {
        gameScreen->setBackground(BKG_DONUT);
    },
    dataContainer->level_bkg_buttons[BKG_DONUT],
    dataContainer->level_bkg_buttons[BKG_DONUT],
    dataContainer->level_bkg_buttons[BKG_DONUT]);
    gridBkgPanel->addObject(bkgDonutButton);
    GUIButton* bkgPharmButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 60, 12, gridBkgPanel,
            []()
    {
        gameScreen->setBackground(BKG_PHARM);
    },
    dataContainer->level_bkg_buttons[BKG_PHARM],
    dataContainer->level_bkg_buttons[BKG_PHARM],
    dataContainer->level_bkg_buttons[BKG_PHARM]);
    gridBkgPanel->addObject(bkgPharmButton);
    GUIButton* bkgPedButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 60, 12, gridBkgPanel,
                                            []()
    {
        gameScreen->setBackground(BKG_PED);
    },
    dataContainer->level_bkg_buttons[BKG_PED],
    dataContainer->level_bkg_buttons[BKG_PED],
    dataContainer->level_bkg_buttons[BKG_PED]);
    gridBkgPanel->addObject(bkgPedButton);
    GUIButton* bkgMonkeyButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 60, 12, gridBkgPanel,
            []()
    {
        gameScreen->setBackground(BKG_MONKEY);
    },
    dataContainer->level_bkg_buttons[BKG_MONKEY],
    dataContainer->level_bkg_buttons[BKG_MONKEY],
    dataContainer->level_bkg_buttons[BKG_MONKEY]);
    gridBkgPanel->addObject(bkgMonkeyButton);
    GUIButton* bkgCellButton = new GUIButton(ANCHOR_NORTHWEST, 4, 4 + 16*col++, 60, 12, gridBkgPanel,
            []()
    {
        gameScreen->setBackground(BKG_CELL);
    },
    dataContainer->level_bkg_buttons[BKG_CELL],
    dataContainer->level_bkg_buttons[BKG_CELL],
    dataContainer->level_bkg_buttons[BKG_CELL]);
    gridBkgPanel->addObject(bkgCellButton);

    progDisp = new ProgramDisplayContainer(ANCHOR_SOUTHEAST, -220, -120, 200, 100, this);
    addObject(progDisp);

    GUIButton* endTurnButton = new GUIButton(ANCHOR_SOUTH, -100, -100, 200, 50, this,
                                             [](){gameScreen->endTurn();},
                                             dataContainer->gameButtonEndTurn);
    addObject(endTurnButton);

    GUIButton* abandonGameButton = new GUIButton(ANCHOR_NORTHEAST, -250, 25, 200, 50, this,
                                                 [](){currScreen = mapScreen; mapScreen->clearSelectedNode();},
                                                 dataContainer->gameButtonAbandonGame);
    addObject(abandonGameButton);
    GUIButton* winGameButton = new GUIButton(ANCHOR_NORTHEAST, -250, 100, 200, 50, this,
                                             [](){mapScreen->getSelectedNode()->winNode(); mapScreen->clearSelectedNode(); currScreen = mapScreen;},
                                             dataContainer->gameButtonWinGame);
    addObject(winGameButton);
    GUIButton* resetGameButton = new GUIButton(ANCHOR_NORTHEAST, -250, 175, 200, 50, this,
                                               [](){gameScreen->loadLevel(mapScreen->getSelectedNode()->getLevelStr());},
                                               dataContainer->gameButtonResetGame);
    addObject(resetGameButton);
}

void GameScreen::resetBounds()
{
    GUIContainer::resetBounds();
    checkShiftable();
    if (!canShiftScreen) centerScreen();
}

bool GameScreen::mouseDown()
{
    GUIContainer::mouseDown();

    if (debug)
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
            int x = (bkgX + mousePosX)/32;
            int y = (bkgY + mousePosY)/32;

            if (brushMode == BRUSH_PROGRAMS)
            {
                // TODO
            }
            else if (brushMode == BRUSH_TILES)
            {
                game->setTileAt(x, y, brushTileType);
            }
            else if (brushMode == BRUSH_DELETEPROGRAMS)
            {
                game->setProgramAt(x, y, NULL);
            }
            else if (brushMode == BRUSH_ITEMS)
            {
                game->setItemAt(x, y, brushItemType);
            }
            else
            {
                return false;
            }
            return true;
        }
    }
    else // if not in debug mode
    {
        int x = (bkgX + mousePosX)/32;
        int y = (bkgY + mousePosY)/32;

        game->moveSelectedProgram(x, y);
        game->setSelectedProgram(x, y);
        if (game->getSelectedProgram() == NULL)
        {
            selectedTileX = x;
            selectedTileY = y;
        }
        else
        {
            selectedTileX = game->getSelectedProgram()->getCoreX();
            selectedTileY = game->getSelectedProgram()->getCoreY();
        }

        return false;
    }
}

bool GameScreen::mouseUp()
{
    GUIContainer::mouseUp();

    bool r = false;

    if (debug)
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
    game->endTurn();
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
    int topLeftTileX = bkgX / 32;
    int topLeftTileY = bkgY / 32;
    int bottomRightTileX = topLeftTileX + SCREEN_WIDTH / 32 + 1;
    int bottomRightTileY = topLeftTileY + SCREEN_HEIGHT / 32 + 1;
    // check for overflows
    if (bottomRightTileX >= 200)
    {
        bottomRightTileX = 200;
    }
    if (bottomRightTileY >= 200)
    {
        bottomRightTileY = 200;
    }
    if (topLeftTileX < 0)
    {
        topLeftTileX = 0;
    }
    if (topLeftTileY < 0)
    {
        topLeftTileY = 0;
    }

    // draw grid
    for (int x = topLeftTileX; x < bottomRightTileX; x++)
    {
        for (int y = topLeftTileY; y < bottomRightTileY; y++)
        {
            if (game->getTileAt(x, y) == TILE_NONE && !debug)
            {
                continue;
            }

            // default position of a tile,
            // assuming it is 28x28 px
            int xDefault = 4 + 32*x - bkgX;
            int yDefault = 4 + 32*y - bkgY;
            int sizeDefault = 28;
            tileRect.x = xDefault;
            tileRect.y = yDefault;
            tileRect.w = sizeDefault;
            tileRect.h = sizeDefault;

            if (game->getProgramAt(x, y) != NULL)
            {
                // get this program
                Program* prog = game->getProgramAt(x, y);

                // draw this program's tile
                tileRect.x = xDefault - 1;
                tileRect.y = yDefault - 1;
                tileRect.w = sizeDefault + 2;
                tileRect.h = sizeDefault + 2;
                if (programViewTeams)
                {
                    if (prog->getTeam() == 0)
                    {
                        SDL_SetTextureColorMod(dataContainer->program_core, 0, 0, prog->getColor(2));
                    }
                    else
                    {
                        SDL_SetTextureColorMod(dataContainer->program_core, prog->getColor(0), 0, 0);
                    }
                }
                else
                {
                    SDL_SetTextureColorMod(dataContainer->program_core, prog->getColor(0), prog->getColor(1), prog->getColor(2));
                }

                // if this is the farthest chunk of this program
                if (prog == game->getSelectedProgram() && prog->getHealth() == prog->getMaxHealth())
                {
                    Pair<int>* currTail = game->getFarthestContiguousBlock(prog);
                    if (x == currTail->a && y == currTail->b)
                    {
                        if (tickCount % 100 < 50) SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);
                    }
                    else SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);
                    delete currTail;
                }
                else SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);

                // if this is part of the selected program, indicate it
                if (prog == game->getSelectedProgram() && debug)
                {
                    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
                    SDL_RenderDrawLine(gRenderer, tileRect.x, tileRect.y, tileRect.x + tileRect.w, tileRect.y + tileRect.h);
                    SDL_RenderDrawLine(gRenderer, tileRect.x + tileRect.w, tileRect.y, tileRect.x, tileRect.y + tileRect.h);
                }

                // draw the bridges from this program's tile to adjacent tiles
                if (game->getProgramAt(x, y+1) == prog)
                {
                    tileRect.x = xDefault + 9;
                    tileRect.y = yDefault + 26;
                    tileRect.w = 10;
                    tileRect.h = 5;
                    if (programViewTeams)
                    {
                        if (prog->getTeam() == 0)
                        {
                            SDL_SetTextureColorMod(dataContainer->program_core_vertical, 0, 0, prog->getColor(2));
                        }
                        else
                        {
                            SDL_SetTextureColorMod(dataContainer->program_core_vertical, prog->getColor(0), 0, 0);
                        }
                    }
                    else
                    {
                        SDL_SetTextureColorMod(dataContainer->program_core_vertical, prog->getColor(0), prog->getColor(1), prog->getColor(2));
                    }
                    SDL_RenderCopy(gRenderer, dataContainer->program_core_vertical, NULL, &tileRect);
                }
                if (game->getProgramAt(x+1, y) == prog)
                {
                    tileRect.x = xDefault + 26;
                    tileRect.y = yDefault + 8;
                    tileRect.w = 5;
                    tileRect.h = 12;
                    if (programViewTeams)
                    {
                        if (prog->getTeam() == 0)
                        {
                            SDL_SetTextureColorMod(dataContainer->program_core_horizontal, 0, 0, prog->getColor(2));
                        }
                        else
                        {
                            SDL_SetTextureColorMod(dataContainer->program_core_horizontal, prog->getColor(0), 0, 0);
                        }
                    }
                    else
                    {
                        SDL_SetTextureColorMod(dataContainer->program_core_horizontal, prog->getColor(0), prog->getColor(1), prog->getColor(2));
                    }
                    SDL_RenderCopy(gRenderer, dataContainer->program_core_horizontal, NULL, &tileRect);
                }

                // draw the icon IF this is the core tile
                if (prog->getCoreX() == x &&
                    prog->getCoreY() == y)
                {
                    tileRect.x = xDefault - 1;
                    tileRect.y = yDefault - 1;
                    tileRect.w = 27;
                    tileRect.h = 27;
                    SDL_RenderCopy(gRenderer, prog->getIcon(), NULL, &tileRect);

                    // draw the highlight rectangle if this program is selected
                    if (prog == game->getSelectedProgram())
                    {
                        tileRect.x = xDefault - 2;
                        tileRect.y = yDefault - 2;
                        tileRect.w = 32;
                        tileRect.h = 32;
                        SDL_SetTextureAlphaMod(dataContainer->tile_selected, ((double)-tickCount/1000.0)*255 + 255);
                        SDL_RenderCopy(gRenderer, dataContainer->tile_selected, NULL, &tileRect);
                    }
                }
            }
            else // if there is no program on this tile
            {
                SDL_Texture* tileImg = dataContainer->tile_images[game->getTileAt(x, y)];
                if (game->getTileAt(x, y) == TILE_NONE && !debug)
                {
                    continue;
                }

                SDL_QueryTexture(tileImg, NULL, NULL, &tileRect.w, &tileRect.h);
                tileRect.x = xDefault - (tileRect.w - 28)/2;
                tileRect.y = yDefault - (tileRect.h - 28)/2;
                SDL_RenderCopy(gRenderer, tileImg, NULL, &tileRect);
            }

            // if the mouse is over this tile
            if (mousePosX - tileRect.x > 0 &&
                mousePosX - (tileRect.x + tileRect.w) < 0 &&
                mousePosY - tileRect.y > 0 &&
                mousePosY - (tileRect.y + tileRect.h) < 0)
            {
                tileRect.x = xDefault;
                tileRect.y = yDefault;
                tileRect.w = sizeDefault;
                tileRect.h = sizeDefault;
                SDL_SetTextureAlphaMod(dataContainer->tile_over, 125);
                SDL_RenderCopy(gRenderer, dataContainer->tile_over, NULL, &tileRect);
            }

            if (game->getItemAt(x, y) != ITEM_NONE)
            {
                SDL_QueryTexture(dataContainer->item_icons[game->getItemAt(x, y)], NULL, NULL, &tileRect.w, &tileRect.h);
                tileRect.x = xDefault - (tileRect.w - 28)/2;
                tileRect.y = yDefault - (tileRect.h - 28)/2;
                SDL_RenderCopy(gRenderer, dataContainer->item_icons[game->getItemAt(x, y)], NULL, &tileRect);
            }

            if (x == selectedTileX && y == selectedTileY)
            {
                tileRect.x = xDefault - 2;
                tileRect.y = yDefault - 2;
                tileRect.w = 32;
                tileRect.h = 32;
                SDL_SetTextureAlphaMod(dataContainer->tile_selected, ((double)-tickCount/1000.0)*255 + 255);
                SDL_RenderCopy(gRenderer, dataContainer->tile_selected, NULL, &tileRect);
            }

            if (game->getSelectedProgramDist(x, y) != -1)
            {
                tileRect.x = xDefault;
                tileRect.y = yDefault;
                tileRect.w = sizeDefault;
                tileRect.h = sizeDefault;

                if (game->getSelectedProgramDist(x, y) == 0)
                {
                    continue;
                }
                if (x == game->getSelectedProgram()->getCoreX() && y == game->getSelectedProgram()->getCoreY() + 1)
                {
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveSouth, NULL, &tileRect);
                }
                else if (x == game->getSelectedProgram()->getCoreX() + 1 && y == game->getSelectedProgram()->getCoreY())
                {
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveEast, NULL, &tileRect);
                }
                else if (x == game->getSelectedProgram()->getCoreX() && y == game->getSelectedProgram()->getCoreY() - 1)
                {
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveNorth, NULL, &tileRect);
                }
                else if (x == game->getSelectedProgram()->getCoreX() - 1 && y == game->getSelectedProgram()->getCoreY())
                {
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveWest, NULL, &tileRect);
                }
                else
                {
                    SDL_RenderCopy(gRenderer, dataContainer->tile_movePossible, NULL, &tileRect);
                }
            }
        }
    }

    // draw board bounding rectangle
    if (debug)
    {
        tileRect.x = -bkgX + 4;
        tileRect.y = -bkgY + 4;
        tileRect.w = 32 * 200 - 4;
        tileRect.h = 32 * 200 - 4;
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 0);
        SDL_RenderDrawRect(gRenderer, &tileRect);

        SDL_RenderDrawLine(gRenderer, -bkgX + 100*32, -bkgY + 4, -bkgX + 100*32, -bkgY + 200*32); // vert
        SDL_RenderDrawLine(gRenderer, -bkgX + 4, -bkgY + 100*32, -bkgX + 200*32, -bkgY + 100*32); // horiz

        tileRect.x = -bkgX + game->getLeftBound()*32;
        tileRect.y = -bkgY + game->getTopBound()*32;
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

    if (debug)
    {
        gridEditPanel->draw();
        gridProgramEditPanel->draw();
        gridSelectBrushPanel->draw();
        gridItemEditPanel->draw();
        gridBkgPanel->draw();
    }
}

void GameScreen::shiftBkg(int x, int y)
{
    if (!canShiftScreen) return;

    if (bkgX + x + SCREEN_WIDTH/2 < game->getLeftBound()*32)
    {
        bkgX = game->getLeftBound()*32 - SCREEN_WIDTH/2;
    }
    else if (bkgX + x + SCREEN_WIDTH/2 > game->getRightBound()*32)
    {
        bkgX = game->getRightBound()*32 - SCREEN_WIDTH/2;
    }
    else
    {
        bkgX += x;
    }

    if (bkgY + y + SCREEN_HEIGHT/2 < game->getTopBound()*32)
    {
        bkgY = game->getTopBound()*32 - SCREEN_HEIGHT/2;
    }
    else if (bkgY + y + SCREEN_HEIGHT/2 > game->getBottomBound()*32)
    {
        bkgY = game->getBottomBound()*32 - SCREEN_HEIGHT/2;
    }
    else
    {
        bkgY += y;
    }
}

void GameScreen::setBrushTileType(TILE t)
{
    brushTileType = t;
}

void GameScreen::setBrushProgramType(PROGRAM p)
{
    brushProgramType = p;
    game->setSelectedProgram(-1, -1);
}

void GameScreen::setBrushMode(BRUSH b)
{
    brushMode = b;
}

void GameScreen::setBrushProgramTeam(int t)
{
    game->setSelectedProgram(-1, -1);
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

void GameScreen::setBackground(BACKGROUND b)
{
    bkgImg = dataContainer->level_backgrounds[b];
    game->setBackground(b);
}

void GameScreen::checkShiftable()
{
    if ((game->getRightBound() - game->getLeftBound())*32 < SCREEN_WIDTH - 200 &&
        (game->getBottomBound() - game->getTopBound())*32 < SCREEN_HEIGHT - 200)
        canShiftScreen = false;
    else canShiftScreen = true;
}

Program* GameScreen::getSelectedProgram()
{
    return game->getSelectedProgram();
}

void GameScreen::tick(int ms)
{
    // adjust time-dependent textures
    tickCount += ms;
    if (tickCount >= 1000)
    {
        tickCount = 0;
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
    if (mousePosX < 20)
    {
        shiftBkg(-shiftAmt, 0);
    }
    else if (mousePosX > SCREEN_WIDTH - 20)
    {
        shiftBkg(shiftAmt, 0);
    }

    if (mousePosY < 20)
    {
        shiftBkg(0, -shiftAmt);
    }
    else if (mousePosY > SCREEN_HEIGHT - 20)
    {
        shiftBkg(0, shiftAmt);
    }
}

void GameScreen::saveLevel()
{
    game->saveLevel();
}

void GameScreen::loadLevel(std::string s)
{
    delete game;
    game = new Game(s);
    checkShiftable();
    centerScreen();
}

void GameScreen::clearLevel()
{
    delete game;
    game = new Game();
    canShiftScreen = false;
    centerScreen();
}

void GameScreen::centerScreen()
{
    bkgX = (game->getRightBound() + game->getLeftBound())*32/2 - SCREEN_WIDTH/2;
    bkgY = (game->getBottomBound() + game->getTopBound())*32/2 - SCREEN_HEIGHT/2;
}

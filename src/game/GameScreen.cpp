#include "GameScreen.h"
#include "GUIContainer.h"
#include "Global.h"
#include "Program.h"
#include "GUIButton.h"
#include "LinkedList.h"
#include "ResourceLoader.h"
#include "Pair.h"

#include <stdlib.h>
#include <stdio.h>
#include <fstream>

GameScreen::GameScreen()
    : GUIContainer(ANCHOR_NORTHWEST, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, loadTexture("resources/company_4.png"))
{
    initBoard();
    buildGUI();

    brushMode = BRUSH_NONE;
    brushTileType = TILE_NONE;
    brushProgramType = PROGRAM_BALLISTA;
    brushProgramTeam = 0;
    brushItemType = ITEM_NONE;
    programViewTeams = false;
    setSelectedProgram(NULL);
    selectedTileX = 0;
    selectedTileY = 0;
    bkgX = 0;
    bkgY = 0;
    canPanScreen = true;
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
    dataContainer->tile_images[TILE_NONE],
    dataContainer->tile_images[TILE_NONE],
    dataContainer->tile_images[TILE_NONE]);
    GUIButton* plainButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN);
    },
    dataContainer->tile_images[TILE_PLAIN],
    dataContainer->tile_images[TILE_PLAIN],
    dataContainer->tile_images[TILE_PLAIN]);
    GUIButton* plain2Button = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4, 28, 28, gridEditPanel,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN2);
    },
    dataContainer->tile_images[TILE_PLAIN2],
    dataContainer->tile_images[TILE_PLAIN2],
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
    GUIButton* nullProgramButton = new GUIButton(ANCHOR_NORTHWEST, 4 + 32*col++, 4 + 32*ln, 28, 28, gridProgramEditPanel,
            []()
    {
        gameScreen->setSelectedProgram(NULL);
    },
    NULL,
    NULL,
    NULL);
    gridProgramEditPanel->addObject(nullProgramButton);
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
}

void GameScreen::resetBounds()
{
    GUIContainer::resetBounds();
    checkShiftable();
    if (!canPanScreen)
    {
        bkgX = (gridRightBound + gridLeftBound)*32/2 - SCREEN_WIDTH/2;
        bkgY = (gridBottomBound + gridTopBound)*32/2 - SCREEN_HEIGHT/2;
    }
}

bool GameScreen::mouseDown()
{
    if (debug)
    {
        if (gridEditPanel->isMouseOver()) // click the tile pallette
        {
            gridEditPanel->mouseDown();
            return true;
        }
        else if (gridProgramEditPanel->isMouseOver()) // click the program pallette
        {
            gridProgramEditPanel->mouseDown();
            return true;
        }
        else if (gridSelectBrushPanel->isMouseOver())
        {
            gridSelectBrushPanel->mouseDown();
            return true;
        }
        else if (gridItemEditPanel->isMouseOver())
        {
            gridItemEditPanel->mouseDown();
            return true;
        }
        else if (gridBkgPanel->isMouseOver())
        {
            gridBkgPanel->mouseDown();
            return true;
        }
        else // click the grid
        {
            int currTileX = (bkgX + mousePosX)/32;
            int currTileY = (bkgY + mousePosY)/32;
            if (brushMode == BRUSH_PROGRAMS)
            {
                if (gridTiles[currTileX][currTileY] == TILE_NONE)
                {
                    return false;
                }

                if (gridPrograms[currTileX][currTileY] == NULL)
                {
                    if (selectedProgram == NULL)
                    {
                        gridPrograms[currTileX][currTileY] = new Program(brushProgramType, brushProgramTeam, currTileX, currTileY);
                        selectedProgram = gridPrograms[currTileX][currTileY];
                    }
                    else
                    {
                        gridPrograms[currTileX][currTileY] = selectedProgram;
                    }
                }
                else if (gridPrograms[currTileX][currTileY] != NULL &&
                         gridPrograms[currTileX][currTileY]->getCoreX() == currTileX &&
                         gridPrograms[currTileX][currTileY]->getCoreY() == currTileY)
                {
                    selectedProgram = gridPrograms[currTileX][currTileY];
                }
            }
            else if (brushMode == BRUSH_TILES)
            {
                setTileAt(currTileX, currTileY, brushTileType);
            }
            else if (brushMode == BRUSH_DELETEPROGRAMS)
            {
                if (gridPrograms[currTileX][currTileY] != NULL)
                {
                    if (gridPrograms[currTileX][currTileY]->getCoreX() == currTileX &&
                            gridPrograms[currTileX][currTileY]->getCoreY() == currTileY)
                    {
                        Program* temp = gridPrograms[currTileX][currTileY];
                        deleteProgram(temp);
                    }
                    else
                    {
                        gridPrograms[currTileX][currTileY] = NULL;
                    }
                }
            }
            else if (brushMode == BRUSH_ITEMS)
            {
                if (gridTiles[currTileX][currTileY] != TILE_NONE)
                {
                    gridItems[currTileX][currTileY] = brushItemType;
                }
            }
            else
            {
                return false;
            }
            return true;
        }
    }
    else
    {
        int currTileX = (bkgX + mousePosX)/32;
        int currTileY = (bkgY + mousePosY)/32;

        selectedTileX = currTileX;
        selectedTileY = currTileY;
        setSelectedProgram(gridPrograms[currTileX][currTileY]);

        return false;
    }
}

void GameScreen::drawBkg()
{
    // draw background image
    SDL_RenderCopy(gRenderer, bkgImg, NULL, NULL);
}

void GameScreen::drawContents()
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
            if (gridTiles[x][y] == TILE_NONE && !debug)
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

            if (gridPrograms[x][y] != NULL)
            {
                // get this program
                Program* prog = gridPrograms[x][y];

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
                SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);

                // if this is part of the selected program, indicate it
                if (gridPrograms[x][y] == selectedProgram && debug)
                {
                    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
                    SDL_RenderDrawLine(gRenderer, tileRect.x, tileRect.y, tileRect.x + tileRect.w, tileRect.y + tileRect.h);
                    SDL_RenderDrawLine(gRenderer, tileRect.x + tileRect.w, tileRect.y, tileRect.x, tileRect.y + tileRect.h);
                }

                // draw the bridges from this program's tile to adjacent tiles
                if (gridPrograms[x][y+1] == gridPrograms[x][y])
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
                if (gridPrograms[x+1][y] == gridPrograms[x][y])
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
                if (gridPrograms[x][y]->getCoreX() == x &&
                        gridPrograms[x][y]->getCoreY() == y)
                {
                    tileRect.x = xDefault - 1;
                    tileRect.y = yDefault - 1;
                    tileRect.w = 27;
                    tileRect.h = 27;
                    SDL_RenderCopy(gRenderer, gridPrograms[x][y]->getIcon(), NULL, &tileRect);
                }
            }
            else
            {
                SDL_Texture* tileImg = dataContainer->tile_images[gridTiles[x][y]];
                if (gridTiles[x][y] == TILE_NONE && !debug)
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

            if (gridItems[x][y] != ITEM_NONE)
            {
                SDL_QueryTexture(dataContainer->item_icons[gridItems[x][y]], NULL, NULL, &tileRect.w, &tileRect.h);
                tileRect.x = xDefault - (tileRect.w - 28)/2;
                tileRect.y = yDefault - (tileRect.h - 28)/2;
                SDL_RenderCopy(gRenderer, dataContainer->item_icons[gridItems[x][y]], NULL, &tileRect);
            }

            if (x == selectedTileX && y == selectedTileY)
            {
                tileRect.x = xDefault - 2;
                tileRect.y = yDefault - 2;
                tileRect.w = 32;
                tileRect.h = 32;
                SDL_RenderCopy(gRenderer, dataContainer->tile_selected, NULL, &tileRect);
            }

            if (gridProgramDist[x][y] != -1)
            {
                tileRect.x = xDefault;
                tileRect.y = yDefault;
                tileRect.w = sizeDefault;
                tileRect.w = sizeDefault;

                switch (gridProgramDist[x][y])
                {
                case 0:
                    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
                    break;
                case 1:
                    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
                    break;
                case 2:
                    SDL_SetRenderDrawColor(gRenderer, 255, 128, 0, 255);
                    break;
                case 3:
                    SDL_SetRenderDrawColor(gRenderer, 128, 255, 0, 255);
                    break;
                case 4:
                    SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
                    break;
                case 5:
                    SDL_SetRenderDrawColor(gRenderer, 0, 255, 128, 255);
                    break;
                default:
                    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
                    break;
                }
                SDL_RenderFillRect(gRenderer, &tileRect);
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

        tileRect.x = -bkgX + gridLeftBound*32;
        tileRect.y = -bkgY + gridTopBound*32;
        tileRect.w = (gridRightBound - gridLeftBound)*32;
        tileRect.h = (gridBottomBound - gridTopBound)*32;
        SDL_RenderDrawRect(gRenderer, &tileRect);
    }
}

void GameScreen::draw()
{
    drawBkg();
    drawContents();
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
    if (!canPanScreen) return;

    if (bkgX + x + SCREEN_WIDTH/2 < gridLeftBound*32)
    {
        bkgX = gridLeftBound*32 - SCREEN_WIDTH/2;
    }
    else if (bkgX + x + SCREEN_WIDTH/2 > gridRightBound*32)
    {
        bkgX = gridRightBound*32 - SCREEN_WIDTH/2;
    }
    else
    {
        bkgX += x;
    }

    if (bkgY + y + SCREEN_HEIGHT/2 < gridTopBound*32)
    {
        bkgY = gridTopBound*32 - SCREEN_HEIGHT/2;
    }
    else if (bkgY + y + SCREEN_HEIGHT/2 > gridBottomBound*32)
    {
        bkgY = gridBottomBound*32 - SCREEN_HEIGHT/2;
    }
    else
    {
        bkgY += y;
    }
}

bool GameScreen::isDrawValid(int x, int y, int height, int width)
{
    if (x < 0 || x + width > 200 ||
            y < 0 || y + height > 200)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void GameScreen::drawRectInBoard(TILE tileType, int x, int y, int width, int height)
{
    if (!isDrawValid(x, y, width, height))
    {
        return;
    }

    for (int xx = x; xx < x + width; xx++)
    {
        setTileAt(xx, y, tileType);
        setTileAt(xx, y + height - 1, tileType);
    }
    for (int yy = y; yy < y + height; yy++)
    {
        setTileAt(x, yy, tileType);
        setTileAt(x + width - 1, yy, tileType);
    }
}

void GameScreen::fillRectInBoard(TILE tileType, int x, int y, int width, int height)
{
    if (!isDrawValid(x, y, width, height))
    {
        return;
    }

    for (int i = x; i < x + width; i++)
    {
        for (int j = y; j < y + height; j++)
        {
            setTileAt(i, j, tileType);
        }
    }
}

void GameScreen::drawOvalInBoard(TILE tileType, int x, int y, int width, int height)
{

}

void GameScreen::fillOvalInBoard(TILE tileType, int x, int y, int width, int height)
{
    int xCenter = x + width/2;
    int yCenter = y + height/2;
    for (int i = x; i < x + width - 1; i++)
    {
        for (int j = y; j < y + height - 1; j++)
        {
            if (((i - xCenter)*(i - xCenter))/((width/2)*(width/2)) &&
                    ((j - yCenter)*(j - yCenter))/((height/2)*(height/2)))
            {
                setTileAt(i, j, tileType);
            }
        }
    }
}

void GameScreen::initBoard()
{
    for (int x = 0; x < 200; x++)
    {
        for (int y = 0; y < 200; y++)
        {
            gridTiles[x][y] = TILE_NONE;
            gridItems[x][y] = ITEM_NONE;
            gridPrograms[x][y] = NULL;
            gridProgramDist[x][y] = -1;
        }
    }

    gridLeftBound = 100;
    gridRightBound = 100;
    gridTopBound = 100;
    gridBottomBound = 100;
}

void GameScreen::clearLevel()
{
    for (int x = 0; x < 200; x++)
    {
        for (int y = 0; y < 200; y++)
        {
            gridTiles[x][y] = TILE_NONE;
            gridItems[x][y] = ITEM_NONE;
            if (gridPrograms[x][y] != NULL)
            {
                Program* temp = gridPrograms[x][y];
                deleteProgram(temp);
            }
        }
    }

    selectedProgram = NULL;

    gridLeftBound = 100;
    gridRightBound = 100;
    gridTopBound = 100;
    gridBottomBound = 100;
}

void GameScreen::saveLevel()
{
    std::ofstream lvl;
    lvl.open("levels/default.urf", std::ios::out | std::ios::binary | std::ios::trunc);
    if (!lvl.is_open())
    {
        if (debug) printf("err opening file");
    }
    else
    {
        if (debug) printf("saving level...\n");

        // begin by writing the sizes of various data types
        int8_t sizeOfInt = sizeof(int);
        int8_t sizeOfChar = sizeof(char);
        int8_t sizeOfDouble = sizeof(double);
        int8_t sizeOfBool = sizeof(bool);
        if (debug) printf("saving constants... int:%i, char:%i, double:%i, bool:%i\n", sizeOfInt, sizeOfChar, sizeOfDouble, sizeOfBool);
        lvl.write((char*) &sizeOfInt, 1);
        lvl.write((char*) &sizeOfChar, 1);
        lvl.write((char*) &sizeOfDouble, 1);
        lvl.write((char*) &sizeOfBool, 1);

        // write the size of the game grid to the file
        if (debug) printf("saving grid bounds... left:%i, right:%i, top:%i, bottom:%i\n", gridLeftBound, gridRightBound, gridTopBound, gridBottomBound);
        lvl.write((char*) &gridLeftBound, sizeOfInt);
        lvl.write((char*) &gridRightBound, sizeOfInt);
        lvl.write((char*) &gridTopBound, sizeOfInt);
        lvl.write((char*) &gridBottomBound, sizeOfInt);

        // write the enum of the level's background
        lvl.write((char*) &bkgTex, sizeOfInt);

        // collect all the programs in a linked list
        if (debug) printf("gathering program list...\n");
        LinkedList<Program*> progs = LinkedList<Program*>();
        for (int x = gridLeftBound; x < gridRightBound; x++)
        {
            for (int y = gridTopBound; y < gridBottomBound; y++)
            {
                if (gridPrograms[x][y] == NULL)
                {
                    continue;
                }
                if (progs.contains(gridPrograms[x][y]))
                {
                    if (debug) printf("saving program\n");
                    progs.addLast(gridPrograms[x][y]);
                }
            }
        }

        // write all programs to the file
        int numPrograms = progs.getLength();
        if (debug) printf("saving %i programs...\n", numPrograms);
        lvl.write((char*)(&numPrograms), sizeOfInt);
        for (int i = 0; i < progs.getLength(); i++)
        {
            Program* currProg = progs.getObjectAt(i);

            int xHead = currProg->getCoreX();
            lvl.write((char*)(&xHead), sizeOfInt);
            int yHead = currProg->getCoreY();
            lvl.write((char*)(&yHead), sizeOfInt);
            int type = currProg->getType();
            lvl.write((char*)(&type), sizeOfInt);
            int health = currProg->getHealth();
            lvl.write((char*)(&health), sizeOfInt);
            int maxHealth = currProg->getMaxHealth();
            lvl.write((char*)(&maxHealth), sizeOfInt);
            int speed = currProg->getSpeed();
            lvl.write((char*)(&speed), sizeOfInt);
            int team = currProg->getTeam();
            lvl.write((char*)(&team), sizeOfInt);
        }

        // write the grid to the file
        if (debug) printf("saving tiles, items, and program pointers...\n");
        for (int x = gridLeftBound; x < gridRightBound; x++)
        {
            for (int y = gridTopBound; y < gridBottomBound; y++)
            {
                lvl.write((char*)(&(gridTiles[x][y])), sizeOfInt);
                lvl.write((char*)(&(gridItems[x][y])), sizeOfInt);
                int index = progs.getIndexOf(gridPrograms[x][y]);
                lvl.write((char*)(&index), sizeOfInt);
            }
        }

        //TODO: DELETE PROGRAM LISTNODES

        // flush and close the file
        if (debug) printf("flushing and closing save file... ");
        lvl.flush();
        lvl.close();
        if (debug) printf("done\n");
    }
}

void GameScreen::loadLevel(std::string str)
{
    std::ifstream lvl;
    if (str.size() == 0)
    {
        lvl.open("levels/default.urf", std::ios::in | std::ios::binary);
    }
    else
    {
        lvl.open(str, std::ios::in | std::ios::binary);
    }

    if (!lvl.is_open())
    {
        if (debug) printf("err opening level %s\n", str.c_str());
    }
    else
    {
        if (debug) printf("loading level %s...\n", str.c_str());

        // clear the grid
        clearLevel();

        // read the sizes of various data types
        if (debug) printf("loading constants...\n");
        int8_t sizeOfInt;
        lvl.read((char*) &sizeOfInt, 1);
        int8_t sizeOfChar;
        lvl.read((char*) &sizeOfChar, 1);
        int8_t sizeOfDouble;
        lvl.read((char*) &sizeOfDouble, 1);
        int8_t sizeOfBool;
        lvl.read((char*) &sizeOfBool, 1);

        // load the size of the game grid
        if (debug) printf("loading grid bounds...\n");
        int left, right, top, bottom;
        lvl.read((char*) &left, sizeOfInt);
        lvl.read((char*) &right, sizeOfInt);
        lvl.read((char*) &top, sizeOfInt);
        lvl.read((char*) &bottom, sizeOfInt);

        // load the enum of the level's background
        lvl.read((char*) &bkgTex, sizeOfInt);
        setBackground(bkgTex);

        // load the list of programs
        int numPrograms;
        lvl.read((char*)(&numPrograms), sizeOfInt);
        if (debug) printf("loading %i programs...\n", numPrograms);
        LinkedList<Program*> progs = LinkedList<Program*>();
        for (int i = 0; i < numPrograms; i++)
        {
            int xHead, yHead, type, health, maxHealth, speed, team;
            lvl.read((char*)(&xHead), sizeOfInt);
            lvl.read((char*)(&yHead), sizeOfInt);
            lvl.read((char*)(&type), sizeOfInt);
            lvl.read((char*)(&health), sizeOfInt);
            lvl.read((char*)(&maxHealth), sizeOfInt);
            lvl.read((char*)(&speed), sizeOfInt);
            lvl.read((char*)(&team), sizeOfInt);
            Program* p = new Program((PROGRAM)type, team, xHead, yHead);
            p->setHealth(health);
            p->setMaxHealth(maxHealth);
            p->setSpeed(speed);
            progs.addLast(p);
        }

        // load the grid from the file
        if (debug) printf("loading tiles, items, and program pointers...\n");
        for (int x = left; x < right; x++)
        {
            for (int y = top; y < bottom; y++)
            {
                // tiles
                TILE typ;
                lvl.read((char*)(&typ), sizeOfInt);
                setTileAt(x, y, typ);

                // items
                ITEM itm;
                lvl.read((char*)(&itm), sizeOfInt);
                gridItems[x][y] = itm;

                // programs
                int indx;
                lvl.read((char*)(&indx), sizeOfInt);
                gridPrograms[x][y] = progs.getObjectAt(indx);
            }
        }

        // set background position
        bkgX = ((gridRightBound + gridLeftBound)*32)/2 - SCREEN_WIDTH/2;
        bkgY = ((gridBottomBound + gridTopBound)*32)/2 - SCREEN_HEIGHT/2;
        checkShiftable();

        //TODO: DELETE PROGRAM LISTNODES

        // close the file
        lvl.close();
        if (debug) printf("done\n");
    }
}

void GameScreen::setBrushTileType(TILE t)
{
    brushTileType = t;
}

void GameScreen::setTileAt(int x, int y, TILE t)
{
    // check for OOB
    if (x < 0 || x > 199 || y < 0 || y > 199)
    {
        return;
    }

    // set the tile
    gridTiles[x][y] = t;

    // try increasing bounds
    if (t != TILE_NONE)
    {
        if (x < gridLeftBound)
        {
            gridLeftBound = x;
        }
        else if (x + 1 > gridRightBound)
        {
            gridRightBound = x + 1;
        }

        if (y < gridTopBound)
        {
            gridTopBound = y;
        }
        else if (y + 1 > gridBottomBound)
        {
            gridBottomBound = y + 1;
        }
    }
    else // try decreasing bounds
    {
        if (x == gridLeftBound)
        {
            bool b = true;
            while (b && gridLeftBound < 100)
            {
                for (int i = 0; i < 200; i++)
                {
                    if (gridTiles[gridLeftBound][i] != TILE_NONE)
                    {
                        b = false;
                    }
                }
                if (b)
                {
                    gridLeftBound++;
                }
            }
        }
        else if (x + 1 == gridRightBound)
        {
            bool b = true;
            while (b && gridRightBound > 100)
            {
                for (int i = 0; i < 200; i++)
                {
                    if (gridTiles[gridRightBound - 1][i] != TILE_NONE)
                    {
                        b = false;
                    }
                }
                if (b)
                {
                    gridRightBound--;
                }
            }
        }

        if (y == gridTopBound)
        {
            bool b = true;
            while (b && gridTopBound < 100)
            {
                for (int i = 0; i < 200; i++)
                {
                    if (gridTiles[i][gridTopBound] != TILE_NONE)
                    {
                        b = false;
                    }
                }
                if (b)
                {
                    gridTopBound++;
                }
            }
        }
        else if (y + 1 == gridBottomBound)
        {
            bool b = true;
            while (b && gridBottomBound > 100)
            {
                for (int i = 0; i < 200; i++)
                {
                    if (gridTiles[i][gridBottomBound - 1] != TILE_NONE)
                    {
                        b = false;
                    }
                }
                if (b)
                {
                    gridBottomBound--;
                }
            }
        }

        if (gridRightBound - gridLeftBound == 0 ||
                gridBottomBound - gridTopBound == 0)
        {
            clearLevel();
        }
    }

    checkShiftable();
}

void GameScreen::setBrushProgramType(PROGRAM p)
{
    brushProgramType = p;
    selectedProgram = NULL;
}

void GameScreen::setSelectedTile(int x, int y)
{
    selectedTileX = x;
    selectedTileY = y;
}

void GameScreen::setSelectedProgram(Program* p)
{
    if (p == selectedProgram) return;
    selectedProgram = p;
    for (int x = 0; x < 200; x++) for (int y = 0; y < 200; y++) gridProgramDist[x][y] = -1;
    if (p == NULL) return;
    else calculateProgramDist(p);
}

void GameScreen::setBrushMode(BRUSH b)
{
    brushMode = b;
}

void GameScreen::deleteProgram(Program* p)
{
    for (int i = 0; i < 200; i++)
    {
        for (int j = 0; j < 200; j++)
        {
            if (gridPrograms[i][j] == p)
            {
                gridPrograms[i][j] = NULL;
            }
        }
    }
    delete p;
}

void GameScreen::setBrushProgramTeam(int t)
{
    selectedProgram = NULL;
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
    bkgTex = b;
}

void GameScreen::checkShiftable()
{
    if ((gridRightBound - gridLeftBound)*32 < SCREEN_WIDTH - 200 &&
            (gridBottomBound - gridTopBound)*32 < SCREEN_HEIGHT - 200)
        canPanScreen = false;
    else canPanScreen = true;
}

Program* GameScreen::getSelectedProgram()
{
    return selectedProgram;
}

void GameScreen::calculateProgramDist(Program* p)
{
    int xH = p->getCoreX();
    int yH = p->getCoreY();

    gridProgramDist[xH][yH] = 0;
    LinkedList<Pair<int>*> ll = LinkedList<Pair<int>*>();
    ll.addFirst(new Pair<int>(xH, yH));

    // for each pair in the list
    while (ll.getLength() > 0)
    {
        Pair<int>* currPair = ll.poll();

        int xCurr = currPair->a;
        int yCurr = currPair->b;
        int dCurr = gridProgramDist[xCurr][yCurr];

        // if this pair is the edge of the program's range
        if (dCurr < p->getSpeed())
        {
            // check tile to the right
            if (xCurr + 1 < 200 &&
                    gridTiles[xCurr+1][yCurr] != TILE_NONE &&
                    (gridPrograms[xCurr+1][yCurr] == NULL || gridPrograms[xCurr+1][yCurr] == p) &&
                    gridProgramDist[xCurr+1][yCurr] == -1)
            {
                gridProgramDist[xCurr+1][yCurr] = dCurr + 1;
                ll.addLast(new Pair<int>(xCurr + 1, yCurr));
            }

            // check tile to the left
            if (xCurr - 1 >= 0 &&
                    gridTiles[xCurr-1][yCurr] != TILE_NONE &&
                    (gridPrograms[xCurr-1][yCurr] == NULL || gridPrograms[xCurr-1][yCurr] == p) &&
                    gridProgramDist[xCurr-1][yCurr] == -1)
            {
                gridProgramDist[xCurr-1][yCurr] = dCurr + 1;
                ll.addLast(new Pair<int>(xCurr - 1, yCurr));
            }

            // check tile below
            if (yCurr + 1 < 200 &&
                    gridTiles[xCurr][yCurr+1] != TILE_NONE &&
                    (gridPrograms[xCurr][yCurr+1] == NULL || gridPrograms[xCurr][yCurr+1] == p) &&
                    gridProgramDist[xCurr][yCurr+1] == -1)
            {
                gridProgramDist[xCurr][yCurr+1] = dCurr + 1;
                ll.addLast(new Pair<int>(xCurr, yCurr + 1));
            }

            // check tile above
            if (yCurr - 1 >= 0 &&
                    gridTiles[xCurr][yCurr-1] != TILE_NONE &&
                    (gridPrograms[xCurr][yCurr-1] == NULL || gridPrograms[xCurr][yCurr-1] == p) &&
                    gridProgramDist[xCurr][yCurr-1] == -1)
            {
                gridProgramDist[xCurr][yCurr-1] = dCurr + 1;
                ll.addLast(new Pair<int>(xCurr, yCurr - 1));
            }
        }
        delete currPair;
    }
}

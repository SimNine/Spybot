#include "Standard.h"
#include "GameScreen.h"

#include "Global.h"
#include "Client.h"
#include "Message.h"
#include "ResourceLoader.h"
#include "GUIButton.h"
#include "DataContainer.h"
#include "GUITexture.h"
#include "ProgramDisplayContainer.h"
#include "ProgramInventoryDisplay.h"
#include "PlayerDisplayContainer.h"
#include "Game.h"
#include "Player.h"
#include "Program.h"
#include "ProgramAction.h"
#include "MapScreen.h"
#include "Node.h"

GameScreen::GameScreen()
    : GUIContainer(ANCHOR_NORTHWEST, {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT}, NULL, loadTexture("resources/company_4.png"))
{
    buildGUI();

    editorMode_ = false;
    brushMode_ = BRUSH_NONE;
    brushTileType_ = TILE_NONE;
    brushProgramType_ = PROGRAM_BALLISTA;
    brushProgramTeam_ = 0;
    brushItemType_ = ITEM_NONE;
    programViewTeams_ = false;
	programViewPlayers_ = true;
    textureTickCount_ = 0;
    turnTickCount_ = 0;
    bkgPos_ = {0, 0};
    shiftSpeed_ = 0.1;
    canShiftScreen_ = true;
}

GameScreen::~GameScreen()
{
    //dtor
}

void GameScreen::buildGUI()
{
    int ln = 0;
    int col = 0;
    gridEditPanel_ = new GUIContainer(ANCHOR_NORTHWEST, {20, 20}, {8 + 32*12, 36}, this, NULL);
    GUIButton* emptyButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_NONE);
    },
    dataContainer->tile_images[TILE_NONE]);
    GUIButton* plainButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN);
    },
    dataContainer->tile_images[TILE_PLAIN]);
    GUIButton* plain2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN2);
    },
    dataContainer->tile_images[TILE_PLAIN2]);
    GUIButton* plain3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN3);
    },
    dataContainer->tile_images[TILE_PLAIN3]);
    GUIButton* plain4Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN4);
    },
    dataContainer->tile_images[TILE_PLAIN4]);
    GUIButton* plain5Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN5);
    },
    dataContainer->tile_images[TILE_PLAIN5]);
    GUIButton* plain6Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN6);
    },
    dataContainer->tile_images[TILE_PLAIN6]);
    GUIButton* plain7Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN7);
    },
    dataContainer->tile_images[TILE_PLAIN7]);
    GUIButton* plain8Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN8);
    },
    dataContainer->tile_images[TILE_PLAIN9]);
    GUIButton* plain9Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_PLAIN9);
    },
    dataContainer->tile_images[TILE_PLAIN9]);
    GUIButton* spawnButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                           []()
    {
        gameScreen->setBrushTileType(TILE_SPAWN);
    },
    dataContainer->tile_images[TILE_SPAWN]);
    GUIButton* spawn2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridEditPanel_,
                                            []()
    {
        gameScreen->setBrushTileType(TILE_SPAWN2);
    },
    dataContainer->tile_images[TILE_SPAWN2]);

    gridEditPanel_->addObject(emptyButton);
    gridEditPanel_->addObject(plainButton);
    gridEditPanel_->addObject(plain2Button);
    gridEditPanel_->addObject(plain3Button);
    gridEditPanel_->addObject(plain4Button);
    gridEditPanel_->addObject(plain5Button);
    gridEditPanel_->addObject(plain6Button);
    gridEditPanel_->addObject(plain7Button);
    gridEditPanel_->addObject(plain8Button);
    gridEditPanel_->addObject(plain9Button);
    gridEditPanel_->addObject(spawnButton);
    gridEditPanel_->addObject(spawn2Button);

    ln = 0;
    col = 0;
    gridProgramEditPanel_ = new GUIContainer(ANCHOR_NORTHWEST, {20, 60}, {32*15 + 8, 136}, this, NULL);
    GUIButton* ballistaButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BALLISTA);
    },
    dataContainer->program_icons[PROGRAM_BALLISTA],
    dataContainer->program_icons[PROGRAM_BALLISTA],
    dataContainer->program_icons[PROGRAM_BALLISTA]);
    gridProgramEditPanel_->addObject(ballistaButton);
    GUIButton* bitmanButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BITMAN);
    },
    dataContainer->program_icons[PROGRAM_BITMAN],
    dataContainer->program_icons[PROGRAM_BITMAN],
    dataContainer->program_icons[PROGRAM_BITMAN]);
    gridProgramEditPanel_->addObject(bitmanButton);
    GUIButton* bitman2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BITMAN2);
    },
    dataContainer->program_icons[PROGRAM_BITMAN2],
    dataContainer->program_icons[PROGRAM_BITMAN2],
    dataContainer->program_icons[PROGRAM_BITMAN2]);
    gridProgramEditPanel_->addObject(bitman2Button);
    GUIButton* blackwidowButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BLACKWIDOW);
    },
    dataContainer->program_icons[PROGRAM_BLACKWIDOW],
    dataContainer->program_icons[PROGRAM_BLACKWIDOW],
    dataContainer->program_icons[PROGRAM_BLACKWIDOW]);
    gridProgramEditPanel_->addObject(blackwidowButton);
    GUIButton* bossButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BOSS);
    },
    dataContainer->program_icons[PROGRAM_BOSS],
    dataContainer->program_icons[PROGRAM_BOSS],
    dataContainer->program_icons[PROGRAM_BOSS]);
    gridProgramEditPanel_->addObject(bossButton);
    GUIButton* bugButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                         []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG);
    },
    dataContainer->program_icons[PROGRAM_BUG],
    dataContainer->program_icons[PROGRAM_BUG],
    dataContainer->program_icons[PROGRAM_BUG]);
    gridProgramEditPanel_->addObject(bugButton);
    GUIButton* bug2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG2);
    },
    dataContainer->program_icons[PROGRAM_BUG2],
    dataContainer->program_icons[PROGRAM_BUG2],
    dataContainer->program_icons[PROGRAM_BUG2]);
    gridProgramEditPanel_->addObject(bug2Button);
    GUIButton* bug3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_BUG3);
    },
    dataContainer->program_icons[PROGRAM_BUG3],
    dataContainer->program_icons[PROGRAM_BUG3],
    dataContainer->program_icons[PROGRAM_BUG3]);
    gridProgramEditPanel_->addObject(bug3Button);
    GUIButton* catapultButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CATAPULT);
    },
    dataContainer->program_icons[PROGRAM_CATAPULT],
    dataContainer->program_icons[PROGRAM_CATAPULT],
    dataContainer->program_icons[PROGRAM_CATAPULT]);
    gridProgramEditPanel_->addObject(catapultButton);
    GUIButton* clogButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG);
    },
    dataContainer->program_icons[PROGRAM_CLOG],
    dataContainer->program_icons[PROGRAM_CLOG],
    dataContainer->program_icons[PROGRAM_CLOG]);
    gridProgramEditPanel_->addObject(clogButton);
    GUIButton* clog2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG2);
    },
    dataContainer->program_icons[PROGRAM_CLOG2],
    dataContainer->program_icons[PROGRAM_CLOG2],
    dataContainer->program_icons[PROGRAM_CLOG2]);
    gridProgramEditPanel_->addObject(clog2Button);
    GUIButton* clog3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_CLOG3);
    },
    dataContainer->program_icons[PROGRAM_CLOG3],
    dataContainer->program_icons[PROGRAM_CLOG3],
    dataContainer->program_icons[PROGRAM_CLOG3]);
    gridProgramEditPanel_->addObject(clog3Button);
    GUIButton* databombButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DATABOMB);
    },
    dataContainer->program_icons[PROGRAM_DATABOMB],
    dataContainer->program_icons[PROGRAM_DATABOMB],
    dataContainer->program_icons[PROGRAM_DATABOMB]);
    gridProgramEditPanel_->addObject(databombButton);
    GUIButton* datadoctorButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DATADOCTOR);
    },
    dataContainer->program_icons[PROGRAM_DATADOCTOR],
    dataContainer->program_icons[PROGRAM_DATADOCTOR],
    dataContainer->program_icons[PROGRAM_DATADOCTOR]);
    gridProgramEditPanel_->addObject(datadoctorButton);
    ln++;
    col = 0;
    GUIButton* datadoctor2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DATADOCTOR2);
    },
    dataContainer->program_icons[PROGRAM_DATADOCTOR2],
    dataContainer->program_icons[PROGRAM_DATADOCTOR2],
    dataContainer->program_icons[PROGRAM_DATADOCTOR2]);
    gridProgramEditPanel_->addObject(datadoctor2Button);
    GUIButton* dogButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                         []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG);
    },
    dataContainer->program_icons[PROGRAM_DOG],
    dataContainer->program_icons[PROGRAM_DOG],
    dataContainer->program_icons[PROGRAM_DOG]);
    gridProgramEditPanel_->addObject(dogButton);
    GUIButton* dog2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG2);
    },
    dataContainer->program_icons[PROGRAM_DOG2],
    dataContainer->program_icons[PROGRAM_DOG2],
    dataContainer->program_icons[PROGRAM_DOG2]);
    gridProgramEditPanel_->addObject(dog2Button);
    GUIButton* dog3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_DOG3);
    },
    dataContainer->program_icons[PROGRAM_DOG3],
    dataContainer->program_icons[PROGRAM_DOG3],
    dataContainer->program_icons[PROGRAM_DOG3]);
    gridProgramEditPanel_->addObject(dog3Button);
    GUIButton* fiddleButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_FIDDLE);
    },
    dataContainer->program_icons[PROGRAM_FIDDLE],
    dataContainer->program_icons[PROGRAM_FIDDLE],
    dataContainer->program_icons[PROGRAM_FIDDLE]);
    gridProgramEditPanel_->addObject(fiddleButton);
    GUIButton* firewallButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_FIREWALL);
    },
    dataContainer->program_icons[PROGRAM_FIREWALL],
    dataContainer->program_icons[PROGRAM_FIREWALL],
    dataContainer->program_icons[PROGRAM_FIREWALL]);
    gridProgramEditPanel_->addObject(firewallButton);
    GUIButton* golemButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM);
    },
    dataContainer->program_icons[PROGRAM_GOLEM],
    dataContainer->program_icons[PROGRAM_GOLEM],
    dataContainer->program_icons[PROGRAM_GOLEM]);
    gridProgramEditPanel_->addObject(golemButton);
    GUIButton* golem2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM2);
    },
    dataContainer->program_icons[PROGRAM_GOLEM2],
    dataContainer->program_icons[PROGRAM_GOLEM2],
    dataContainer->program_icons[PROGRAM_GOLEM2]);
    gridProgramEditPanel_->addObject(golem2Button);
    GUIButton* golem3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_GOLEM3);
    },
    dataContainer->program_icons[PROGRAM_GOLEM3],
    dataContainer->program_icons[PROGRAM_GOLEM3],
    dataContainer->program_icons[PROGRAM_GOLEM3]);
    gridProgramEditPanel_->addObject(golem3Button);
    GUIButton* hackButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK);
    },
    dataContainer->program_icons[PROGRAM_HACK],
    dataContainer->program_icons[PROGRAM_HACK],
    dataContainer->program_icons[PROGRAM_HACK]);
    gridProgramEditPanel_->addObject(hackButton);
    GUIButton* hack2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK2);
    },
    dataContainer->program_icons[PROGRAM_HACK2],
    dataContainer->program_icons[PROGRAM_HACK2],
    dataContainer->program_icons[PROGRAM_HACK2]);
    gridProgramEditPanel_->addObject(hack2Button);
    GUIButton* hack3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_HACK3);
    },
    dataContainer->program_icons[PROGRAM_HACK3],
    dataContainer->program_icons[PROGRAM_HACK3],
    dataContainer->program_icons[PROGRAM_HACK3]);
    gridProgramEditPanel_->addObject(hack3Button);
    GUIButton* kamikazeeButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_KAMIKAZEE);
    },
    dataContainer->program_icons[PROGRAM_KAMIKAZEE],
    dataContainer->program_icons[PROGRAM_KAMIKAZEE],
    dataContainer->program_icons[PROGRAM_KAMIKAZEE]);
    gridProgramEditPanel_->addObject(kamikazeeButton);
    GUIButton* medicButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_MEDIC);
    },
    dataContainer->program_icons[PROGRAM_MEDIC],
    dataContainer->program_icons[PROGRAM_MEDIC],
    dataContainer->program_icons[PROGRAM_MEDIC]);
    gridProgramEditPanel_->addObject(medicButton);
    GUIButton* memhogButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_MEMHOG);
    },
    dataContainer->program_icons[PROGRAM_MEMHOG],
    dataContainer->program_icons[PROGRAM_MEMHOG],
    dataContainer->program_icons[PROGRAM_MEMHOG]);
    gridProgramEditPanel_->addObject(memhogButton);
    col = 0;
    ln++;
    GUIButton* mobiletowerButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_MOBILETOWER);
    },
    dataContainer->program_icons[PROGRAM_MOBILETOWER],
    dataContainer->program_icons[PROGRAM_MOBILETOWER],
    dataContainer->program_icons[PROGRAM_MOBILETOWER]);
    gridProgramEditPanel_->addObject(mobiletowerButton);
    GUIButton* satelliteButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SATELLITE);
    },
    dataContainer->program_icons[PROGRAM_SATELLITE],
    dataContainer->program_icons[PROGRAM_SATELLITE],
    dataContainer->program_icons[PROGRAM_SATELLITE]);
    gridProgramEditPanel_->addObject(satelliteButton);
    GUIButton* satellite2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SATELLITE2);
    },
    dataContainer->program_icons[PROGRAM_SATELLITE2],
    dataContainer->program_icons[PROGRAM_SATELLITE2],
    dataContainer->program_icons[PROGRAM_SATELLITE2]);
    gridProgramEditPanel_->addObject(satellite2Button);
    GUIButton* seekerButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER);
    },
    dataContainer->program_icons[PROGRAM_SEEKER],
    dataContainer->program_icons[PROGRAM_SEEKER],
    dataContainer->program_icons[PROGRAM_SEEKER]);
    gridProgramEditPanel_->addObject(seekerButton);
    GUIButton* seeker2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER2);
    },
    dataContainer->program_icons[PROGRAM_SEEKER2],
    dataContainer->program_icons[PROGRAM_SEEKER2],
    dataContainer->program_icons[PROGRAM_SEEKER2]);
    gridProgramEditPanel_->addObject(seeker2Button);
    GUIButton* seeker3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SEEKER3);
    },
    dataContainer->program_icons[PROGRAM_SEEKER3],
    dataContainer->program_icons[PROGRAM_SEEKER3],
    dataContainer->program_icons[PROGRAM_SEEKER3]);
    gridProgramEditPanel_->addObject(seeker3Button);
    GUIButton* slingshotButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SLINGSHOT);
    },
    dataContainer->program_icons[PROGRAM_SLINGSHOT],
    dataContainer->program_icons[PROGRAM_SLINGSHOT],
    dataContainer->program_icons[PROGRAM_SLINGSHOT]);
    gridProgramEditPanel_->addObject(slingshotButton);
    GUIButton* sonarButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR);
    },
    dataContainer->program_icons[PROGRAM_SONAR],
    dataContainer->program_icons[PROGRAM_SONAR],
    dataContainer->program_icons[PROGRAM_SONAR]);
    gridProgramEditPanel_->addObject(sonarButton);
    GUIButton* sonar2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR2);
    },
    dataContainer->program_icons[PROGRAM_SONAR2],
    dataContainer->program_icons[PROGRAM_SONAR2],
    dataContainer->program_icons[PROGRAM_SONAR2]);
    gridProgramEditPanel_->addObject(sonar2Button);
    GUIButton* sonar3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SONAR3);
    },
    dataContainer->program_icons[PROGRAM_SONAR3],
    dataContainer->program_icons[PROGRAM_SONAR3],
    dataContainer->program_icons[PROGRAM_SONAR3]);
    gridProgramEditPanel_->addObject(sonar3Button);
    GUIButton* specsButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SPECS);
    },
    dataContainer->program_icons[PROGRAM_SPECS],
    dataContainer->program_icons[PROGRAM_SPECS],
    dataContainer->program_icons[PROGRAM_SPECS]);
    gridProgramEditPanel_->addObject(specsButton);
    GUIButton* sumoButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                          []()
    {
        gameScreen->setBrushProgramType(PROGRAM_SUMO);
    },
    dataContainer->program_icons[PROGRAM_SUMO],
    dataContainer->program_icons[PROGRAM_SUMO],
    dataContainer->program_icons[PROGRAM_SUMO]);
    gridProgramEditPanel_->addObject(sumoButton);
    GUIButton* tarantulaButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TARANTULA);
    },
    dataContainer->program_icons[PROGRAM_TARANTULA],
    dataContainer->program_icons[PROGRAM_TARANTULA],
    dataContainer->program_icons[PROGRAM_TARANTULA]);
    gridProgramEditPanel_->addObject(tarantulaButton);
    GUIButton* towerButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TOWER);
    },
    dataContainer->program_icons[PROGRAM_TOWER],
    dataContainer->program_icons[PROGRAM_TOWER],
    dataContainer->program_icons[PROGRAM_TOWER]);
    gridProgramEditPanel_->addObject(towerButton);
    GUIButton* turboButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                           []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TURBO);
    },
    dataContainer->program_icons[PROGRAM_TURBO],
    dataContainer->program_icons[PROGRAM_TURBO],
    dataContainer->program_icons[PROGRAM_TURBO]);
    gridProgramEditPanel_->addObject(turboButton);
    col = 0;
    ln++;
    GUIButton* turbo2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TURBO2);
    },
    dataContainer->program_icons[PROGRAM_TURBO2],
    dataContainer->program_icons[PROGRAM_TURBO2],
    dataContainer->program_icons[PROGRAM_TURBO2]);
    gridProgramEditPanel_->addObject(turbo2Button);
    GUIButton* turbo3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_TURBO3);
    },
    dataContainer->program_icons[PROGRAM_TURBO3],
    dataContainer->program_icons[PROGRAM_TURBO3],
    dataContainer->program_icons[PROGRAM_TURBO3]);
    gridProgramEditPanel_->addObject(turbo3Button);
    GUIButton* walkerButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER);
    },
    dataContainer->program_icons[PROGRAM_WALKER],
    dataContainer->program_icons[PROGRAM_WALKER],
    dataContainer->program_icons[PROGRAM_WALKER]);
    gridProgramEditPanel_->addObject(walkerButton);
    GUIButton* walker2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER2);
    },
    dataContainer->program_icons[PROGRAM_WALKER2],
    dataContainer->program_icons[PROGRAM_WALKER2],
    dataContainer->program_icons[PROGRAM_WALKER2]);
    gridProgramEditPanel_->addObject(walker2Button);
    GUIButton* walker3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WALKER3);
    },
    dataContainer->program_icons[PROGRAM_WALKER3],
    dataContainer->program_icons[PROGRAM_WALKER3],
    dataContainer->program_icons[PROGRAM_WALKER3]);
    gridProgramEditPanel_->addObject(walker3Button);
    GUIButton* wardenButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN);
    },
    dataContainer->program_icons[PROGRAM_WARDEN],
    dataContainer->program_icons[PROGRAM_WARDEN],
    dataContainer->program_icons[PROGRAM_WARDEN]);
    gridProgramEditPanel_->addObject(wardenButton);
    GUIButton* warden2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN2);
    },
    dataContainer->program_icons[PROGRAM_WARDEN2],
    dataContainer->program_icons[PROGRAM_WARDEN2],
    dataContainer->program_icons[PROGRAM_WARDEN2]);
    gridProgramEditPanel_->addObject(warden2Button);
    GUIButton* warden3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WARDEN3);
    },
    dataContainer->program_icons[PROGRAM_WARDEN3],
    dataContainer->program_icons[PROGRAM_WARDEN3],
    dataContainer->program_icons[PROGRAM_WARDEN3]);
    gridProgramEditPanel_->addObject(warden3Button);
    GUIButton* watchmanButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN],
    dataContainer->program_icons[PROGRAM_WATCHMAN],
    dataContainer->program_icons[PROGRAM_WATCHMAN]);
    gridProgramEditPanel_->addObject(watchmanButton);
    GUIButton* watchman2Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN2);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN2],
    dataContainer->program_icons[PROGRAM_WATCHMAN2],
    dataContainer->program_icons[PROGRAM_WATCHMAN2]);
    gridProgramEditPanel_->addObject(watchman2Button);
    GUIButton* watchman3Button = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WATCHMAN3);
    },
    dataContainer->program_icons[PROGRAM_WATCHMAN3],
    dataContainer->program_icons[PROGRAM_WATCHMAN3],
    dataContainer->program_icons[PROGRAM_WATCHMAN3]);
    gridProgramEditPanel_->addObject(watchman3Button);
    GUIButton* wizardButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
                                            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WIZARD);
    },
    dataContainer->program_icons[PROGRAM_WIZARD],
    dataContainer->program_icons[PROGRAM_WIZARD],
    dataContainer->program_icons[PROGRAM_WIZARD]);
    gridProgramEditPanel_->addObject(wizardButton);
    GUIButton* wolfspiderButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4 + 32*ln}, {28, 28}, gridProgramEditPanel_,
            []()
    {
        gameScreen->setBrushProgramType(PROGRAM_WOLFSPIDER);
    },
    dataContainer->program_icons[PROGRAM_WOLFSPIDER],
    dataContainer->program_icons[PROGRAM_WOLFSPIDER],
    dataContainer->program_icons[PROGRAM_WOLFSPIDER]);
    gridProgramEditPanel_->addObject(wolfspiderButton);

    ln = 0;
    col = 0;
    gridItemEditPanel_ = new GUIContainer(ANCHOR_NORTHWEST, {20, 200}, {4 + 36*4, 36}, this, NULL);
    GUIButton* creditButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridItemEditPanel_,
                                            []()
    {
        gameScreen->setBrushItem(ITEM_CREDIT);
    },
    dataContainer->item_icons[ITEM_CREDIT],
    dataContainer->item_icons[ITEM_CREDIT],
    dataContainer->item_icons[ITEM_CREDIT]);
    GUIButton* bigCreditButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridItemEditPanel_,
            []()
    {
        gameScreen->setBrushItem(ITEM_BIGCREDIT);
    },
    dataContainer->item_icons[ITEM_BIGCREDIT],
    dataContainer->item_icons[ITEM_BIGCREDIT],
    dataContainer->item_icons[ITEM_BIGCREDIT]);
    GUIButton* filesButton = new GUIButton(ANCHOR_NORTHWEST, {4 + 32*col++, 4}, {28, 28}, gridItemEditPanel_,
                                           []()
    {
        gameScreen->setBrushItem(ITEM_FILES);
    },
    dataContainer->item_icons[ITEM_FILES],
    dataContainer->item_icons[ITEM_FILES],
    dataContainer->item_icons[ITEM_FILES]);

    gridItemEditPanel_->addObject(creditButton);
    gridItemEditPanel_->addObject(bigCreditButton);
    gridItemEditPanel_->addObject(filesButton);

    col = 0;
    gridSelectBrushPanel_ = new GUIContainer(ANCHOR_NORTHWEST, {20, 240}, {100, 200}, this, NULL);
    GUIButton* brushNoneButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {45, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->setBrushMode(BRUSH_NONE);
    },
    dataContainer->game_editor_brush_none);
    GUIButton* brushTileButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {45, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->setBrushMode(BRUSH_TILES);
    },
    dataContainer->game_editor_brush_tiles);
    GUIButton* brushProgramButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->setBrushMode(BRUSH_PROGRAMS);
    },
    dataContainer->game_editor_brush_programs);
    GUIButton* brushClearProgramButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->setBrushMode(BRUSH_DELETEPROGRAMS);
    },
    dataContainer->game_editor_brush_deletePrograms);
    GUIButton* brushItemButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->setBrushMode(BRUSH_ITEMS);
    },
    dataContainer->game_editor_brush_items);
    col++;
    GUIButton* brushTeamPlayerButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->setBrushProgramTeam(0);
    },
    dataContainer->game_editor_toggle_teamPlayer);
    GUIButton* brushTeamComputerButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->setBrushProgramTeam(1);
    },
    dataContainer->game_editor_toggle_teamComputer);
    GUIButton* brushTeamViewButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {73, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->toggleViewTeams();
    },
    dataContainer->game_editor_toggle_teamView);
    GUIButton* brushClearGridButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {44, 12}, gridSelectBrushPanel_,
            []()
    {
        gameScreen->resetScreen();
    },
    dataContainer->game_editor_button_clearGrid);

    gridSelectBrushPanel_->addObject(brushNoneButton);
    gridSelectBrushPanel_->addObject(brushTileButton);
    gridSelectBrushPanel_->addObject(brushProgramButton);
    gridSelectBrushPanel_->addObject(brushClearProgramButton);
    gridSelectBrushPanel_->addObject(brushItemButton);
    gridSelectBrushPanel_->addObject(brushTeamPlayerButton);
    gridSelectBrushPanel_->addObject(brushTeamComputerButton);
    gridSelectBrushPanel_->addObject(brushTeamViewButton);
    gridSelectBrushPanel_->addObject(brushClearGridButton);

    col = 0;
    gridBkgPanel_ = new GUIContainer(ANCHOR_NORTHWEST, {124, 240}, {100, 200}, this, NULL);
    GUIButton* bkgDonutButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel_,
            []()
    {
        gameScreen->setBackgroundImg(BKG_DONUT);
    },
    dataContainer->game_editor_button_bkg[BKG_DONUT],
    dataContainer->game_editor_button_bkg[BKG_DONUT],
    dataContainer->game_editor_button_bkg[BKG_DONUT]);
    gridBkgPanel_->addObject(bkgDonutButton);
    GUIButton* bkgPharmButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel_,
            []()
    {
        gameScreen->setBackgroundImg(BKG_PHARM);
    },
    dataContainer->game_editor_button_bkg[BKG_PHARM],
    dataContainer->game_editor_button_bkg[BKG_PHARM],
    dataContainer->game_editor_button_bkg[BKG_PHARM]);
    gridBkgPanel_->addObject(bkgPharmButton);
    GUIButton* bkgPedButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel_,
                                            []()
    {
        gameScreen->setBackgroundImg(BKG_PED);
    },
    dataContainer->game_editor_button_bkg[BKG_PED],
    dataContainer->game_editor_button_bkg[BKG_PED],
    dataContainer->game_editor_button_bkg[BKG_PED]);
    gridBkgPanel_->addObject(bkgPedButton);
    GUIButton* bkgMonkeyButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel_,
            []()
    {
        gameScreen->setBackgroundImg(BKG_MONKEY);
    },
    dataContainer->game_editor_button_bkg[BKG_MONKEY],
    dataContainer->game_editor_button_bkg[BKG_MONKEY],
    dataContainer->game_editor_button_bkg[BKG_MONKEY]);
    gridBkgPanel_->addObject(bkgMonkeyButton);
    GUIButton* bkgCellButton = new GUIButton(ANCHOR_NORTHWEST, {4, 4 + 16*col++}, {60, 12}, gridBkgPanel_,
            []()
    {
        gameScreen->setBackgroundImg(BKG_CELL);
    },
    dataContainer->game_editor_button_bkg[BKG_CELL],
    dataContainer->game_editor_button_bkg[BKG_CELL],
    dataContainer->game_editor_button_bkg[BKG_CELL]);
    gridBkgPanel_->addObject(bkgCellButton);

    // turn display items
    playerTurn_ = new GUITexture(ANCHOR_NORTH, {-200, 50},
                                dataContainer->game_disp_playerturn,
                                {400, 100}, this);
    playerTurn_->setTransparency(0);
    addObject(playerTurn_);
    aiTurn_ = new GUITexture(ANCHOR_NORTH, {-200, 50},
                            dataContainer->game_disp_compturn,
                            {400, 100}, this);
    aiTurn_->setTransparency(0);
    addObject(aiTurn_);

    // debug options
    debugOptions_ = new GUIContainer(ANCHOR_NORTHWEST, {10, 10}, {220, 190}, this, {120, 120, 120, 140});
    GUIButton* teamViewButton = new GUIButton(ANCHOR_NORTHWEST, {10, 10}, {200, 50}, debugOptions_,
                                   [](){gameScreen->toggleViewTeams();},
                                   dataContainer->game_button_viewTeams);
    debugOptions_->addObject(teamViewButton);
    GUIButton* winGameButton = new GUIButton(ANCHOR_NORTHWEST, {10, 70}, {200, 50}, debugOptions_,
		[]() {Message m; m.type = MSGTYPE_INFO; m.infoType = MSGINFOTYPE_GAMESTATUS; m.statusType = GAMESTATUS_WON; client->sendMessage(m); },
                                  dataContainer->game_button_win);
    debugOptions_->addObject(winGameButton);
    GUIButton* stepButton = new GUIButton(ANCHOR_NORTHWEST, {10, 130}, {200, 50}, debugOptions_,
                               [](){Message msg; msg.type = MSGTYPE_AISTEP;	client->sendMessage(msg);},
                               dataContainer->game_button_aiStep);
    debugOptions_->addObject(stepButton);
    debugOptions_->setTransparency(0);
    addObject(debugOptions_);

    // pre-game options
    preGameOptions_ = new GUIContainer(ANCHOR_SOUTHWEST, {10, -150}, {220, 130}, this, {120, 120, 120, 140});
    GUIButton* startGameButton = new GUIButton(ANCHOR_NORTHWEST, {10, 10}, {200, 50}, preGameOptions_,
                                    [](){Message m; m.type = MSGTYPE_INFO; m.infoType = MSGINFOTYPE_GAMESTATUS; 
									m.statusType = GAMESTATUS_PLAYING; client->sendMessage(m);},
                                    dataContainer->game_button_start);
    preGameOptions_->addObject(startGameButton);
    GUIButton* backToMapButton = new GUIButton(ANCHOR_NORTHWEST, {10, 70}, {200, 50}, preGameOptions_,
                                    [](){Message m; m.type = MSGTYPE_LEAVE; client->sendMessage(m);},
                                    dataContainer->game_button_quitToMap);
    preGameOptions_->addObject(backToMapButton);
    addObject(preGameOptions_);

    // pause menu
    pauseMenu_ = new GUIContainer(ANCHOR_CENTER, {-110, -200}, {220, 5*60 + 10}, this, {120, 120, 120, 140});
    GUIButton* resumeGameButton = new GUIButton(ANCHOR_NORTHWEST, {10, 10}, {200, 50}, pauseMenu_,
                                                [](){gameScreen->togglePauseMenu();},
                                                dataContainer->game_button_resume);
    pauseMenu_->addObject(resumeGameButton);
    /*GUIButton* resetGameButton = new GUIButton(ANCHOR_NORTHWEST, {10, 70}, {200, 50}, pauseMenu_,
                                    [](){client->loadGame(mapScreen->getSelectedNode()->getLevelId());},
                                    dataContainer->game_button_reset);
    pauseMenu_->addObject(resetGameButton);*/
    GUIButton* exitToMapButton = new GUIButton(ANCHOR_NORTHWEST, {10, 130}, {200, 50}, pauseMenu_,
                                      [](){Message m; m.type = MSGTYPE_LEAVE; client->sendMessage(m);},
                                      dataContainer->game_button_quitToMap);
    pauseMenu_->addObject(exitToMapButton);
    GUIButton* exitToMainButton = new GUIButton(ANCHOR_NORTHWEST, {10, 190}, {200, 50}, pauseMenu_,
                                                [](){Message m; m.type = MSGTYPE_LEAVE; client->sendMessage(m);},
                                                dataContainer->game_button_quitToMain);
    pauseMenu_->addObject(exitToMainButton);
    GUIButton* exitToDesktopButton = new GUIButton(ANCHOR_NORTHWEST, {10, 250}, {200, 50}, pauseMenu_,
                                                   [](){quit = true;},
                                                   dataContainer->game_button_quitToDesktop);
    pauseMenu_->addObject(exitToDesktopButton);
    pauseMenu_->setTransparency(0);
    pauseMenu_->setMovable(false);
    addObject(pauseMenu_);

    // end turn button
    turnButton_ = new GUIButton(ANCHOR_SOUTHEAST, {-220, -60}, {200, 50}, this,
                               [](){Message msg; msg.type = MSGTYPE_NEXTTURN; client->sendMessage(msg);},
                               dataContainer->game_button_endTurn);
    turnButton_->setTransparency(0);
    addObject(turnButton_);

    // program display window
    progDisp_ = new ProgramDisplayContainer(ANCHOR_SOUTHWEST, {10, -210}, {200, 200}, this);
    progDisp_->setTransparency(0);
    addObject(progDisp_);

	// player display window
	playerDisp_ = new PlayerDisplayContainer(ANCHOR_NORTHWEST, { 10, 10 }, { 10, 10 }, this);
	playerDisp_->setTransparency(255);
	addObject(playerDisp_);

    // add the program inventory display
    progInv_ = new ProgramInventoryDisplay(ANCHOR_NORTHEAST, {0, 0}, {0, 0}, this);
    addObject(progInv_);
}

void GameScreen::resetBounds()
{
    GUIContainer::resetBounds();
    checkShiftable();
    if (!canShiftScreen_)
        centerScreen();
}

bool GameScreen::mouseDown()
{
    if (pauseMenu_->isVisible())
        return pauseMenu_->mouseDown();

    if (GUIContainer::mouseDown())
        return true;

    if (editorMode_)
    {
        if (gridEditPanel_->isMouseOver()) // click the tile pallette
        {
            return gridEditPanel_->mouseDown();
        }
        else if (gridProgramEditPanel_->isMouseOver()) // click the program pallette
        {
            return gridProgramEditPanel_->mouseDown();
        }
        else if (gridSelectBrushPanel_->isMouseOver())
        {
            return gridSelectBrushPanel_->mouseDown();
        }
        else if (gridItemEditPanel_->isMouseOver())
        {
            return gridItemEditPanel_->mouseDown();
        }
        else if (gridBkgPanel_->isMouseOver())
        {
            return gridBkgPanel_->mouseDown();
        }
        else // click the grid
        {
            // find the clicked tile
            Coord click;
            click.x = (bkgPos_.x + mousePos.x)/TILE_WIDTH;
            click.y = (bkgPos_.y + mousePos.y)/TILE_WIDTH;

            if (brushMode_ == BRUSH_PROGRAMS)
            {
                // TODO
            }
            else if (brushMode_ == BRUSH_TILES)
            {
                //client->getGame()->setTileAt(click, brushTileType_);
            }
            else if (brushMode_ == BRUSH_DELETEPROGRAMS)
            {
                //client->getGame()->setProgramAt(click, NULL);
            }
            else if (brushMode_ == BRUSH_ITEMS)
            {
				//client->getGame()->setItemAt(click, brushItemType_);
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
        Coord click = {(bkgPos_.x + mousePos.x)/TILE_WIDTH, (bkgPos_.y + mousePos.y)/TILE_WIDTH};

        if (client->getGame()->getStatus() == GAMESTATUS_PLACING_PROGRAMS)
        {
            Message msg;
            msg.type = MSGTYPE_SELECT;
            msg.pos = click;
            msg.selectType = MSGSELECTTYPE_TILE;
			client->sendMessage(msg);
        }
        else if (client->getGame()->getStatus() == GAMESTATUS_PLAYING)
        {
            if ((client->getPlayer()->getSelectedProgram() == NULL || !client->getPlayer()->canSelectedProgramMoveTo(click)) &&
                (client->getPlayer()->getSelectedAction() == NULL || !(client->getPlayer()->getSelectedActionDist(click) > 0)))
            {
                Message msg;
                msg.type = MSGTYPE_SELECT;
                msg.pos = click;
                msg.selectType = MSGSELECTTYPE_TILE;
				client->sendMessage(msg);
            }
            else //if (game->getPlayer()->canSelectedProgramMoveTo(x, y))
            {
                if (client->getPlayer()->getSelectedAction() == NULL)
                {
                    Message msg;
                    msg.type = MSGTYPE_MOVE;
                    msg.pos = click;
					msg.playerID = client->getPlayer()->getPlayerID();
					msg.programID = client->getPlayer()->getSelectedProgram()->getProgramID();
					client->sendMessage(msg);
                }
                else if (client->getPlayer()->getSelectedActionDist(click) > 0)
                {
                    if (debug >= DEBUG_NORMAL)
                        printf("player using move\n");

                    Message msg;
                    msg.type = MSGTYPE_ACTION;
                    msg.pos = click;
					client->sendMessage(msg);
                }
            }
        }

        return false;
    }
}

bool GameScreen::mouseUp()
{
    if (pauseMenu_->isVisible())
        return pauseMenu_->mouseUp();

    GUIContainer::mouseUp();

    bool r = false;

    if (editorMode_)
    {
        if (gridEditPanel_->isMouseOver()) // click the tile pallette
        {
            r = gridEditPanel_->mouseUp();
        }
        else if (gridProgramEditPanel_->isMouseOver()) // click the program pallette
        {
            r = gridProgramEditPanel_->mouseUp();
        }
        else if (gridSelectBrushPanel_->isMouseOver())
        {
            r = gridSelectBrushPanel_->mouseUp();
        }
        else if (gridItemEditPanel_->isMouseOver())
        {
            r = gridItemEditPanel_->mouseUp();
        }
        else if (gridBkgPanel_->isMouseOver())
        {
            r = gridBkgPanel_->mouseUp();
        }

        gridEditPanel_->setPressed(false);
        gridProgramEditPanel_->setPressed(false);
        gridSelectBrushPanel_->setPressed(false);
        gridItemEditPanel_->setPressed(false);
        gridBkgPanel_->setPressed(false);
    }

    return r;
}

void GameScreen::endTurn()
{
    aiTurn_->fade(255, 500);
}

void GameScreen::resumeTurn()
{
	playerTurn_->fade(255, 500);
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
    Coord topLeft = {bkgPos_.x / TILE_WIDTH, bkgPos_.y / TILE_WIDTH};
    Coord bottomRight;
    bottomRight.x = topLeft.x + SCREEN_WIDTH / TILE_WIDTH + 1;
    bottomRight.y = topLeft.y + SCREEN_HEIGHT / TILE_WIDTH + 1;

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
            if (client->getGame()->getTileAt(curr) == TILE_NONE && !editorMode_)
                continue;

            // default position of a tile,
            // assuming it is 28x28 px
            int xDefault = 4 + TILE_WIDTH*curr.x - bkgPos_.x;
            int yDefault = 4 + TILE_WIDTH*curr.y - bkgPos_.y;
            int sizeDefault = TILE_WIDTH - 4;
            tileRect.x = xDefault;
            tileRect.y = yDefault;
            tileRect.w = sizeDefault;
            tileRect.h = sizeDefault;

            // if there's a program at this tile
            if (client->getGame()->getProgramAt(curr) != NULL)
            {
                // get this program
                Program* prog = client->getGame()->getProgramAt(curr);

                // draw this program's tile
                tileRect.x = xDefault - 1;
                tileRect.y = yDefault - 1;
                tileRect.w = sizeDefault + 2;
                tileRect.h = sizeDefault + 2;
				if (programViewPlayers_)
				{
					SDL_Color c = prog->getOwner()->getColor();
					SDL_SetTextureColorMod(dataContainer->program_core, c.r, c.g, c.b);
				}
                else if (programViewTeams_)
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

                if (client->getPlayer() == NULL)
                    continue;

                // if this is the farthest chunk of this program
                if (prog == client->getPlayer()->getSelectedProgram() &&
                    prog->getHealth() == prog->getMaxHealth() &&
                    prog->getMoves() > 0)
                {
                    Coord currTail = prog->getTail();
                    if (curr.x == currTail.x && curr.y == currTail.y)
                    {
                        if (textureTickCount_ % 100 < 50)
                            SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);
                    }
                    else
                        SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);
                }
                else SDL_RenderCopy(gRenderer, dataContainer->program_core, NULL, &tileRect);

                // if this is part of the selected program, indicate it
                if (prog == client->getPlayer()->getSelectedProgram() && debug >= DEBUG_NORMAL)
                {
                    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 0);
                    SDL_RenderDrawLine(gRenderer, tileRect.x, tileRect.y, tileRect.x + tileRect.w, tileRect.y + tileRect.h);
                    SDL_RenderDrawLine(gRenderer, tileRect.x + tileRect.w, tileRect.y, tileRect.x, tileRect.y + tileRect.h);
                }

                // draw the bridges from this program's tile to adjacent tiles
                if (client->getGame()->getProgramAt({curr.x, curr.y+1}) == prog)
                {
                    tileRect.x = xDefault + 9;
                    tileRect.y = yDefault + 26;
                    tileRect.w = 10;
                    tileRect.h = 5;
					if (programViewPlayers_)
					{
						SDL_Color c = prog->getOwner()->getColor();
						SDL_SetTextureColorMod(dataContainer->program_core_vertical, c.r, c.g, c.b);
					}
                    else if (programViewTeams_)
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
                if (client->getGame()->getProgramAt({curr.x+1, curr.y}) == prog)
                {
                    tileRect.x = xDefault + 26;
                    tileRect.y = yDefault + 8;
                    tileRect.w = 5;
                    tileRect.h = 12;
					if (programViewPlayers_)
					{
						SDL_Color c = prog->getOwner()->getColor();
						SDL_SetTextureColorMod(dataContainer->program_core_horizontal, c.r, c.g, c.b);
					}
                    else if (programViewTeams_)
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
                    if (prog == client->getPlayer()->getSelectedProgram())
                    {
                        tileRect.x = xDefault - 2;
                        tileRect.y = yDefault - 2;
                        tileRect.w = TILE_WIDTH;
                        tileRect.h = TILE_WIDTH;
                        SDL_SetTextureAlphaMod(dataContainer->tile_selected, ((double)-textureTickCount_/1000.0)*255 + 255);
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
                SDL_Texture* tileImg = dataContainer->tile_images[client->getGame()->getTileAt(curr)];
                if (client->getGame()->getTileAt(curr) == TILE_NONE && !editorMode_)
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
                !pauseMenu_->isVisible())
            {
                tileRect.x = xDefault;
                tileRect.y = yDefault;
                tileRect.w = sizeDefault;
                tileRect.h = sizeDefault;
                SDL_SetTextureAlphaMod(dataContainer->tile_over, 125);
                SDL_RenderCopy(gRenderer, dataContainer->tile_over, NULL, &tileRect);
            }

            // if there is an item on this tile
            if (client->getGame()->getItemAt(curr) != ITEM_NONE)
            {
                SDL_QueryTexture(dataContainer->item_icons[client->getGame()->getItemAt(curr)], NULL, NULL, &tileRect.w, &tileRect.h);
                tileRect.x = xDefault - (tileRect.w - 28)/2;
                tileRect.y = yDefault - (tileRect.h - 28)/2;
                SDL_RenderCopy(gRenderer, dataContainer->item_icons[client->getGame()->getItemAt(curr)], NULL, &tileRect);
            }

            if (client->getPlayer() == NULL)
                continue;

            // if this is the selected tile
			Iterator<Player*> playIt = client->getGame()->getHumanPlayers()->getIterator();
			while (playIt.hasNext())
			{
				Player* pCurr = playIt.next();
				if (curr == pCurr->getSelectedTile())
				{
					tileRect.x = xDefault - 2;
					tileRect.y = yDefault - 2;
					tileRect.w = TILE_WIDTH;
					tileRect.h = TILE_WIDTH;
					SDL_SetTextureColorMod(dataContainer->tile_selected, pCurr->getColor().r, pCurr->getColor().g, pCurr->getColor().b);
					SDL_SetTextureAlphaMod(dataContainer->tile_selected, ((double)-textureTickCount_ / 1000.0) * 255 + 255);
					SDL_RenderCopy(gRenderer, dataContainer->tile_selected, NULL, &tileRect);
				}
			}

            // if this tile is movable-to
            if (client->getPlayer()->getSelectedProgramDist(curr) > 0 && client->getPlayer()->getSelectedProgram()->getOwner() == client->getPlayer())
            {
                tileRect.x = xDefault;
                tileRect.y = yDefault;
                tileRect.w = sizeDefault;
                tileRect.h = sizeDefault;

                if (curr == client->getPlayer()->getSelectedProgram()->getCore() + Coord{0, 1})
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveSouth, NULL, &tileRect);
                else if (curr == client->getPlayer()->getSelectedProgram()->getCore() + Coord{1, 0})
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveEast, NULL, &tileRect);
                else if (curr == client->getPlayer()->getSelectedProgram()->getCore() + Coord{0, -1})
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveNorth, NULL, &tileRect);
                else if (curr == client->getPlayer()->getSelectedProgram()->getCore() + Coord{-1, 0})
                    SDL_RenderCopy(gRenderer, dataContainer->tile_moveWest, NULL, &tileRect);
                else
                    SDL_RenderCopy(gRenderer, dataContainer->tile_movePossible, NULL, &tileRect);
            }

            // if this tile is in the range of a programAction
            if (client->getPlayer()->getSelectedActionDist(curr) > 0 &&
				client->getGame()->getProgramAt(curr) != client->getPlayer()->getSelectedProgram())
            {
                tileRect.x = xDefault - 2;
                tileRect.y = yDefault - 2;
                tileRect.w = sizeDefault + 4;
                tileRect.h = sizeDefault + 4;

                switch (client->getPlayer()->getSelectedAction()->type)
                {
                case ACTIONTYPE_DAMAGE:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionDamage, NULL, &tileRect);
                    break;
                case ACTIONTYPE_HEAL:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case ACTIONTYPE_MAXHEALTHDOWN:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case ACTIONTYPE_MAXHEALTHUP:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case ACTIONTYPE_SPEEDDOWN:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionSpeed, NULL, &tileRect);
                    break;
                case ACTIONTYPE_SPEEDUP:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case ACTIONTYPE_TILEDELETE:
                    SDL_RenderCopy(gRenderer, dataContainer->tile_actionHeal, NULL, &tileRect);
                    break;
                case ACTIONTYPE_TILEPLACE:
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
        tileRect.x = -bkgPos_.x + 4;
        tileRect.y = -bkgPos_.y + 4;
        tileRect.w = TILE_WIDTH * 200 - 4;
        tileRect.h = TILE_WIDTH * 200 - 4;
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 255, 0);
        SDL_RenderDrawRect(gRenderer, &tileRect);

        SDL_RenderDrawLine(gRenderer, -bkgPos_.x + 100*TILE_WIDTH, -bkgPos_.y + 4, -bkgPos_.x + 100*TILE_WIDTH, -bkgPos_.y + 200*TILE_WIDTH); // vert
        SDL_RenderDrawLine(gRenderer, -bkgPos_.x + 4, -bkgPos_.y + 100*TILE_WIDTH, -bkgPos_.x + 200*TILE_WIDTH, -bkgPos_.y + 100*TILE_WIDTH); // horiz

        tileRect.x = -bkgPos_.x + client->getGame()->getLeftBound()*TILE_WIDTH;
        tileRect.y = -bkgPos_.y + client->getGame()->getTopBound()*TILE_WIDTH;
        tileRect.w = (client->getGame()->getRightBound() - client->getGame()->getLeftBound())*TILE_WIDTH;
        tileRect.h = (client->getGame()->getBottomBound() - client->getGame()->getTopBound())*TILE_WIDTH;
        SDL_RenderDrawRect(gRenderer, &tileRect);
    }
}

void GameScreen::draw()
{
    drawBkg();
    drawGrid();
    GUIContainer::drawContents();

    if (editorMode_)
    {
        gridEditPanel_->draw();
        gridProgramEditPanel_->draw();
        gridSelectBrushPanel_->draw();
        gridItemEditPanel_->draw();
        gridBkgPanel_->draw();
    }

    if (pauseMenu_->isVisible())
    {
        SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 140);
        SDL_RenderFillRect(gRenderer, &bounds);

        SDL_Texture* pause = loadString("PAUSED", FONT_NORMAL, 200, {255, 255, 255, 0});
        SDL_Rect pauseRect = {50, 50, 0, 0};
        SDL_QueryTexture(pause, NULL, NULL, &pauseRect.w, &pauseRect.h);
        SDL_RenderCopy(gRenderer, pause, NULL, &pauseRect);
        SDL_DestroyTexture(pause);

        pauseMenu_->draw();
    }
}

void GameScreen::shiftBkg(Coord disp)
{
    if (!canShiftScreen_) return;

    if (bkgPos_.x + disp.x + SCREEN_WIDTH/2 < client->getGame()->getLeftBound()*TILE_WIDTH)
        bkgPos_.x = client->getGame()->getLeftBound()*TILE_WIDTH - SCREEN_WIDTH/2;
    else if (bkgPos_.x + disp.x + SCREEN_WIDTH/2 > client->getGame()->getRightBound()*TILE_WIDTH)
        bkgPos_.x = client->getGame()->getRightBound()*TILE_WIDTH - SCREEN_WIDTH/2;
    else
        bkgPos_.x += disp.x;

    if (bkgPos_.y + disp.y + SCREEN_HEIGHT/2 < client->getGame()->getTopBound()*TILE_WIDTH)
        bkgPos_.y = client->getGame()->getTopBound()*TILE_WIDTH - SCREEN_HEIGHT/2;
    else if (bkgPos_.y + disp.y + SCREEN_HEIGHT/2 > client->getGame()->getBottomBound()*TILE_WIDTH)
        bkgPos_.y = client->getGame()->getBottomBound()*TILE_WIDTH - SCREEN_HEIGHT/2;
    else
        bkgPos_.y += disp.y;
}

void GameScreen::setBrushTileType(TILE t)
{
    brushTileType_ = t;
}

void GameScreen::setBrushProgramType(PROGRAM p)
{
    brushProgramType_ = p;
    //gameMirror->getHumanPlayer(clientID)->setSelectedProgram(NULL);
}

void GameScreen::setBrushMode(BRUSH b)
{
    brushMode_ = b;
}

void GameScreen::setBrushProgramTeam(int t)
{
    //gameMirror->getHumanPlayer(clientID)->setSelectedProgram(NULL);
    brushProgramTeam_ = t;
}

void GameScreen::toggleViewTeams()
{
    programViewTeams_ = !programViewTeams_;
}

void GameScreen::setBrushItem(ITEM i)
{
    brushItemType_ = i;
}

void GameScreen::setBackgroundImg(BACKGROUND b)
{
    bkgImg = dataContainer->game_backgrounds[b];
	//client->getGame()->setBackground(b);
}

void GameScreen::checkShiftable()
{
	if (client->getGame() == NULL)
		canShiftScreen_ = false;
    else if ((client->getGame()->getRightBound() - client->getGame()->getLeftBound())*TILE_WIDTH < SCREEN_WIDTH - 200 &&
        (client->getGame()->getBottomBound() - client->getGame()->getTopBound())*TILE_WIDTH < SCREEN_HEIGHT - 200)
        canShiftScreen_ = false;
    else
        canShiftScreen_ = true;
}

void GameScreen::tick(int ms)
{
    // tick gui objects
    GUIContainer::tick(ms);

    // don't do anything if pause is visible
    if (pauseMenu_->isVisible())
        return;

    // test to activate/deactivate the debug options
    if (debug >= DEBUG_MINIMAL)
        debugOptions_->setTransparency(255);
    else
        debugOptions_->setTransparency(0);

    // adjust time-dependent textures
    textureTickCount_ += ms;
    if (textureTickCount_ >= 1000) textureTickCount_ = 0;

    // fade out the playerTurn display if it is visible
    if (playerTurn_->isVisible() && playerTurn_->getTransparency() == 255)
    {
        playerTurn_->fade(0, 500);
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
    int shiftAmt = ((int)shiftSpeed_)*ms;
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
    //gameMirror->saveLevel();
}

void GameScreen::resetScreen()
{
    canShiftScreen_ = false;
    centerScreen();
}

void GameScreen::centerScreen()
{
	if (client == NULL || client->getGame() == NULL)
	{
		bkgPos_.x = 100;
		bkgPos_.y = 100;
	}
	else
	{
		bkgPos_.x = (client->getGame()->getRightBound() + client->getGame()->getLeftBound())*TILE_WIDTH / 2 - SCREEN_WIDTH / 2;
		bkgPos_.y = (client->getGame()->getBottomBound() + client->getGame()->getTopBound())*TILE_WIDTH / 2 - SCREEN_HEIGHT / 2;
	}
}

void GameScreen::tryPlacingProgram(PROGRAM p)
{
	// get position
	Coord selectedTile = client->getPlayer()->getSelectedTile();

	// check for correct game state
	if (client->getGame()->getStatus() != GAMESTATUS_PLACING_PROGRAMS)
		return;

	// check for valid value of p
	if (p == PROGRAM_NONE || p == PROGRAM_CUSTOM || p == PROGRAM_NUM_PROGTYPES)
		return;

	// check for a valid spawn tile in this location
	if (client->getGame()->getTileAt(selectedTile) != TILE_SPAWN && client->getGame()->getTileAt(selectedTile) != TILE_SPAWN2)
		return;

	// remove any program that already exists here
	Program* prog = client->getGame()->getProgramAt(selectedTile);
	if (prog != NULL)
	{
		usedPrograms[prog->getType()]--;
		progListCurrent[prog->getType()]++;
		Message m;
		m.type = MSGTYPE_INFO;
		m.infoType = MSGINFOTYPE_PROGRAM;
		m.progType = PROGRAM_NONE;
		m.pos = selectedTile;
		m.playerID = client->getPlayer()->getPlayerID();
		client->sendMessage(m);
	}

	// place the new program
	Message m;
	m.type = MSGTYPE_INFO;
	m.infoType = MSGINFOTYPE_PROGRAM;
	m.progType = p;
	m.pos = selectedTile;
	m.playerID = client->getPlayer()->getPlayerID();
	client->sendMessage(m);

	usedPrograms[p]++;
	progListCurrent[p]--;
	progInv_->updateContents();
}

void GameScreen::toggleEditorMode()
{
    editorMode_ = !editorMode_;
}

void GameScreen::togglePauseMenu()
{
    if (pauseMenu_->isVisible()) pauseMenu_->setTransparency(0);
    else pauseMenu_->setTransparency(255);
}

void GameScreen::changeGameStatus(GAMESTATUS g)
{
	switch (g)
	{
	case GAMESTATUS_NO_GAME:
		break;
	case GAMESTATUS_PLACING_PROGRAMS:
		for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
			usedPrograms[i] = 0;

		preGameOptions_->setTransparency(255);
		progDisp_->setTransparency(0);
		turnButton_->setTransparency(0);
		progInv_->setTransparency(255);
		progInv_->updateContents();
		break;
	case GAMESTATUS_PLAYING:
		preGameOptions_->setTransparency(0);
		progDisp_->setTransparency(255);
		turnButton_->setTransparency(255);
		progInv_->setTransparency(0);
		break;
	case GAMESTATUS_WON:
		// TODO: refactor this
		for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
			progListCurrent[i] += usedPrograms[i];
		mapScreen->getSelectedNode()->winNode();
		mapScreen->clearSelectedNode();
		break;
	case GAMESTATUS_LOST:
		// TODO: refactor this
		for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++)
			progListCurrent[i] += usedPrograms[i];
		pauseMenu_->setTransparency(0);
		mapScreen->clearSelectedNode();
		break;
	}
}
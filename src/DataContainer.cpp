#include <stdio.h>
#include <stdlib.h>

#include "DataContainer.h"
#include "Global.h"
#include "ResourceLoader.h"

DataContainer::DataContainer()
{
    //ctor
}

DataContainer::~DataContainer()
{
    //dtor
}

void initData()
{
    // create the datacontainer
    dataContainer = new DataContainer();

    // titleScreen resources
    dataContainer->title_title = loadTexture("resources/title_main.png");
    dataContainer->title_subtitle = loadTexture("resources/title_subtitle.png");
    dataContainer->title_company = loadTexture("resources/title/title_urfco_alt3.png");

    //dataContainer->node_normal[0] = loadTexture("resources/map/node0_normal.png");
    dataContainer->node_normal[1] = loadTexture("resources/map/node1_normal.png");
    dataContainer->node_normal[2] = loadTexture("resources/map/node2_normal.png");
    dataContainer->node_normal[3] = loadTexture("resources/map/node3_normal.png");
    dataContainer->node_normal[4] = loadTexture("resources/map/node4_normal.png");
    dataContainer->node_normal[5] = loadTexture("resources/map/node5_normal.png");
    dataContainer->node_normal[6] = loadTexture("resources/map/node6_normal.png");
    dataContainer->node_normal[7] = loadTexture("resources/map/node7_normal.png");

    //dataContainer->node_normal_over[0] = loadTexture("resources/map/node0_normal_over.png");
    dataContainer->node_normal_over[1] = loadTexture("resources/map/node1_normal_over.png");
    dataContainer->node_normal_over[2] = loadTexture("resources/map/node2_normal_over.png");
    dataContainer->node_normal_over[3] = loadTexture("resources/map/node3_normal_over.png");
    dataContainer->node_normal_over[4] = loadTexture("resources/map/node4_normal_over.png");
    dataContainer->node_normal_over[5] = loadTexture("resources/map/node5_normal_over.png");
    dataContainer->node_normal_over[6] = loadTexture("resources/map/node6_normal_over.png");
    dataContainer->node_normal_over[7] = loadTexture("resources/map/node7_normal_over.png");

    dataContainer->node_normal_selected[0] = loadTexture("resources/map/node0_normal_selected.png");
    dataContainer->node_normal_selected[1] = loadTexture("resources/map/node1_normal_selected.png");
    dataContainer->node_normal_selected[2] = loadTexture("resources/map/node2_normal_selected.png");
    dataContainer->node_normal_selected[3] = loadTexture("resources/map/node3_normal_selected.png");
    dataContainer->node_normal_selected[4] = loadTexture("resources/map/node4_normal_selected.png");
    dataContainer->node_normal_selected[5] = loadTexture("resources/map/node5_normal_selected.png");
    dataContainer->node_normal_selected[6] = loadTexture("resources/map/node6_normal_selected.png");
    dataContainer->node_normal_selected[7] = loadTexture("resources/map/node7_normal_selected.png");

    dataContainer->node_owned[0] = loadTexture("resources/map/node0_owned.png");
    dataContainer->node_owned[1] = loadTexture("resources/map/node1_owned.png");
    dataContainer->node_owned[2] = loadTexture("resources/map/node2_owned.png");
    dataContainer->node_owned[3] = loadTexture("resources/map/node3_owned.png");
    dataContainer->node_owned[4] = loadTexture("resources/map/node4_owned.png");
    dataContainer->node_owned[5] = loadTexture("resources/map/node5_owned.png");
    dataContainer->node_owned[6] = loadTexture("resources/map/node6_owned.png");
    dataContainer->node_owned[7] = loadTexture("resources/map/node7_owned.png");

    dataContainer->node_owned_over[0] = loadTexture("resources/map/node0_owned_over.png");
    dataContainer->node_owned_over[1] = loadTexture("resources/map/node1_owned_over.png");
    dataContainer->node_owned_over[2] = loadTexture("resources/map/node2_owned_over.png");
    dataContainer->node_owned_over[3] = loadTexture("resources/map/node3_owned_over.png");
    dataContainer->node_owned_over[4] = loadTexture("resources/map/node4_owned_over.png");
    dataContainer->node_owned_over[5] = loadTexture("resources/map/node5_owned_over.png");
    dataContainer->node_owned_over[6] = loadTexture("resources/map/node6_owned_over.png");
    dataContainer->node_owned_over[7] = loadTexture("resources/map/node7_owned_over.png");

    dataContainer->node_owned_selected[0] = loadTexture("resources/map/node0_owned_selected.png");
    dataContainer->node_owned_selected[1] = loadTexture("resources/map/node1_owned_selected.png");
    dataContainer->node_owned_selected[2] = loadTexture("resources/map/node2_owned_selected.png");
    dataContainer->node_owned_selected[3] = loadTexture("resources/map/node3_owned_selected.png");
    dataContainer->node_owned_selected[4] = loadTexture("resources/map/node4_owned_selected.png");
    dataContainer->node_owned_selected[5] = loadTexture("resources/map/node5_owned_selected.png");
    dataContainer->node_owned_selected[6] = loadTexture("resources/map/node6_owned_selected.png");
    dataContainer->node_owned_selected[7] = loadTexture("resources/map/node7_owned_selected.png");

    dataContainer->node_yoffset[0] = 73;
    dataContainer->node_yoffset[1] = 68;
    dataContainer->node_yoffset[2] = 90;
    dataContainer->node_yoffset[3] = 63;
    dataContainer->node_yoffset[4] = 103;
    dataContainer->node_yoffset[5] = 48;
    dataContainer->node_yoffset[6] = 98;
    dataContainer->node_yoffset[7] = 49;

    dataContainer->map_window_levelConfirm = loadTexture("resources/level_confirm.png");
    dataContainer->map_window_levelCancel = loadTexture("resources/level_cancel.png");

    dataContainer->map_button_beginDatabattle_normal = loadTexture("resources/begin_databattle_button_normal.png");
    dataContainer->map_button_beginDatabattle_over = loadTexture("resources/begin_databattle_button_over.png");
    dataContainer->map_button_cancel_normal = loadTexture("resources/cancel_button_normal.png");
    dataContainer->map_button_cancel_over = loadTexture("resources/cancel_button_over.png");

    // gameScreen resources
    dataContainer->game_backgrounds[BKG_DONUT] = loadTexture("resources/company_0.png");
    dataContainer->game_backgrounds[BKG_CELL] = loadTexture("resources/company_1.png");
    dataContainer->game_backgrounds[BKG_MONKEY] = loadTexture("resources/company_2.png");
    dataContainer->game_backgrounds[BKG_PHARM] = loadTexture("resources/company_3.png");
    dataContainer->game_backgrounds[BKG_PED] = loadTexture("resources/company_4.png");
    dataContainer->game_editor_button_bkg[BKG_DONUT] = loadTexture("resources/board/editor/bkg_donut.png");
    dataContainer->game_editor_button_bkg[BKG_CELL] = loadTexture("resources/board/editor/bkg_cellular.png");
    dataContainer->game_editor_button_bkg[BKG_MONKEY] = loadTexture("resources/board/editor/bkg_monkey.png");
    dataContainer->game_editor_button_bkg[BKG_PHARM] = loadTexture("resources/board/editor/bkg_pharmhaus.png");
    dataContainer->game_editor_button_bkg[BKG_PED] = loadTexture("resources/board/editor/bkg_ped.png");

    // initialize tile graphics
    dataContainer->tile_over = loadTexture("resources/board/tile_empty.png");
    dataContainer->tile_selected = loadTexture("resources/board/tile_selected.png");
    dataContainer->tile_moveEast = loadTexture("resources/board/arrowright.png");
    dataContainer->tile_moveSouth = loadTexture("resources/board/arrowdown.png");
    dataContainer->tile_moveWest = loadTexture("resources/board/arrowleft.png");
    dataContainer->tile_moveNorth = loadTexture("resources/board/arrowup.png");
    dataContainer->tile_movePossible = loadTexture("resources/board/movepossible.png");
    dataContainer->tile_images[TILE_PLAIN] = loadTexture("resources/board/tile_empty_transparent.png");
    dataContainer->tile_images[TILE_PLAIN2] = loadTexture("resources/board/tile_empty2_transparent.png");
    dataContainer->tile_images[TILE_PLAIN3] = loadTexture("resources/board/tile_empty3_transparent.png");
    dataContainer->tile_images[TILE_PLAIN4] = loadTexture("resources/board/tile_empty4_transparent.png");
    dataContainer->tile_images[TILE_PLAIN5] = loadTexture("resources/board/tile_empty5_transparent.png");
    dataContainer->tile_images[TILE_PLAIN6] = loadTexture("resources/board/tile_empty6_transparent.png");
    dataContainer->tile_images[TILE_PLAIN7] = loadTexture("resources/board/tile_empty7_transparent.png");
    dataContainer->tile_images[TILE_PLAIN8] = loadTexture("resources/board/tile_empty8_transparent.png");
    dataContainer->tile_images[TILE_PLAIN9] = loadTexture("resources/board/tile_empty9_transparent.png");
    dataContainer->tile_images[TILE_SPAWN] = loadTexture("resources/board/spawn_transparency.png");
    dataContainer->tile_images[TILE_SPAWN2] = loadTexture("resources/board/spawn2_transparency.png");
    dataContainer->tile_images[TILE_NONE] = loadTexture("resources/board/tile_null.png");
    dataContainer->tile_actionDamage = loadTexture("resources/board/hitsquare_transparency.png");
    dataContainer->tile_actionHeal = loadTexture("resources/board/hitsquare2_transparency.png");
    dataContainer->tile_actionSpeed = loadTexture("resources/board/hitsquare3_transparency.png");
    dataContainer->tile_actionGrid = loadTexture("resources/board/hitsquare3_transparency.png");

    // initialize program core graphics
    dataContainer->program_core = loadTexture("resources/board/programs/program_core.png");
    dataContainer->program_core_50px = loadTexture("resources/board/programs/program_core_50px.png");
    dataContainer->program_core_100px = loadTexture("resources/board/programs/program_core_100px.png");
    dataContainer->program_core_vertical = loadTexture("resources/board/programs/program_core_vertical.png");
    dataContainer->program_core_horizontal = loadTexture("resources/board/programs/program_core_horizontal.png");

    // initialize program data
    dataContainer->program_icons[PROGRAM_BALLISTA] = loadTexture("resources/board/programs/program_icon_ballista.png");
    dataContainer->program_icons[PROGRAM_BITMAN] = loadTexture("resources/board/programs/program_icon_bitman.png");
    dataContainer->program_icons[PROGRAM_BITMAN2] = loadTexture("resources/board/programs/program_icon_bitman2.png");
    dataContainer->program_icons[PROGRAM_BLACKWIDOW] = loadTexture("resources/board/programs/program_icon_blackwidow.png");
    dataContainer->program_icons[PROGRAM_BUG] = loadTexture("resources/board/programs/program_icon_bug.png");
    dataContainer->program_icons[PROGRAM_BUG2] = loadTexture("resources/board/programs/program_icon_bug2.png");
    dataContainer->program_icons[PROGRAM_BUG3] = loadTexture("resources/board/programs/program_icon_bug3.png");
    dataContainer->program_icons[PROGRAM_CATAPULT] = loadTexture("resources/board/programs/program_icon_catapult.png");
    dataContainer->program_icons[PROGRAM_CLOG] = loadTexture("resources/board/programs/program_icon_clog.png");
    dataContainer->program_icons[PROGRAM_CLOG2] = loadTexture("resources/board/programs/program_icon_clog2.png");
    dataContainer->program_icons[PROGRAM_CLOG3] = loadTexture("resources/board/programs/program_icon_clog3.png");
    dataContainer->program_icons[PROGRAM_DATABOMB] = loadTexture("resources/board/programs/program_icon_databomb.png");
    dataContainer->program_icons[PROGRAM_DATADOCTOR] = loadTexture("resources/board/programs/program_icon_datadoctor.png");
    dataContainer->program_icons[PROGRAM_DATADOCTOR2] = loadTexture("resources/board/programs/program_icon_datadoctor2.png");
    dataContainer->program_icons[PROGRAM_FIDDLE] = loadTexture("resources/board/programs/program_icon_fiddle.png");
    dataContainer->program_icons[PROGRAM_GOLEM] = loadTexture("resources/board/programs/program_icon_golem.png");
    dataContainer->program_icons[PROGRAM_GOLEM2] = loadTexture("resources/board/programs/program_icon_golem2.png");
    dataContainer->program_icons[PROGRAM_GOLEM3] = loadTexture("resources/board/programs/program_icon_golem3.png");
    dataContainer->program_icons[PROGRAM_HACK] = loadTexture("resources/board/programs/program_icon_hack.png");
    dataContainer->program_icons[PROGRAM_HACK2] = loadTexture("resources/board/programs/program_icon_hack2.png");
    dataContainer->program_icons[PROGRAM_HACK3] = loadTexture("resources/board/programs/program_icon_hack3.png");
    dataContainer->program_icons[PROGRAM_KAMIKAZEE] = loadTexture("resources/board/programs/program_icon_kamikazee.png");
    dataContainer->program_icons[PROGRAM_MEDIC] = loadTexture("resources/board/programs/program_icon_medic.png");
    dataContainer->program_icons[PROGRAM_MEMHOG] = loadTexture("resources/board/programs/program_icon_memhog.png");
    dataContainer->program_icons[PROGRAM_MOBILETOWER] = loadTexture("resources/board/programs/program_icon_mobiletower.png");
    dataContainer->program_icons[PROGRAM_SATELLITE] = loadTexture("resources/board/programs/program_icon_satellite.png");
    dataContainer->program_icons[PROGRAM_SATELLITE2] = loadTexture("resources/board/programs/program_icon_satellite2.png");
    dataContainer->program_icons[PROGRAM_SEEKER] = loadTexture("resources/board/programs/program_icon_seeker.png");
    dataContainer->program_icons[PROGRAM_SEEKER2] = loadTexture("resources/board/programs/program_icon_seeker2.png");
    dataContainer->program_icons[PROGRAM_SEEKER3] = loadTexture("resources/board/programs/program_icon_seeker3.png");
    dataContainer->program_icons[PROGRAM_SLINGSHOT] = loadTexture("resources/board/programs/program_icon_slingshot.png");
    dataContainer->program_icons[PROGRAM_SPECS] = loadTexture("resources/board/programs/program_icon_specs.png");
    dataContainer->program_icons[PROGRAM_SUMO] = loadTexture("resources/board/programs/program_icon_sumo.png");
    dataContainer->program_icons[PROGRAM_TARANTULA] = loadTexture("resources/board/programs/program_icon_tarantula.png");
    dataContainer->program_icons[PROGRAM_TOWER] = loadTexture("resources/board/programs/program_icon_tower.png");
    dataContainer->program_icons[PROGRAM_TURBO] = loadTexture("resources/board/programs/program_icon_turbo.png");
    dataContainer->program_icons[PROGRAM_TURBO2] = loadTexture("resources/board/programs/program_icon_turbo2.png");
    dataContainer->program_icons[PROGRAM_TURBO3] = loadTexture("resources/board/programs/program_icon_turbo3.png");
    dataContainer->program_icons[PROGRAM_WIZARD] = loadTexture("resources/board/programs/program_icon_wizard.png");
    dataContainer->program_icons[PROGRAM_WOLFSPIDER] = loadTexture("resources/board/programs/program_icon_wolfspider.png");
    dataContainer->program_icons[PROGRAM_BOSS] = loadTexture("resources/board/programs/program_icon_boss.png");
    dataContainer->program_icons[PROGRAM_DOG] = loadTexture("resources/board/programs/program_icon_dog.png");
    dataContainer->program_icons[PROGRAM_DOG2] = loadTexture("resources/board/programs/program_icon_dog2.png");
    dataContainer->program_icons[PROGRAM_DOG3] = loadTexture("resources/board/programs/program_icon_dog3.png");
    dataContainer->program_icons[PROGRAM_FIREWALL] = loadTexture("resources/board/programs/program_icon_firewall.png");
    dataContainer->program_icons[PROGRAM_SONAR] = loadTexture("resources/board/programs/program_icon_sonar.png");
    dataContainer->program_icons[PROGRAM_SONAR2] = loadTexture("resources/board/programs/program_icon_sonar2.png");
    dataContainer->program_icons[PROGRAM_SONAR3] = loadTexture("resources/board/programs/program_icon_sonar3.png");
    dataContainer->program_icons[PROGRAM_WALKER] = loadTexture("resources/board/programs/program_icon_walker.png");
    dataContainer->program_icons[PROGRAM_WALKER2] = loadTexture("resources/board/programs/program_icon_walker2.png");
    dataContainer->program_icons[PROGRAM_WALKER3] = loadTexture("resources/board/programs/program_icon_walker3.png");
    dataContainer->program_icons[PROGRAM_WARDEN] = loadTexture("resources/board/programs/program_icon_warden.png");
    dataContainer->program_icons[PROGRAM_WARDEN2] = loadTexture("resources/board/programs/program_icon_warden2.png");
    dataContainer->program_icons[PROGRAM_WARDEN3] = loadTexture("resources/board/programs/program_icon_warden3.png");
    dataContainer->program_icons[PROGRAM_WATCHMAN] = loadTexture("resources/board/programs/program_icon_watchman.png");
    dataContainer->program_icons[PROGRAM_WATCHMAN2] = loadTexture("resources/board/programs/program_icon_watchman2.png");
    dataContainer->program_icons[PROGRAM_WATCHMAN3] = loadTexture("resources/board/programs/program_icon_watchman3.png");

    dataContainer->game_editor_brush_none = loadTexture("resources/board/editor/brush_none.png");
    dataContainer->game_editor_brush_tiles = loadTexture("resources/board/editor/brush_tile.png");
    dataContainer->game_editor_brush_programs = loadTexture("resources/board/editor/brush_program.png");
    dataContainer->game_editor_brush_deletePrograms = loadTexture("resources/board/editor/brush_clearprogram.png");
    dataContainer->game_editor_brush_items = loadTexture("resources/board/editor/brush_item.png");
    dataContainer->game_editor_toggle_teamView = loadTexture("resources/board/editor/team_view.png");
    dataContainer->game_editor_toggle_teamPlayer = loadTexture("resources/board/editor/team_player.png");
    dataContainer->game_editor_toggle_teamComputer = loadTexture("resources/board/editor/team_computer.png");
    dataContainer->game_editor_button_clearGrid = loadTexture("resources/board/editor/clear_grid.png");

    dataContainer->item_icons[ITEM_CREDIT] = loadTexture("resources/board/items/item_credit.png");
    dataContainer->item_icons[ITEM_BIGCREDIT] = loadTexture("resources/board/items/item_coin.png");
    dataContainer->item_icons[ITEM_FILES] = loadTexture("resources/board/items/item_files.png");

    dataContainer->game_button_endTurn = "resources/board/button_end_turn.png";
    dataContainer->game_button_aiStep = "resources/board/button_step.png";
    dataContainer->game_button_win = "resources/board/button_win_game.png";
    dataContainer->game_button_start = "resources/board/button_begin_game.png";

    dataContainer->game_button_resume = "resources/board/button_resume_game.png";
    dataContainer->game_button_reset = "resources/board/button_reset_game.png";
    dataContainer->game_button_viewTeams = "resources/board/button_view_teams.png";
    dataContainer->game_button_quitToMap = "resources/board/button_back_to_map.png";
    dataContainer->game_button_quitToMain = "resources/board/button_quit_to_menu.png";
    dataContainer->game_button_quitToDesktop = "resources/board/button_quit_to_desktop.png";

    dataContainer->game_icon_checkmark = loadTexture("resources/board/checkmark2.png");

    dataContainer->game_disp_compturn = "resources/board/disp_turn_ai.png";
    dataContainer->game_disp_playerturn = "resources/board/disp_turn_player.png";

    // mainScreen resources
    dataContainer->main_bkgdata = loadTexture("resources/main/menu_bkgdata.png");
    dataContainer->main_bkgsplotch = loadTexture("resources/main/menu_bkgsplotch.png");

    dataContainer->main_button_classic = "resources/main/menu_button_classic.png";
    dataContainer->main_button_classic_over = "resources/main/menu_button_classic_over.png";
    dataContainer->main_button_nightfall = "resources/main/menu_button_nightfall.png";
    dataContainer->main_button_nightfall_over = "resources/main/menu_button_nightfall_over.png";
    dataContainer->main_button_freeform = "resources/main/menu_button_freeform.png";
    dataContainer->main_button_freeform_over = "resources/main/menu_button_freeform_over.png";
    dataContainer->main_button_options = "resources/main/menu_button_options.png";
    dataContainer->main_button_options_over = "resources/main/menu_button_options_over.png";
    dataContainer->main_button_achievements = "resources/main/menu_button_achievements.png";
    dataContainer->main_button_achievements_over = "resources/main/menu_button_achievements_over.png";
    dataContainer->main_button_quit = "resources/main/menu_button_quit.png";
    dataContainer->main_button_quit_over = "resources/main/menu_button_quit_over.png";

    // SDL_Mixer music
    dataContainer->music_title = loadMusic("resources/sounds/title_music.flac");
    dataContainer->music_map_ambient = loadMusic("resources/sounds/map_ambient.flac");
    dataContainer->music_game1 = loadMusic("resources/sounds/e1.flac");
    dataContainer->music_game2 = loadMusic("resources/sounds/e3.flac");
    dataContainer->music_game3 = loadMusic("resources/sounds/e5.flac");
    dataContainer->music_game4 = loadMusic("resources/sounds/e8.flac");

    // SDL_Mixer sounds
    dataContainer->sound_move_player = loadSound("resources/sounds/move_player.flac");
    dataContainer->sound_move_computer = loadSound("resources/sounds/move_computer.flac");
    dataContainer->sound_action_attack = loadSound("resources/sounds/action_attack.flac");
    dataContainer->sound_action_grid_damage = loadSound("resources/sounds/action_damage_grid.flac");
    dataContainer->sound_action_grid_fix = loadSound("resources/sounds/action_fix_grid.flac");
    dataContainer->sound_action_heal = loadSound("resources/sounds/action_size.flac");
    dataContainer->sound_action_speed = loadSound("resources/sounds/action_speed.flac");
    dataContainer->sound_pickup_credit = loadSound("resources/sounds/game_credit.flac");
    dataContainer->sound_pickup_files = loadSound("resources/sounds/win_flag.flac");
}

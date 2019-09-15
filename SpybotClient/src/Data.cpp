#include "Standard.h"
#include "Data.h"

#include "Global.h"
#include "ResourceLoader.h"

// titleScreen resources
SDL_Texture* _title_title;
SDL_Texture* _title_subtitle;
SDL_Texture* _title_company;

// mapScreen resources
SDL_Texture* _node_normal[8];
SDL_Texture* _node_normal_over[8];
SDL_Texture* _node_normal_selected[8];
SDL_Texture* _node_owned[8];
SDL_Texture* _node_owned_over[8];
SDL_Texture* _node_owned_selected[8];
int _node_yoffset[8];

SDL_Texture* _map_window_levelConfirm;
SDL_Texture* _map_window_levelCancel;

SDL_Texture* _map_button_beginDatabattle_normal;
SDL_Texture* _map_button_beginDatabattle_over;
SDL_Texture* _map_button_cancel_normal;
SDL_Texture* _map_button_cancel_over;

SDL_Texture* _map_button_joingame;

// lobbyScreen resources
SDL_Texture* _lobby_button_gamemode_coop;
SDL_Texture* _lobby_button_gamemode_coop_over;
SDL_Texture* _lobby_button_gamemode_ffa;
SDL_Texture* _lobby_button_gamemode_ffa_over;
SDL_Texture* _lobby_button_gamemode_teamdm;
SDL_Texture* _lobby_button_gamemode_teamdm_over;
SDL_Texture* _lobby_button_gamemode_selected;

// game resources
SDL_Texture* _game_backgrounds[BKG_NUM_BKGS];

SDL_Texture* _tile_over;
SDL_Texture* _tile_selected;
SDL_Texture* _tile_moveNorth;
SDL_Texture* _tile_moveEast;
SDL_Texture* _tile_moveSouth;
SDL_Texture* _tile_moveWest;
SDL_Texture* _tile_movePossible;
SDL_Texture* _tile_images[TILE_NUM_TILES];
SDL_Texture* _tile_actionDamage;
SDL_Texture* _tile_actionHeal;
SDL_Texture* _tile_actionSpeed;
SDL_Texture* _tile_actionGrid;

SDL_Texture* _program_core;
SDL_Texture* _program_core_50px;
SDL_Texture* _program_core_100px;
SDL_Texture* _program_core_vertical;
SDL_Texture* _program_core_horizontal;

SDL_Texture* _program_icons[PROGRAM_NUM_PROGTYPES];
int _program_colorDefault[PROGRAM_NUM_PROGTYPES][3];
int _program_maxHealthDefault[PROGRAM_NUM_PROGTYPES];
int _program_speedDefault[PROGRAM_NUM_PROGTYPES];
std::string _program_nameDefault[PROGRAM_NUM_PROGTYPES];
std::string _program_descriptionDefault[PROGRAM_NUM_PROGTYPES];
int _program_costDefault[PROGRAM_NUM_PROGTYPES];

SDL_Texture* _item_icons[ITEM_NUM_ITEMS];

SDL_Texture* _game_editor_brush_none;
SDL_Texture* _game_editor_brush_tiles;
SDL_Texture* _game_editor_brush_programs;
SDL_Texture* _game_editor_brush_deletePrograms;
SDL_Texture* _game_editor_brush_items;
SDL_Texture* _game_editor_toggle_teamView;
SDL_Texture* _game_editor_toggle_teamPlayer;
SDL_Texture* _game_editor_toggle_teamComputer;
SDL_Texture* _game_editor_button_clearGrid;
SDL_Texture* _game_editor_button_bkg[BKG_NUM_BKGS];

std::string _game_button_endTurn;
std::string _game_button_aiStep;
std::string _game_button_win;
std::string _game_button_start;

std::string _game_button_resume;
std::string _game_button_reset;
std::string _game_button_viewTeams;
std::string _game_button_quitToMap;
std::string _game_button_quitToMain;
std::string _game_button_quitToDesktop;

SDL_Texture* _game_icon_checkmark;
SDL_Texture* _game_icon_dead;
SDL_Texture* _game_icon_owner;
SDL_Texture* _game_icon_ai;
SDL_Texture* _game_icon_human;
SDL_Texture* _game_icon_currTurn;

SDL_Texture* _game_icon_action_attack;
SDL_Texture* _game_icon_action_medic;
SDL_Texture* _game_icon_action_unmedic;
SDL_Texture* _game_icon_action_slow;
SDL_Texture* _game_icon_action_speedup;
SDL_Texture* _game_icon_action_tilecreate;
SDL_Texture* _game_icon_action_tiledestroy;

std::string _game_disp_playerturn;
std::string _game_disp_compturn;

// mainScreen resources
SDL_Texture* _main_bkgdata;
SDL_Texture* _main_bkgsplotch;
SDL_Texture* _main_bkgsplotch2;

std::string _main_button_classic;
std::string _main_button_classic_over;
std::string _main_button_nightfall;
std::string _main_button_nightfall_over;
std::string _main_button_freeform;
std::string _main_button_freeform_over;
std::string _main_button_multiplayer;
std::string _main_button_multiplayer_over;
std::string _main_button_options;
std::string _main_button_options_over;
std::string _main_button_achievements;
std::string _main_button_achievements_over;
std::string _main_button_quit;
std::string _main_button_quit_over;

// SDL_Mixer music
Mix_Music* _music_title;
Mix_Music* _music_map_ambient;
Mix_Music* _music_game1;
Mix_Music* _music_game2;
Mix_Music* _music_game3;
Mix_Music* _music_game4;

// SDL_Mixer sounds
Mix_Chunk* _sound_move_player;
Mix_Chunk* _sound_move_computer;
Mix_Chunk* _sound_action_attack;
Mix_Chunk* _sound_action_speed;
Mix_Chunk* _sound_action_heal;
Mix_Chunk* _sound_action_grid_damage;
Mix_Chunk* _sound_action_grid_fix;
Mix_Chunk* _sound_select;
Mix_Chunk* _sound_pickup_credit;
Mix_Chunk* _sound_pickup_files;

// colors
SDL_Color _color_bkg_standard;
SDL_Color _color_black;
SDL_Color _color_white;
SDL_Color _color_action_medic;
SDL_Color _color_action_unmedic;
SDL_Color _color_action_attack;
SDL_Color _color_action_slow;
SDL_Color _color_action_speedup;
SDL_Color _color_action_tilecreate;
SDL_Color _color_action_tiledestroy;

// animations
SDL_Texture* _particle_basic;
SDL_Texture* _particle_selectring;

// initialize all data
void initData()
{
	// titleScreen resources
	_title_title = loadTexture("resources/title_main.png");
	_title_subtitle = loadTexture("resources/title_subtitle.png");
	_title_company = loadTexture("resources/title/title_urfco_alt3.png");

	_node_normal[0] = NULL; //loadTexture("resources/map/node0_normal.png");
	_node_normal[1] = loadTexture("resources/map/node1_normal.png");
	_node_normal[2] = loadTexture("resources/map/node2_normal.png");
	_node_normal[3] = loadTexture("resources/map/node3_normal.png");
	_node_normal[4] = loadTexture("resources/map/node4_normal.png");
	_node_normal[5] = loadTexture("resources/map/node5_normal.png");
	_node_normal[6] = loadTexture("resources/map/node6_normal.png");
	_node_normal[7] = loadTexture("resources/map/node7_normal.png");

	_node_normal_over[0] = NULL; //loadTexture("resources/map/node0_normal_over.png");
	_node_normal_over[1] = loadTexture("resources/map/node1_normal_over.png");
	_node_normal_over[2] = loadTexture("resources/map/node2_normal_over.png");
	_node_normal_over[3] = loadTexture("resources/map/node3_normal_over.png");
	_node_normal_over[4] = loadTexture("resources/map/node4_normal_over.png");
	_node_normal_over[5] = loadTexture("resources/map/node5_normal_over.png");
	_node_normal_over[6] = loadTexture("resources/map/node6_normal_over.png");
	_node_normal_over[7] = loadTexture("resources/map/node7_normal_over.png");

	_node_normal_selected[0] = loadTexture("resources/map/node0_normal_selected.png");
	_node_normal_selected[1] = loadTexture("resources/map/node1_normal_selected.png");
	_node_normal_selected[2] = loadTexture("resources/map/node2_normal_selected.png");
	_node_normal_selected[3] = loadTexture("resources/map/node3_normal_selected.png");
	_node_normal_selected[4] = loadTexture("resources/map/node4_normal_selected.png");
	_node_normal_selected[5] = loadTexture("resources/map/node5_normal_selected.png");
	_node_normal_selected[6] = loadTexture("resources/map/node6_normal_selected.png");
	_node_normal_selected[7] = loadTexture("resources/map/node7_normal_selected.png");

	_node_owned[0] = loadTexture("resources/map/node0_owned.png");
	_node_owned[1] = loadTexture("resources/map/node1_owned.png");
	_node_owned[2] = loadTexture("resources/map/node2_owned.png");
	_node_owned[3] = loadTexture("resources/map/node3_owned.png");
	_node_owned[4] = loadTexture("resources/map/node4_owned.png");
	_node_owned[5] = loadTexture("resources/map/node5_owned.png");
	_node_owned[6] = loadTexture("resources/map/node6_owned.png");
	_node_owned[7] = loadTexture("resources/map/node7_owned.png");

	_node_owned_over[0] = loadTexture("resources/map/node0_owned_over.png");
	_node_owned_over[1] = loadTexture("resources/map/node1_owned_over.png");
	_node_owned_over[2] = loadTexture("resources/map/node2_owned_over.png");
	_node_owned_over[3] = loadTexture("resources/map/node3_owned_over.png");
	_node_owned_over[4] = loadTexture("resources/map/node4_owned_over.png");
	_node_owned_over[5] = loadTexture("resources/map/node5_owned_over.png");
	_node_owned_over[6] = loadTexture("resources/map/node6_owned_over.png");
	_node_owned_over[7] = loadTexture("resources/map/node7_owned_over.png");

	_node_owned_selected[0] = loadTexture("resources/map/node0_owned_selected.png");
	_node_owned_selected[1] = loadTexture("resources/map/node1_owned_selected.png");
	_node_owned_selected[2] = loadTexture("resources/map/node2_owned_selected.png");
	_node_owned_selected[3] = loadTexture("resources/map/node3_owned_selected.png");
	_node_owned_selected[4] = loadTexture("resources/map/node4_owned_selected.png");
	_node_owned_selected[5] = loadTexture("resources/map/node5_owned_selected.png");
	_node_owned_selected[6] = loadTexture("resources/map/node6_owned_selected.png");
	_node_owned_selected[7] = loadTexture("resources/map/node7_owned_selected.png");

	_node_yoffset[0] = 73;
	_node_yoffset[1] = 68;
	_node_yoffset[2] = 90;
	_node_yoffset[3] = 63;
	_node_yoffset[4] = 103;
	_node_yoffset[5] = 48;
	_node_yoffset[6] = 98;
	_node_yoffset[7] = 49;

	_map_window_levelConfirm = loadTexture("resources/level_confirm.png");
	_map_window_levelCancel = loadTexture("resources/level_cancel.png");

	_map_button_beginDatabattle_normal = loadTexture("resources/begin_databattle_button_normal.png");
	_map_button_beginDatabattle_over = loadTexture("resources/begin_databattle_button_over.png");
	_map_button_cancel_normal = loadTexture("resources/cancel_button_normal.png");
	_map_button_cancel_over = loadTexture("resources/cancel_button_over.png");

	_map_button_joingame = loadTexture("resources/map/joinexisting.png");

	// lobbyScreen resources
	_lobby_button_gamemode_coop = loadTexture("resources/lobby/gamemode_coop.png");
	_lobby_button_gamemode_coop_over = loadTexture("resources/lobby/gamemode_coop_over.png");
	_lobby_button_gamemode_ffa = loadTexture("resources/lobby/gamemode_ffa.png");
	_lobby_button_gamemode_ffa_over = loadTexture("resources/lobby/gamemode_ffa_over.png");
	_lobby_button_gamemode_teamdm = loadTexture("resources/lobby/gamemode_teamdm.png");
	_lobby_button_gamemode_teamdm_over = loadTexture("resources/lobby/gamemode_teamdm_over.png");
	_lobby_button_gamemode_selected = loadTexture("resources/lobby/gamemode_selected.png");

	// gameScreen resources
	_game_backgrounds[0] = loadTexture("resources/company_0.png");
	_game_backgrounds[1] = loadTexture("resources/company_1.png");
	_game_backgrounds[2] = loadTexture("resources/company_2.png");
	_game_backgrounds[3] = loadTexture("resources/company_3.png");
	_game_backgrounds[4] = loadTexture("resources/company_4.png");

	// initialize tile graphics
	_tile_over = loadTexture("resources/board/tile_empty.png");
	_tile_selected = loadTexture("resources/board/tile_selected_bold.png");
	_tile_moveEast = loadTexture("resources/board/arrowright.png");
	_tile_moveSouth = loadTexture("resources/board/arrowdown.png");
	_tile_moveWest = loadTexture("resources/board/arrowleft.png");
	_tile_moveNorth = loadTexture("resources/board/arrowup.png");
	_tile_movePossible = loadTexture("resources/board/movepossible.png");
	_tile_actionDamage = loadTexture("resources/board/hitsquare_transparency.png");
	_tile_actionHeal = loadTexture("resources/board/hitsquare2_transparency.png");
	_tile_actionSpeed = loadTexture("resources/board/hitsquare3_transparency.png");
	_tile_actionGrid = loadTexture("resources/board/hitsquare3_transparency.png");
	_tile_images[TILE_NONE] = loadTexture("resources/board/tile_null.png");
	_tile_images[TILE_PLAIN] = loadTexture("resources/board/tile_empty_transparent.png");
	_tile_images[TILE_PLAIN2] = loadTexture("resources/board/tile_empty2_transparent.png");
	_tile_images[TILE_PLAIN3] = loadTexture("resources/board/tile_empty3_transparent.png");
	_tile_images[TILE_PLAIN4] = loadTexture("resources/board/tile_empty4_transparent.png");
	_tile_images[TILE_PLAIN5] = loadTexture("resources/board/tile_empty5_transparent.png");
	_tile_images[TILE_PLAIN6] = loadTexture("resources/board/tile_empty6_transparent.png");
	_tile_images[TILE_PLAIN7] = loadTexture("resources/board/tile_empty7_transparent.png");
	_tile_images[TILE_PLAIN8] = loadTexture("resources/board/tile_empty8_transparent.png");
	_tile_images[TILE_PLAIN9] = loadTexture("resources/board/tile_empty9_transparent.png");
	_tile_images[TILE_SPAWN] = loadTexture("resources/board/spawn_transparency.png");
	_tile_images[TILE_SPAWN2] = loadTexture("resources/board/spawn2_transparency.png");

	// initialize program core graphics
	_program_core = loadTexture("resources/board/programs/program_core.png");
	_program_core_50px = loadTexture("resources/board/programs/program_core_50px.png");
	_program_core_100px = loadTexture("resources/board/programs/program_core_100px.png");
	_program_core_vertical = loadTexture("resources/board/programs/program_core_vertical.png");
	_program_core_horizontal = loadTexture("resources/board/programs/program_core_horizontal.png");

	// initialize program data
	_program_icons[0] = NULL;
	_program_icons[1] = NULL;
	_program_icons[2] = loadTexture("resources/board/programs/program_icon_ballista.png");
	_program_icons[3] = loadTexture("resources/board/programs/program_icon_bitman.png");
	_program_icons[4] = loadTexture("resources/board/programs/program_icon_bitman2.png");
	_program_icons[5] = loadTexture("resources/board/programs/program_icon_blackwidow.png");
	_program_icons[6] = loadTexture("resources/board/programs/program_icon_bug.png");
	_program_icons[7] = loadTexture("resources/board/programs/program_icon_bug2.png");
	_program_icons[8] = loadTexture("resources/board/programs/program_icon_bug3.png");
	_program_icons[9] = loadTexture("resources/board/programs/program_icon_catapult.png");
	_program_icons[10] = loadTexture("resources/board/programs/program_icon_clog.png");
	_program_icons[11] = loadTexture("resources/board/programs/program_icon_clog2.png");
	_program_icons[12] = loadTexture("resources/board/programs/program_icon_clog3.png");
	_program_icons[13] = loadTexture("resources/board/programs/program_icon_databomb.png");
	_program_icons[14] = loadTexture("resources/board/programs/program_icon_datadoctor.png");
	_program_icons[15] = loadTexture("resources/board/programs/program_icon_datadoctor2.png");
	_program_icons[16] = loadTexture("resources/board/programs/program_icon_fiddle.png");
	_program_icons[17] = loadTexture("resources/board/programs/program_icon_golem.png");
	_program_icons[18] = loadTexture("resources/board/programs/program_icon_golem2.png");
	_program_icons[19] = loadTexture("resources/board/programs/program_icon_golem3.png");
	_program_icons[20] = loadTexture("resources/board/programs/program_icon_hack.png");
	_program_icons[21] = loadTexture("resources/board/programs/program_icon_hack2.png");
	_program_icons[22] = loadTexture("resources/board/programs/program_icon_hack3.png");
	_program_icons[23] = loadTexture("resources/board/programs/program_icon_kamikazee.png");
	_program_icons[24] = loadTexture("resources/board/programs/program_icon_medic.png");
	_program_icons[25] = loadTexture("resources/board/programs/program_icon_memhog.png");
	_program_icons[26] = loadTexture("resources/board/programs/program_icon_mobiletower.png");
	_program_icons[27] = loadTexture("resources/board/programs/program_icon_satellite.png");
	_program_icons[28] = loadTexture("resources/board/programs/program_icon_satellite2.png");
	_program_icons[29] = loadTexture("resources/board/programs/program_icon_seeker.png");
	_program_icons[30] = loadTexture("resources/board/programs/program_icon_seeker2.png");
	_program_icons[31] = loadTexture("resources/board/programs/program_icon_seeker3.png");
	_program_icons[32] = loadTexture("resources/board/programs/program_icon_slingshot.png");
	_program_icons[33] = loadTexture("resources/board/programs/program_icon_specs.png");
	_program_icons[34] = loadTexture("resources/board/programs/program_icon_sumo.png");
	_program_icons[35] = loadTexture("resources/board/programs/program_icon_tarantula.png");
	_program_icons[36] = loadTexture("resources/board/programs/program_icon_tower.png");
	_program_icons[37] = loadTexture("resources/board/programs/program_icon_turbo.png");
	_program_icons[38] = loadTexture("resources/board/programs/program_icon_turbo2.png");
	_program_icons[39] = loadTexture("resources/board/programs/program_icon_turbo3.png");
	_program_icons[40] = loadTexture("resources/board/programs/program_icon_wizard.png");
	_program_icons[41] = loadTexture("resources/board/programs/program_icon_wolfspider.png");
	_program_icons[42] = loadTexture("resources/board/programs/program_icon_boss.png");
	_program_icons[43] = loadTexture("resources/board/programs/program_icon_dog.png");
	_program_icons[44] = loadTexture("resources/board/programs/program_icon_dog2.png");
	_program_icons[45] = loadTexture("resources/board/programs/program_icon_dog3.png");
	_program_icons[46] = loadTexture("resources/board/programs/program_icon_firewall.png");
	_program_icons[47] = loadTexture("resources/board/programs/program_icon_sonar.png");
	_program_icons[48] = loadTexture("resources/board/programs/program_icon_sonar2.png");
	_program_icons[49] = loadTexture("resources/board/programs/program_icon_sonar3.png");
	_program_icons[50] = loadTexture("resources/board/programs/program_icon_walker.png");
	_program_icons[51] = loadTexture("resources/board/programs/program_icon_walker2.png");
	_program_icons[52] = loadTexture("resources/board/programs/program_icon_walker3.png");
	_program_icons[53] = loadTexture("resources/board/programs/program_icon_warden.png");
	_program_icons[54] = loadTexture("resources/board/programs/program_icon_warden2.png");
	_program_icons[55] = loadTexture("resources/board/programs/program_icon_warden3.png");
	_program_icons[56] = loadTexture("resources/board/programs/program_icon_watchman.png");
	_program_icons[57] = loadTexture("resources/board/programs/program_icon_watchman2.png");
	_program_icons[58] = loadTexture("resources/board/programs/program_icon_watchman3.png");

	_game_editor_brush_none = loadTexture("resources/board/editor/brush_none.png");
	_game_editor_brush_tiles = loadTexture("resources/board/editor/brush_tile.png");
	_game_editor_brush_programs = loadTexture("resources/board/editor/brush_program.png");
	_game_editor_brush_deletePrograms = loadTexture("resources/board/editor/brush_clearprogram.png");
	_game_editor_brush_items = loadTexture("resources/board/editor/brush_item.png");
	_game_editor_toggle_teamView = loadTexture("resources/board/editor/team_view.png");
	_game_editor_toggle_teamPlayer = loadTexture("resources/board/editor/team_player.png");
	_game_editor_toggle_teamComputer = loadTexture("resources/board/editor/team_computer.png");
	_game_editor_button_clearGrid = loadTexture("resources/board/editor/clear_grid.png");
	_game_editor_button_bkg[BKG_NONE] = NULL;
	_game_editor_button_bkg[BKG_DONUT] = loadTexture("resources/board/editor/bkg_donut.png");
	_game_editor_button_bkg[BKG_PHARM] = loadTexture("resources/board/editor/bkg_pharmhaus.png");
	_game_editor_button_bkg[BKG_MONKEY] = loadTexture("resources/board/editor/bkg_monkey.png");
	_game_editor_button_bkg[BKG_CELL] = loadTexture("resources/board/editor/bkg_cellular.png");
	_game_editor_button_bkg[BKG_PED] = loadTexture("resources/board/editor/bkg_ped.png");

	_item_icons[ITEM_NONE] = NULL;
	_item_icons[ITEM_CREDIT] = loadTexture("resources/board/items/item_credit.png");
	_item_icons[ITEM_BIGCREDIT] = loadTexture("resources/board/items/item_coin.png");
	_item_icons[ITEM_FILES] = loadTexture("resources/board/items/item_files.png");

	_game_button_endTurn = "resources/board/button_end_turn.png";
	_game_button_aiStep = "resources/board/button_step.png";
	_game_button_win = "resources/board/button_win_game.png";
	_game_button_start = "resources/board/button_begin_game.png";

	_game_button_resume = "resources/board/button_resume_game.png";
	_game_button_reset = "resources/board/button_reset_game.png";
	_game_button_viewTeams = "resources/board/button_view_teams.png";
	_game_button_quitToMap = "resources/board/button_back_to_map.png";
	_game_button_quitToMain = "resources/board/button_quit_to_menu.png";
	_game_button_quitToDesktop = "resources/board/button_quit_to_desktop.png";

	_game_icon_checkmark = loadTexture("resources/board/checkmark2.png");
	_game_icon_dead = loadTexture("resources/lobby/dead_30px.png");
	_game_icon_owner = loadTexture("resources/lobby/crown_35px_30px.png");
	_game_icon_human = loadTexture("resources/lobby/person_23px_30px.png");
	_game_icon_ai = loadTexture("resources/lobby/cpu_30px.png");
	_game_icon_currTurn = loadTexture("resources/lobby/arrow_30px.png");

	_game_icon_action_attack = loadTexture("resources/board/actions/program_icon_action_attack.png");
	_game_icon_action_medic = loadTexture("resources/board/actions/program_icon_action_medic.png");
	_game_icon_action_unmedic = loadTexture("resources/board/actions/program_icon_action_unmedic.png");
	_game_icon_action_slow = loadTexture("resources/board/actions/program_icon_action_slow.png");
	_game_icon_action_speedup = loadTexture("resources/board/actions/program_icon_action_speedup.png");
	_game_icon_action_tilecreate = loadTexture("resources/board/actions/program_icon_action_tilecreate.png");
	_game_icon_action_tiledestroy = loadTexture("resources/board/actions/program_icon_action_tiledestroy.png");

	_game_disp_compturn = "resources/board/disp_turn_ai.png";
	_game_disp_playerturn = "resources/board/disp_turn_player.png";

	// mainScreen resources
	_main_bkgdata = loadTexture("resources/main/menu_bkgdata.png");
	_main_bkgsplotch = loadTexture("resources/main/menu_bkgsplotch.png");
	_main_bkgsplotch2 = loadTexture("resources/main/menu_bkgsplotch2.png");

	_main_button_classic = "resources/main/menu_button_classic.png";
	_main_button_classic_over = "resources/main/menu_button_classic_over.png";
	_main_button_nightfall = "resources/main/menu_button_nightfall.png";
	_main_button_nightfall_over = "resources/main/menu_button_nightfall_over.png";
	_main_button_freeform = "resources/main/menu_button_freeform.png";
	_main_button_freeform_over = "resources/main/menu_button_freeform_over.png";
	_main_button_multiplayer = "resources/main/menu_button_multiplayer.png";
	_main_button_multiplayer_over = "resources/main/menu_button_multiplayer_over.png";
	_main_button_options = "resources/main/menu_button_options.png";
	_main_button_options_over = "resources/main/menu_button_options_over.png";
	_main_button_achievements = "resources/main/menu_button_achievements.png";
	_main_button_achievements_over = "resources/main/menu_button_achievements_over.png";
	_main_button_quit = "resources/main/menu_button_quit.png";
	_main_button_quit_over = "resources/main/menu_button_quit_over.png";

	// SDL_Mixer music
	_music_title = loadMusic("resources/sounds/title_music.flac");
	_music_map_ambient = loadMusic("resources/sounds/map_ambient.flac");
	_music_game1 = loadMusic("resources/sounds/e1.flac");
	_music_game2 = loadMusic("resources/sounds/e3.flac");
	_music_game3 = loadMusic("resources/sounds/e5.flac");
	_music_game4 = loadMusic("resources/sounds/e8.flac");

	// SDL_Mixer sounds
	_sound_move_player = loadSound("resources/sounds/move_player.flac");
	_sound_move_computer = loadSound("resources/sounds/move_computer.flac");
	_sound_action_attack = loadSound("resources/sounds/action_attack.flac");
	_sound_action_grid_damage = loadSound("resources/sounds/action_damage_grid.flac");
	_sound_action_grid_fix = loadSound("resources/sounds/action_fix_grid.flac");
	_sound_action_heal = loadSound("resources/sounds/action_size.flac");
	_sound_action_speed = loadSound("resources/sounds/action_speed.flac");
	_sound_pickup_credit = loadSound("resources/sounds/game_credit.flac");
	_sound_pickup_files = loadSound("resources/sounds/win_flag.flac");

	// colors
	_color_bkg_standard = { 120, 120, 120, 140 };
	_color_black = { 0, 0, 0, 255 };
	_color_white = { 255, 255, 255, 255 };
	_color_action_medic = { 255, 80, 80, 255 };
	_color_action_unmedic = { 160, 180, 160, 255 };
	_color_action_attack = { 255, 51, 0, 255 };
	_color_action_slow = { 0, 102, 255, 255 };
	_color_action_speedup = { 255, 255, 0, 255 };
	_color_action_tilecreate = { 153, 255, 102, 255 };
	_color_action_tiledestroy = { 102, 51, 0, 255 };

	// animations
	_particle_basic = loadTexture("resources/board/particle.png");
	_particle_selectring = loadTexture("resources/board/particle_selectring.png");
}
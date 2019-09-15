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

std::string _map_button_beginDatabattle_normal;
std::string _map_button_beginDatabattle_over;
std::string _map_button_cancel_normal;
std::string _map_button_cancel_over;

std::string _map_button_joingame;

// lobbyScreen resources
std::string _lobby_button_gamemode_coop;
std::string _lobby_button_gamemode_coop_over;
std::string _lobby_button_gamemode_ffa;
std::string _lobby_button_gamemode_ffa_over;
std::string _lobby_button_gamemode_teamdm;
std::string _lobby_button_gamemode_teamdm_over;
std::string _lobby_button_gamemode_selected;

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
std::string _program_icons_paths[PROGRAM_NUM_PROGTYPES];
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
std::string _main_campaign_classic;
std::string _main_campaign_nightfall;
std::string _main_campaign_procedural;
std::string _main_campaign_bkg;

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
SDL_Color _color_clear;
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

// achievements
SDL_Texture* _achievement_badge;
SDL_Texture* _achievement_badge_large;
SDL_Texture* _achievement_texture[ACHIEVEMENT_NUM_ACHIEVEMENTS];
SDL_Texture* _achievement_locked;
std::string _achievement_name[ACHIEVEMENT_NUM_ACHIEVEMENTS];
std::string _achievement_description[ACHIEVEMENT_NUM_ACHIEVEMENTS];

// initialize all data
void initData() {
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

	_map_button_beginDatabattle_normal = "resources/begin_databattle_button_normal.png";
	_map_button_beginDatabattle_over = "resources/begin_databattle_button_over.png";
	_map_button_cancel_normal = "resources/cancel_button_normal.png";
	_map_button_cancel_over = "resources/cancel_button_over.png";

	_map_button_joingame = "resources/map/joinexisting.png";

	// lobbyScreen resources
	_lobby_button_gamemode_coop = "resources/lobby/gamemode_coop.png";
	_lobby_button_gamemode_coop_over = "resources/lobby/gamemode_coop_over.png";
	_lobby_button_gamemode_ffa = "resources/lobby/gamemode_ffa.png";
	_lobby_button_gamemode_ffa_over = "resources/lobby/gamemode_ffa_over.png";
	_lobby_button_gamemode_teamdm = "resources/lobby/gamemode_teamdm.png";
	_lobby_button_gamemode_teamdm_over = "resources/lobby/gamemode_teamdm_over.png";
	_lobby_button_gamemode_selected = "resources/lobby/gamemode_selected.png";

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
	_program_icons_paths[PROGRAM_BALLISTA] = "resources/board/programs/program_icon_ballista.png";
	_program_icons_paths[PROGRAM_BITMAN] = "resources/board/programs/program_icon_bitman.png";
	_program_icons_paths[PROGRAM_BITMAN2] = "resources/board/programs/program_icon_bitman2.png";
	_program_icons_paths[PROGRAM_BLACKWIDOW] = "resources/board/programs/program_icon_blackwidow.png";
	_program_icons_paths[PROGRAM_BUG] = "resources/board/programs/program_icon_bug.png";
	_program_icons_paths[PROGRAM_BUG2] = "resources/board/programs/program_icon_bug2.png";
	_program_icons_paths[PROGRAM_BUG3] = "resources/board/programs/program_icon_bug3.png";
	_program_icons_paths[PROGRAM_CATAPULT] = "resources/board/programs/program_icon_catapult.png";
	_program_icons_paths[PROGRAM_CLOG] = "resources/board/programs/program_icon_clog.png";
	_program_icons_paths[PROGRAM_CLOG2] = "resources/board/programs/program_icon_clog2.png";
	_program_icons_paths[PROGRAM_CLOG3] = "resources/board/programs/program_icon_clog3.png";
	_program_icons_paths[PROGRAM_DATABOMB] = "resources/board/programs/program_icon_databomb.png";
	_program_icons_paths[PROGRAM_DATADOCTOR] = "resources/board/programs/program_icon_datadoctor.png";
	_program_icons_paths[PROGRAM_DATADOCTOR2] = "resources/board/programs/program_icon_datadoctor2.png";
	_program_icons_paths[PROGRAM_FIDDLE] = "resources/board/programs/program_icon_fiddle.png";
	_program_icons_paths[PROGRAM_GOLEM] = "resources/board/programs/program_icon_golem.png";
	_program_icons_paths[PROGRAM_GOLEM2] = "resources/board/programs/program_icon_golem2.png";
	_program_icons_paths[PROGRAM_GOLEM3] = "resources/board/programs/program_icon_golem3.png";
	_program_icons_paths[PROGRAM_HACK] = "resources/board/programs/program_icon_hack.png";
	_program_icons_paths[PROGRAM_HACK2] = "resources/board/programs/program_icon_hack2.png";
	_program_icons_paths[PROGRAM_HACK3] = "resources/board/programs/program_icon_hack3.png";
	_program_icons_paths[PROGRAM_KAMIKAZEE] = "resources/board/programs/program_icon_kamikazee.png";
	_program_icons_paths[PROGRAM_MEDIC] = "resources/board/programs/program_icon_medic.png";
	_program_icons_paths[PROGRAM_MEMHOG] = "resources/board/programs/program_icon_memhog.png";
	_program_icons_paths[PROGRAM_MOBILETOWER] = "resources/board/programs/program_icon_mobiletower.png";
	_program_icons_paths[PROGRAM_SATELLITE] = "resources/board/programs/program_icon_satellite.png";
	_program_icons_paths[PROGRAM_SATELLITE2] = "resources/board/programs/program_icon_satellite2.png";
	_program_icons_paths[PROGRAM_SEEKER] = "resources/board/programs/program_icon_seeker.png";
	_program_icons_paths[PROGRAM_SEEKER2] = "resources/board/programs/program_icon_seeker2.png";
	_program_icons_paths[PROGRAM_SEEKER3] = "resources/board/programs/program_icon_seeker3.png";
	_program_icons_paths[PROGRAM_SLINGSHOT] = "resources/board/programs/program_icon_slingshot.png";
	_program_icons_paths[PROGRAM_SPECS] = "resources/board/programs/program_icon_specs.png";
	_program_icons_paths[PROGRAM_SUMO] = "resources/board/programs/program_icon_sumo.png";
	_program_icons_paths[PROGRAM_TARANTULA] = "resources/board/programs/program_icon_tarantula.png";
	_program_icons_paths[PROGRAM_TOWER] = "resources/board/programs/program_icon_tower.png";
	_program_icons_paths[PROGRAM_TURBO] = "resources/board/programs/program_icon_turbo.png";
	_program_icons_paths[PROGRAM_TURBO2] = "resources/board/programs/program_icon_turbo2.png";
	_program_icons_paths[PROGRAM_TURBO3] = "resources/board/programs/program_icon_turbo3.png";
	_program_icons_paths[PROGRAM_WIZARD] = "resources/board/programs/program_icon_wizard.png";
	_program_icons_paths[PROGRAM_WOLFSPIDER] = "resources/board/programs/program_icon_wolfspider.png";
	_program_icons_paths[PROGRAM_BOSS] = "resources/board/programs/program_icon_boss.png";
	_program_icons_paths[PROGRAM_DOG] = "resources/board/programs/program_icon_dog.png";
	_program_icons_paths[PROGRAM_DOG2] = "resources/board/programs/program_icon_dog2.png";
	_program_icons_paths[PROGRAM_DOG3] = "resources/board/programs/program_icon_dog3.png";
	_program_icons_paths[PROGRAM_FIREWALL] = "resources/board/programs/program_icon_firewall.png";
	_program_icons_paths[PROGRAM_SONAR] = "resources/board/programs/program_icon_sonar.png";
	_program_icons_paths[PROGRAM_SONAR2] = "resources/board/programs/program_icon_sonar2.png";
	_program_icons_paths[PROGRAM_SONAR3] = "resources/board/programs/program_icon_sonar3.png";
	_program_icons_paths[PROGRAM_WALKER] = "resources/board/programs/program_icon_walker.png";
	_program_icons_paths[PROGRAM_WALKER2] = "resources/board/programs/program_icon_walker2.png";
	_program_icons_paths[PROGRAM_WALKER3] = "resources/board/programs/program_icon_walker3.png";
	_program_icons_paths[PROGRAM_WARDEN] = "resources/board/programs/program_icon_warden.png";
	_program_icons_paths[PROGRAM_WARDEN2] = "resources/board/programs/program_icon_warden2.png";
	_program_icons_paths[PROGRAM_WARDEN3] = "resources/board/programs/program_icon_warden3.png";
	_program_icons_paths[PROGRAM_WATCHMAN] = "resources/board/programs/program_icon_watchman.png";
	_program_icons_paths[PROGRAM_WATCHMAN2] = "resources/board/programs/program_icon_watchman2.png";
	_program_icons_paths[PROGRAM_WATCHMAN3] = "resources/board/programs/program_icon_watchman3.png";
	_program_icons_paths[PROGRAM_NUM_PROGTYPES] = "";
	_program_icons_paths[PROGRAM_CUSTOM] = "";
	_program_icons_paths[PROGRAM_NONE] = "";

	for (int i = 0; i < PROGRAM_NUM_PROGTYPES; i++) {
		_program_icons[i] = loadTexture(_program_icons_paths[i]);
	}

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
	_main_campaign_classic = "resources/main/campaign_classic_icon.png";
	_main_campaign_nightfall = "resources/main/campaign_nightfall_icon.png";
	_main_campaign_procedural = "resources/main/campaign_procedural_icon.png";
	_main_campaign_bkg = "resources/main/campaign_icon.png";

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
	_color_clear = { 0, 0, 0, 0 };
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

	// achievements
	_achievement_badge = loadTexture("resources/achievements/plaque.png");
	_achievement_badge_large = loadTexture("resources/achievements/plaque_large.png");

	_achievement_locked = loadTexture("resources/achievements/achievement_locked.png");
	_achievement_texture[ACHIEVEMENT_FIRSTATTEMPTEDCYBERCRIME] = loadTexture("resources/achievements/game_started.png");
	_achievement_texture[ACHIEVEMENT_FIRSTCOMMITTEDCYBERCRIME] = loadTexture("resources/achievements/game_finished.png");
	_achievement_texture[ACHIEVEMENT_FIRSTSUCCESSFULCYBERCRIME] = loadTexture("resources/achievements/game_won.png");
	_achievement_texture[ACHIEVEMENT_BITCOIN] = loadTexture("resources/achievements/credits_bitcoin.png");
	_achievement_texture[ACHIEVEMENT_BITCOINCASH] = loadTexture("resources/achievements/credits_bitcoincash.png");
	_achievement_texture[ACHIEVEMENT_RIPPLE] = loadTexture("resources/achievements/credits_ripple.png");
	_achievement_texture[ACHIEVEMENT_ETHEREUM] = loadTexture("resources/achievements/credits_ethereum.png");
	_achievement_texture[ACHIEVEMENT_LITECOIN] = loadTexture("resources/achievements/credits_litecoin.png");
	_achievement_texture[ACHIEVEMENT_RAIBLOCKS] = loadTexture("resources/achievements/credits_raiblocks.png");
	_achievement_texture[ACHIEVEMENT_DOGECOIN] = loadTexture("resources/achievements/credits_dogecoin.png");
	_achievement_texture[ACHIEVEMENT_GARLICOIN] = loadTexture("resources/achievements/credits_garlicoin.png");
	_achievement_texture[ACHIEVEMENT_PARTICIPATION] = loadTexture("resources/achievements/thumbs.png");

	_achievement_name[ACHIEVEMENT_FIRSTATTEMPTEDCYBERCRIME] = "First Attempted Cybercrime";
	_achievement_name[ACHIEVEMENT_FIRSTCOMMITTEDCYBERCRIME] = "First Committed Cybercrime";
	_achievement_name[ACHIEVEMENT_FIRSTSUCCESSFULCYBERCRIME] = "First Successful Cybercrime";
	_achievement_name[ACHIEVEMENT_BABYHACKER] = "Baby Hacker";
	_achievement_name[ACHIEVEMENT_CASUALHACKER] = "Casual Hacker";
	_achievement_name[ACHIEVEMENT_AMATEURHACKER] = "Amateur Hacker";
	_achievement_name[ACHIEVEMENT_EXPERIENCEDHACKER] = "Experienced Hacker";
	_achievement_name[ACHIEVEMENT_EXPERTHACKER] = "Expert Hacker";
	_achievement_name[ACHIEVEMENT_PROFESSIONALHACKER] = "Professional Hacker";
	_achievement_name[ACHIEVEMENT_HACKERMAN] = "Hackerman";
	_achievement_name[ACHIEVEMENT_BITCOIN] = "Bitcoin";
	_achievement_name[ACHIEVEMENT_BITCOINCASH] = "Bitcoin Cash";
	_achievement_name[ACHIEVEMENT_RIPPLE] = "Ripple";
	_achievement_name[ACHIEVEMENT_ETHEREUM] = "Ethereum";
	_achievement_name[ACHIEVEMENT_LITECOIN] = "Litecoin";
	_achievement_name[ACHIEVEMENT_RAIBLOCKS] = "Raiblocks";
	_achievement_name[ACHIEVEMENT_DOGECOIN] = "Dogecoin";
	_achievement_name[ACHIEVEMENT_GARLICOIN] = "Garlicoin";
	_achievement_name[ACHIEVEMENT_OLDSCHOOL] = "Oldschool";
	_achievement_name[ACHIEVEMENT_NIGHTFALL] = "Nightfall";
	_achievement_name[ACHIEVEMENT_DIGITALWANDERER] = "Digital Wanderer";
	_achievement_name[ACHIEVEMENT_DIGITALEXPLORER] = "Digital Explorer";
	_achievement_name[ACHIEVEMENT_DIGITALCARTOGRAPHER] = "Digital Cartographer";
	_achievement_name[ACHIEVEMENT_DIGITALPIONEER] = "Digital Pioneer";
	_achievement_name[ACHIEVEMENT_DIGITALMAGELLAN] = "Digital Magellan";
	_achievement_name[ACHIEVEMENT_PARTICIPATION] = "Particiation";
	_achievement_name[ACHIEVEMENT_ONEMINUTE] = "One Minute";
	_achievement_name[ACHIEVEMENT_FIVEMINUTES] = "Five Minutes";
	_achievement_name[ACHIEVEMENT_ONEHOUR] = "One Hour";
	_achievement_name[ACHIEVEMENT_FIVEHOURS] = "Five Hours";
	_achievement_name[ACHIEVEMENT_TENHOURS] = "Ten Hours";
	_achievement_name[ACHIEVEMENT_FIFTYHOURS] = "Fifty Hours";
	_achievement_name[ACHIEVEMENT_CENTENNIAL] = "Centennial";
	_achievement_name[ACHIEVEMENT_CASUALGAMER] = "Casual Gamer";
	_achievement_name[ACHIEVEMENT_SERIOUSCASUALGAMER] = "Serious Casual Gamer";
	_achievement_name[ACHIEVEMENT_GAMER] = "Gamer";
	_achievement_name[ACHIEVEMENT_DEDICATEDGAMER] = "Dedicated Gamer";
	_achievement_name[ACHIEVEMENT_TAKEABREAK] = "You Should Take a Break";
	_achievement_name[ACHIEVEMENT_TIMETOSTOP] = "IT'S TIME TO STOP";
	_achievement_name[ACHIEVEMENT_WHY] = "WHY";

	_achievement_description[ACHIEVEMENT_FIRSTATTEMPTEDCYBERCRIME] = "Start a campaign game";
	_achievement_description[ACHIEVEMENT_FIRSTCOMMITTEDCYBERCRIME] = "Finish a campaign game";
	_achievement_description[ACHIEVEMENT_FIRSTSUCCESSFULCYBERCRIME] = "Win a campaign game";
	_achievement_description[ACHIEVEMENT_BABYHACKER] = "Kill a program in campaign mode";
	_achievement_description[ACHIEVEMENT_CASUALHACKER] = "Kill ten programs in campaign mode";
	_achievement_description[ACHIEVEMENT_AMATEURHACKER] = "Kill one hundred programs in campaign mode";
	_achievement_description[ACHIEVEMENT_EXPERIENCEDHACKER] = "Kill five hundred programs in campaign mode";
	_achievement_description[ACHIEVEMENT_EXPERTHACKER] = "Kill one thousand programs in campaign mode";
	_achievement_description[ACHIEVEMENT_PROFESSIONALHACKER] = "Kill two thousand programs in campaign mode";
	_achievement_description[ACHIEVEMENT_HACKERMAN] = "Kill five thousand programs in campaign mode";
	_achievement_description[ACHIEVEMENT_BITCOIN] = "Collect a credit";
	_achievement_description[ACHIEVEMENT_BITCOINCASH] = "Collect 1k credits";
	_achievement_description[ACHIEVEMENT_RIPPLE] = "Collect 2k credits";
	_achievement_description[ACHIEVEMENT_ETHEREUM] = "Collect 5k credits";
	_achievement_description[ACHIEVEMENT_LITECOIN] = "Collect 10k credits";
	_achievement_description[ACHIEVEMENT_RAIBLOCKS] = "Collect 50k credits";
	_achievement_description[ACHIEVEMENT_DOGECOIN] = "Collect 100k credits";
	_achievement_description[ACHIEVEMENT_GARLICOIN] = "Collect 500k credits";
	_achievement_description[ACHIEVEMENT_OLDSCHOOL] = "Completed the classic campaign";
	_achievement_description[ACHIEVEMENT_NIGHTFALL] = "Completed the nightfall campaign";
	_achievement_description[ACHIEVEMENT_DIGITALWANDERER] = "Win one freeform game";
	_achievement_description[ACHIEVEMENT_DIGITALEXPLORER] = "Win ten freeform games";
	_achievement_description[ACHIEVEMENT_DIGITALCARTOGRAPHER] = "Win fifty freeform games";
	_achievement_description[ACHIEVEMENT_DIGITALPIONEER] = "Win one hundred freeform games";
	_achievement_description[ACHIEVEMENT_DIGITALMAGELLAN] = "Win five hundred freeform games";
	_achievement_description[ACHIEVEMENT_PARTICIPATION] = "Play Spybot on your computer";
	_achievement_description[ACHIEVEMENT_ONEMINUTE] = "Play Spybot for one minute total";
	_achievement_description[ACHIEVEMENT_FIVEMINUTES] = "Play Spybot for five minutes total";
	_achievement_description[ACHIEVEMENT_ONEHOUR] = "Play Spybot for one hour total";
	_achievement_description[ACHIEVEMENT_FIVEHOURS] = "Play Spybot for five hours total";
	_achievement_description[ACHIEVEMENT_TENHOURS] = "Play Spybot for ten hours total";
	_achievement_description[ACHIEVEMENT_FIFTYHOURS] = "Play Spybot for fifty hours total";
	_achievement_description[ACHIEVEMENT_CENTENNIAL] = "Play Spybot for 100 hours total";
	_achievement_description[ACHIEVEMENT_CASUALGAMER] = "Play Spybot for five minutes in one session";
	_achievement_description[ACHIEVEMENT_SERIOUSCASUALGAMER] = "Play Spybot for thirty minutes in one session";
	_achievement_description[ACHIEVEMENT_GAMER] = "Play Spybot for one hour in one session";
	_achievement_description[ACHIEVEMENT_DEDICATEDGAMER] = "Play Spybot for three hours in one session";
	_achievement_description[ACHIEVEMENT_TAKEABREAK] = "Play Spybot for five hours in one session";
	_achievement_description[ACHIEVEMENT_TIMETOSTOP] = "Play Spybot for seven hours in one session";
	_achievement_description[ACHIEVEMENT_WHY] = "Play Spybot for ten hours in one session";
}
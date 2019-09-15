#pragma once

#include "Standard.h"

// initializing function
void initData();

// titleScreen resources
extern SDL_Texture* _title_title;
extern SDL_Texture* _title_subtitle;
extern SDL_Texture* _title_company;

// mapScreen resources
extern SDL_Texture* _node_normal[8];
extern SDL_Texture* _node_normal_over[8];
extern SDL_Texture* _node_normal_selected[8];
extern SDL_Texture* _node_owned[8];
extern SDL_Texture* _node_owned_over[8];
extern SDL_Texture* _node_owned_selected[8];
extern int _node_yoffset[8];

extern SDL_Texture* _map_window_levelConfirm;
extern SDL_Texture* _map_window_levelCancel;

extern std::string _map_button_beginDatabattle_normal;
extern std::string _map_button_beginDatabattle_over;
extern std::string _map_button_cancel_normal;
extern std::string _map_button_cancel_over;

extern std::string _map_button_joingame;

// lobbyScreen resources
extern std::string _lobby_button_gamemode_coop;
extern std::string _lobby_button_gamemode_coop_over;
extern std::string _lobby_button_gamemode_ffa;
extern std::string _lobby_button_gamemode_ffa_over;
extern std::string _lobby_button_gamemode_teamdm;
extern std::string _lobby_button_gamemode_teamdm_over;
extern std::string _lobby_button_gamemode_selected;

// game resources
extern SDL_Texture* _game_backgrounds[BKG_NUM_BKGS];

extern SDL_Texture* _tile_over;
extern SDL_Texture* _tile_selected;
extern SDL_Texture* _tile_moveNorth;
extern SDL_Texture* _tile_moveEast;
extern SDL_Texture* _tile_moveSouth;
extern SDL_Texture* _tile_moveWest;
extern SDL_Texture* _tile_movePossible;
extern SDL_Texture* _tile_images[TILE_NUM_TILES];
extern SDL_Texture* _tile_actionDamage;
extern SDL_Texture* _tile_actionHeal;
extern SDL_Texture* _tile_actionSpeed;
extern SDL_Texture* _tile_actionGrid;

extern SDL_Texture* _program_core;
extern SDL_Texture* _program_core_50px;
extern SDL_Texture* _program_core_100px;
extern SDL_Texture* _program_core_vertical;
extern SDL_Texture* _program_core_horizontal;

extern SDL_Texture* _program_icons[PROGRAM_NUM_PROGTYPES];
extern std::string _program_icons_paths[PROGRAM_NUM_PROGTYPES];

extern SDL_Texture* _item_icons[ITEM_NUM_ITEMS];

extern SDL_Texture* _game_editor_brush_none;
extern SDL_Texture* _game_editor_brush_tiles;
extern SDL_Texture* _game_editor_brush_programs;
extern SDL_Texture* _game_editor_brush_deletePrograms;
extern SDL_Texture* _game_editor_brush_items;
extern SDL_Texture* _game_editor_toggle_teamView;
extern SDL_Texture* _game_editor_toggle_teamPlayer;
extern SDL_Texture* _game_editor_toggle_teamComputer;
extern SDL_Texture* _game_editor_button_clearGrid;
extern SDL_Texture* _game_editor_button_bkg[BKG_NUM_BKGS];

extern std::string _game_button_endTurn;
extern std::string _game_button_aiStep;
extern std::string _game_button_win;
extern std::string _game_button_start;

extern std::string _game_button_resume;
extern std::string _game_button_reset;
extern std::string _game_button_viewTeams;
extern std::string _game_button_quitToMap;
extern std::string _game_button_quitToMain;
extern std::string _game_button_quitToDesktop;

extern SDL_Texture* _game_icon_checkmark;
extern SDL_Texture* _game_icon_dead;
extern SDL_Texture* _game_icon_owner;
extern SDL_Texture* _game_icon_ai;
extern SDL_Texture* _game_icon_human;
extern SDL_Texture* _game_icon_currTurn;

extern SDL_Texture* _game_icon_action_attack;
extern SDL_Texture* _game_icon_action_medic;
extern SDL_Texture* _game_icon_action_unmedic;
extern SDL_Texture* _game_icon_action_slow;
extern SDL_Texture* _game_icon_action_speedup;
extern SDL_Texture* _game_icon_action_tilecreate;
extern SDL_Texture* _game_icon_action_tiledestroy;
extern SDL_Texture* _game_icon_action_maxactionsup;
extern SDL_Texture* _game_icon_action_maxactionsdown;

extern std::string _game_disp_playerturn;
extern std::string _game_disp_compturn;

// mainScreen resources
extern SDL_Texture* _main_bkgdata;
extern SDL_Texture* _main_bkgsplotch;
extern SDL_Texture* _main_bkgsplotch2;
extern std::string _main_campaign_classic;
extern std::string _main_campaign_nightfall;
extern std::string _main_campaign_procedural;
extern std::string _main_campaign_bkg;

// SDL_Mixer music
extern Mix_Music* _music_title;
extern Mix_Music* _music_map_ambient;
extern Mix_Music* _music_game1;
extern Mix_Music* _music_game2;
extern Mix_Music* _music_game3;
extern Mix_Music* _music_game4;

// SDL_Mixer sounds
extern Mix_Chunk* _sound_move_player;
extern Mix_Chunk* _sound_move_computer;
extern Mix_Chunk* _sound_action_attack;
extern Mix_Chunk* _sound_action_speed;
extern Mix_Chunk* _sound_action_heal;
extern Mix_Chunk* _sound_action_grid_damage;
extern Mix_Chunk* _sound_action_grid_fix;
extern Mix_Chunk* _sound_select;
extern Mix_Chunk* _sound_pickup_credit;
extern Mix_Chunk* _sound_pickup_files;

// colors
extern SDL_Color _color_bkg_standard;
extern SDL_Color _color_clear;
extern SDL_Color _color_black;
extern SDL_Color _color_white;
extern SDL_Color _color_action_medic;
extern SDL_Color _color_action_unmedic;
extern SDL_Color _color_action_attack;
extern SDL_Color _color_action_slow;
extern SDL_Color _color_action_speedup;
extern SDL_Color _color_action_tilecreate;
extern SDL_Color _color_action_tiledestroy;
extern SDL_Color _color_action_maxactionsup;
extern SDL_Color _color_action_maxactionsdown;

// animations
extern SDL_Texture* _particle_basic;
extern SDL_Texture* _particle_selectring;

// achievements
extern SDL_Texture* _achievement_badge;
extern SDL_Texture* _achievement_badge_large;
extern SDL_Texture* _achievement_texture[ACHIEVEMENT_NUM_ACHIEVEMENTS];
extern SDL_Texture* _achievement_locked;
extern std::string _achievement_name[ACHIEVEMENT_NUM_ACHIEVEMENTS];
extern std::string _achievement_description[ACHIEVEMENT_NUM_ACHIEVEMENTS];
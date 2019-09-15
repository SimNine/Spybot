#ifndef DATA_H
#define DATA_H

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

extern SDL_Texture* _map_button_beginDatabattle_normal;
extern SDL_Texture* _map_button_beginDatabattle_over;
extern SDL_Texture* _map_button_cancel_normal;
extern SDL_Texture* _map_button_cancel_over;

extern SDL_Texture* _map_button_joingame;

// lobbyScreen resources
extern SDL_Texture* _lobby_button_gamemode_coop;
extern SDL_Texture* _lobby_button_gamemode_coop_over;
extern SDL_Texture* _lobby_button_gamemode_ffa;
extern SDL_Texture* _lobby_button_gamemode_ffa_over;
extern SDL_Texture* _lobby_button_gamemode_teamdm;
extern SDL_Texture* _lobby_button_gamemode_teamdm_over;
extern SDL_Texture* _lobby_button_gamemode_selected;

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
extern int _program_colorDefault[PROGRAM_NUM_PROGTYPES][3];
extern int _program_maxHealthDefault[PROGRAM_NUM_PROGTYPES];
extern int _program_speedDefault[PROGRAM_NUM_PROGTYPES];
extern std::string _program_nameDefault[PROGRAM_NUM_PROGTYPES];
extern std::string _program_descriptionDefault[PROGRAM_NUM_PROGTYPES];
extern int _program_costDefault[PROGRAM_NUM_PROGTYPES];

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

extern std::string _game_disp_playerturn;
extern std::string _game_disp_compturn;

// mainScreen resources
extern SDL_Texture* _main_bkgdata;
extern SDL_Texture* _main_bkgsplotch;

extern std::string _main_button_classic;
extern std::string _main_button_classic_over;
extern std::string _main_button_nightfall;
extern std::string _main_button_nightfall_over;
extern std::string _main_button_freeform;
extern std::string _main_button_freeform_over;
extern std::string _main_button_multiplayer;
extern std::string _main_button_multiplayer_over;
extern std::string _main_button_options;
extern std::string _main_button_options_over;
extern std::string _main_button_achievements;
extern std::string _main_button_achievements_over;
extern std::string _main_button_quit;
extern std::string _main_button_quit_over;

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
extern SDL_Color _color_black;
extern SDL_Color _color_white;

#endif // DATA_H

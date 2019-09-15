#ifndef NODEICONCONTAINER_H
#define NODEICONCONTAINER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_TTF.h>

#include "Program.h"
#include "Enums.h"

class DataContainer
{
public:
    DataContainer();
    virtual ~DataContainer();

    // titleScreen resources
    SDL_Texture* title_title;
    SDL_Texture* title_subtitle;
    SDL_Texture* title_company;

    // mapScreen resources
    SDL_Texture* node_normal[8];
    SDL_Texture* node_normal_over[8];
    SDL_Texture* node_normal_selected[8];
    SDL_Texture* node_owned[8];
    SDL_Texture* node_owned_over[8];
    SDL_Texture* node_owned_selected[8];
    int node_yoffset[8];

    SDL_Texture* map_window_levelConfirm;
    SDL_Texture* map_window_levelCancel;

    SDL_Texture* map_button_beginDatabattle_normal;
    SDL_Texture* map_button_beginDatabattle_over;
    SDL_Texture* map_button_cancel_normal;
    SDL_Texture* map_button_cancel_over;

    // game resources
    SDL_Texture* game_backgrounds[BKG_NUM_BKGS];

    SDL_Texture* tile_over;
    SDL_Texture* tile_selected;
    SDL_Texture* tile_moveNorth;
    SDL_Texture* tile_moveEast;
    SDL_Texture* tile_moveSouth;
    SDL_Texture* tile_moveWest;
    SDL_Texture* tile_movePossible;
    SDL_Texture* tile_images[TILE_NUM_TILES];
    SDL_Texture* tile_actionDamage;
    SDL_Texture* tile_actionHeal;
    SDL_Texture* tile_actionSpeed;
    SDL_Texture* tile_actionGrid;

    SDL_Texture* program_core;
    SDL_Texture* program_core_50px;
    SDL_Texture* program_core_100px;
    SDL_Texture* program_core_vertical;
    SDL_Texture* program_core_horizontal;

    SDL_Texture* program_icons[PROGRAM_NUM_PROGTYPES];
    int program_colorDefault[PROGRAM_NUM_PROGTYPES][3];
    int program_maxHealthDefault[PROGRAM_NUM_PROGTYPES];
    int program_speedDefault[PROGRAM_NUM_PROGTYPES];
    std::string program_nameDefault[PROGRAM_NUM_PROGTYPES];
    std::string program_descriptionDefault[PROGRAM_NUM_PROGTYPES];
    int program_costDefault[PROGRAM_NUM_PROGTYPES];

    SDL_Texture* item_icons[ITEM_NUM_ITEMS];

    SDL_Texture* game_editor_brush_none;
    SDL_Texture* game_editor_brush_tiles;
    SDL_Texture* game_editor_brush_programs;
    SDL_Texture* game_editor_brush_deletePrograms;
    SDL_Texture* game_editor_brush_items;
    SDL_Texture* game_editor_toggle_teamView;
    SDL_Texture* game_editor_toggle_teamPlayer;
    SDL_Texture* game_editor_toggle_teamComputer;
    SDL_Texture* game_editor_button_clearGrid;
    SDL_Texture* game_editor_button_bkg[BKG_NUM_BKGS];

    std::string game_button_endTurn;
    std::string game_button_aiStep;
    std::string game_button_win;
    std::string game_button_start;

    std::string game_button_resume;
    std::string game_button_reset;
    std::string game_button_viewTeams;
    std::string game_button_quitToMap;
    std::string game_button_quitToMain;
    std::string game_button_quitToDesktop;

    SDL_Texture* game_icon_checkmark;

    std::string game_disp_playerturn;
    std::string game_disp_compturn;

    // mainScreen resources
    SDL_Texture* main_bkgdata;
    SDL_Texture* main_bkgsplotch;

    std::string main_button_classic;
    std::string main_button_classic_over;
    std::string main_button_nightfall;
    std::string main_button_nightfall_over;
    std::string main_button_freeform;
    std::string main_button_freeform_over;
    std::string main_button_options;
    std::string main_button_options_over;
    std::string main_button_achievements;
    std::string main_button_achievements_over;
    std::string main_button_quit;
    std::string main_button_quit_over;

    // SDL_Mixer music
    Mix_Music* music_title;
    Mix_Music* music_map_ambient;
    Mix_Music* music_game1;
    Mix_Music* music_game2;
    Mix_Music* music_game3;
    Mix_Music* music_game4;

    // SDL_Mixer sounds
    Mix_Chunk* sound_move_player;
    Mix_Chunk* sound_move_computer;
    Mix_Chunk* sound_action_attack;
    Mix_Chunk* sound_action_speed;
    Mix_Chunk* sound_action_heal;
    Mix_Chunk* sound_action_grid_damage;
    Mix_Chunk* sound_action_grid_fix;
    Mix_Chunk* sound_select;
    Mix_Chunk* sound_pickup_credit;
    Mix_Chunk* sound_pickup_files;
protected:
private:
};

void initData();

#endif // NODEICONCONTAINER_H

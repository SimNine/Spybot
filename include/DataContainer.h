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

    SDL_Texture* game_button_endTurn;
    SDL_Texture* game_button_aiStep;
    SDL_Texture* game_button_abandon;
    SDL_Texture* game_button_win;
    SDL_Texture* game_button_reset;
    SDL_Texture* game_button_start;
    SDL_Texture* game_button_returnToMap;

    SDL_Texture* game_disp_playerturn;
    SDL_Texture* game_disp_compturn;

    // mainScreen resources
    SDL_Texture* main_bkgdata;
    SDL_Texture* main_bkgsplotch;
    SDL_Texture* main_button_classic;
    SDL_Texture* main_button_classic_over;
    SDL_Texture* main_button_nightfall;
    SDL_Texture* main_button_nightfall_over;
    SDL_Texture* main_button_freeform;
    SDL_Texture* main_button_freeform_over;
    SDL_Texture* main_button_options;
    SDL_Texture* main_button_options_over;
    SDL_Texture* main_button_achievements;
    SDL_Texture* main_button_achievements_over;
    SDL_Texture* main_button_quit;
    SDL_Texture* main_button_quit_over;

    // SDL_Color presets
    SDL_Color* color_black;
    SDL_Color* color_white;

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
protected:
private:
};

void initData();
//void initProgram(PROGRAM, std::string)

#endif // NODEICONCONTAINER_H

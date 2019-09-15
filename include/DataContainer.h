#ifndef NODEICONCONTAINER_H
#define NODEICONCONTAINER_H

#include <SDL.h>
#include <SDL_mixer.h>

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

    SDL_Texture* level_confirm_window;
    SDL_Texture* level_cancel_window;

    SDL_Texture* begin_databattle_button_normal;
    SDL_Texture* begin_databattle_button_over;
    SDL_Texture* cancel_button_normal;
    SDL_Texture* cancel_button_over;

    // game resources
    SDL_Texture* level_backgrounds[BKG_NUM_BKGS];
    SDL_Texture* level_bkg_buttons[BKG_NUM_BKGS];

    SDL_Texture* tile_over;
    SDL_Texture* tile_selected;
    SDL_Texture* tile_images[TILE_NUM_TILES];

    SDL_Texture* program_core;
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

    SDL_Texture* editorBrushNone;
    SDL_Texture* editorBrushTiles;
    SDL_Texture* editorBrushPrograms;
    SDL_Texture* editorBrushDeletePrograms;
    SDL_Texture* editorBrushItems;
    SDL_Texture* editorTeamViewToggle;
    SDL_Texture* editorTeamPlayer;
    SDL_Texture* editorTeamComputer;
    SDL_Texture* editorClearGrid;

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

    // SDL_Mixer sounds
    Mix_Music* music_title;
    Mix_Music* music_map_ambient;
    Mix_Chunk* music_beep;
    Mix_Chunk* music_boop;
protected:
private:
};

void initData();
//void initProgram(PROGRAM, std::string)

#endif // NODEICONCONTAINER_H

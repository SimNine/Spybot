#ifndef NODEICONCONTAINER_H
#define NODEICONCONTAINER_H

#include <SDL.h>

#include "Program.h"
#include "Enums.h"

class DataContainer
{
public:
    DataContainer();
    virtual ~DataContainer();

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
    int program_color[PROGRAM_NUM_PROGTYPES][3];
    int maxHealthDefault[PROGRAM_NUM_PROGTYPES];
    int speedDefault[PROGRAM_NUM_PROGTYPES];

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
protected:
private:
};

void initData();

#endif // NODEICONCONTAINER_H

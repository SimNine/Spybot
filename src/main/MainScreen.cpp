#include "MainScreen.h"
#include "Global.h"
#include "GUIButton.h"
#include "GUITexture.h"
#include "GUISlider.h"

MainScreen::MainScreen()
    : GUIContainer(ANCHOR_TOP_LEFT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL)
{
    glowList = NULL;

    GUITexture* main_title = new GUITexture(ANCHOR_TOP_LEFT, 20, 20, dataContainer->title_title, 499, 42, this);
    addObject(main_title);
    GUITexture* main_subtitle = new GUITexture(ANCHOR_TOP_LEFT, 20, 82, dataContainer->title_subtitle, 390, 10, this);
    addObject(main_subtitle);

    //GUISlider* exp_slider = new GUISlider(ANCHOR_CENTER, 0, 0, 300, 50, this,
    //                                      []( float d ){ Mix_VolumeMusic(d*128); });
    //addObject(exp_slider);

    int ln = 1;
    GUIButton* button_quit = new GUIButton(ANCHOR_BOTTOM_LEFT, 20, -(41 + 20)*ln++, 73, 41, this,
                                           []()
    {
        Mix_PlayChannel(-1, dataContainer->music_beep, 0);
        quit = true;
    },
    dataContainer->main_button_quit,
    dataContainer->main_button_quit_over);
    addObject(button_quit);

    GUIButton* button_options = new GUIButton(ANCHOR_BOTTOM_LEFT, 20, -(41 + 20)*ln++, 138, 41, this,
            []()
    {
        printf("placeholder: goto options");
        Mix_PlayChannel(-1, dataContainer->music_beep, 0);
    },
    dataContainer->main_button_options,
    dataContainer->main_button_options_over);
    addObject(button_options);

    GUIButton* button_achievements = new GUIButton(ANCHOR_BOTTOM_LEFT, 20, -(41 + 20)*ln++, 242, 41, this,
            []()
    {
        printf("placeholder: goto achievs");
        Mix_PlayChannel(-1, dataContainer->music_beep, 0);
    },
    dataContainer->main_button_achievements,
    dataContainer->main_button_achievements_over);
    addObject(button_achievements);

    GUIButton* button_freeform = new GUIButton(ANCHOR_BOTTOM_LEFT, 20, -(41 + 20)*ln++, 320, 41, this,
            []()
    {
        printf("placeholder: goto freeform map");
        Mix_PlayChannel(-1, dataContainer->music_beep, 0);
    },
    dataContainer->main_button_freeform,
    dataContainer->main_button_freeform_over);
    addObject(button_freeform);

    GUIButton* button_nightfall = new GUIButton(ANCHOR_BOTTOM_LEFT, 20, -(41 + 20)*ln++, 349, 41, this,
            []()
    {
        printf("placeholder: goto nightfall campaign map");
        Mix_PlayChannel(-1, dataContainer->music_beep, 0);
    },
    dataContainer->main_button_nightfall,
    dataContainer->main_button_nightfall_over);
    addObject(button_nightfall);

    GUIButton* button_classic = new GUIButton(ANCHOR_BOTTOM_LEFT, 20, -(41 + 20)*ln++, 315, 41, this,
            []()
    {
        currScreen = mapScreen;
        Mix_PlayMusic(dataContainer->music_map_ambient, -1);
        Mix_PlayChannel(-1, dataContainer->music_beep, 0);
    },
    dataContainer->main_button_classic,
    dataContainer->main_button_classic_over);
    addObject(button_classic);

}

MainScreen::~MainScreen()
{
    //dtor
}

void MainScreen::draw()
{
    // clear the screen (black)
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 40, 0);
    SDL_RenderClear(gRenderer);

    // draw all particulates
    LinkedList<MainScreenGlow*>* currNode = glowList;
    while (currNode != NULL)
    {
        currNode->getContents()->draw();
        currNode = currNode->getNext();
    }

    // draw the text chunk
    SDL_Rect destRect;
    SDL_QueryTexture(dataContainer->main_bkgdata, NULL, NULL, &destRect.w, &destRect.h);
    for (int x = 0; x < SCREEN_WIDTH; x += destRect.w)
    {
        for (int y = 0; y < SCREEN_HEIGHT; y += destRect.h)
        {
            destRect.x = x;
            destRect.y = y;
            SDL_RenderCopy(gRenderer, dataContainer->main_bkgdata, NULL, &destRect);
        }
    }

    drawContents();
}

void MainScreen::tick()
{
    // tick all GUIObjects
    GUIContainer::tick();

    // tick all particulates
    LinkedList<MainScreenGlow*>* currNode = glowList;
    while (currNode != NULL)
    {
        currNode->getContents()->tick();
        currNode = currNode->getNext();
    }

    // check for dead/OOB particulates
    if (glowList == NULL)
    {
        // do nothing
    }
    else if (glowList->getContents()->getTransparent() == true)
    {
        LinkedList<MainScreenGlow*>* tempNode = glowList;
        glowList = glowList->getNext();
        delete tempNode->getContents();
        delete tempNode;
    }
    else if (glowList->getContents()->getXPos() + 200 < 0 ||
             glowList->getContents()->getYPos() + 200 < 0 ||
             glowList->getContents()->getXPos() > SCREEN_WIDTH ||
             glowList->getContents()->getYPos() > SCREEN_HEIGHT)
    {
        LinkedList<MainScreenGlow*>* tempNode = glowList;
        glowList = glowList->getNext();
        delete tempNode->getContents();
        delete tempNode;
    }

    // 2% chance of adding a particulate
    if (rand() % 10 == 0)
    {
        MainScreenGlow* newGlow = new MainScreenGlow(rand()%SCREEN_WIDTH, rand()%SCREEN_HEIGHT);
        if (glowList == NULL)
        {
            glowList = new LinkedList<MainScreenGlow*>(newGlow);
        }
        else
        {
            LL_addObject(glowList, newGlow);
        }
    }

    //printf("num particles: %i\n", LL_getLength(glowList));
}

#include "TitleScreen.h"
#include "Global.h"
#include "GUITexture.h"

TitleScreen::TitleScreen()
    : GUIContainer(ANCHOR_TOP_LEFT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL)
{
    int w;
    int h;
    SDL_QueryTexture(dataContainer->title_company, NULL, NULL, &w, &h);
    addObject(new GUITexture(ANCHOR_CENTER, -w/2, -h/2, dataContainer->title_company, w, h, this));
    SDL_QueryTexture(dataContainer->title_title, NULL, NULL, &w, &h);
    addObject(new GUITexture(ANCHOR_CENTER, -w/2, -h, dataContainer->title_title, w, h, this));
    SDL_QueryTexture(dataContainer->title_subtitle, NULL, NULL, &w, &h);
    addObject(new GUITexture(ANCHOR_CENTER, -w/2, 20, dataContainer->title_subtitle, w, h, this));

    tickCount = 0;

    timingCompany = 300;
    timingTitle = 810;
    timingSubtitle = 910;
}

TitleScreen::~TitleScreen()
{
    //dtor
}

void TitleScreen::draw()
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);
    SDL_RenderClear(gRenderer);

    drawContents();

    if (debug) drawBounds();
}

void TitleScreen::tick()
{
    // tick all GUIObjects
    GUIContainer::tick();

    if (tickCount <= timingCompany - 255)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_company, 0);
    }
    else if (tickCount > timingCompany - 255 && tickCount <= timingCompany)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_company, tickCount - timingCompany + 255);
    }
    else if (tickCount > timingCompany && tickCount <= timingCompany + 255)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_company, timingCompany - tickCount + 255);
    }
    else if (tickCount > timingCompany + 255)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_company, 0);
    }

    if (tickCount <= timingTitle - 255)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_title, 0);
    }
    else if (tickCount > timingTitle - 255 && tickCount <= timingTitle)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_title, tickCount - timingTitle + 255);
    }
    else if (tickCount > timingTitle && tickCount <= timingTitle + 255)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_title, timingTitle - tickCount + 255);
    }
    else if (tickCount > timingTitle + 255)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_title, 0);
    }

    if (tickCount <= timingSubtitle - 255) // before fade in
    {
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, 0);
    }
    else if (tickCount > timingSubtitle - 255 && tickCount <= timingSubtitle) // fading in
    {
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, tickCount - timingSubtitle + 255);
    }
    else if (tickCount > timingSubtitle && tickCount <= timingSubtitle + 255) // fading out
    {
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, timingSubtitle - tickCount + 255);
    }
    else if (tickCount > timingSubtitle + 255)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, 0);
    }

    if (tickCount > timingSubtitle + 305)
    {
        endTitles();
    }

    tickCount++;
}

bool TitleScreen::mouseDown()
{
    advance();
    return true;
}

void TitleScreen::advance()
{
    if (tickCount < timingCompany) tickCount = timingCompany;
    else if (tickCount < timingTitle) tickCount = timingTitle;
    else endTitles();
}

void TitleScreen::endTitles()
{
        currScreen = mainScreen;
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, 255);
        SDL_SetTextureAlphaMod(dataContainer->title_title, 255);
        SDL_ShowCursor(SDL_ENABLE);
        Mix_PlayMusic(dataContainer->music_title, -1);
}

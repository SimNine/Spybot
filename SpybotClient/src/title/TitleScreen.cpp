#include "Standard.h"
#include "TitleScreen.h"

#include "Global.h"
#include "GUITexture.h"
#include "DataContainer.h"
#include "MainScreen.h"

TitleScreen::TitleScreen()
    : GUIContainer(ANCHOR_NORTHWEST, {0, 0}, {SCREEN_WIDTH, SCREEN_HEIGHT}, NULL, NULL)
{
    int w;
    int h;
    SDL_QueryTexture(dataContainer->title_company, NULL, NULL, &w, &h);
    addObject(new GUITexture(ANCHOR_CENTER, {-w/2, -h/2}, dataContainer->title_company, {w, h}, this));
    SDL_QueryTexture(dataContainer->title_title, NULL, NULL, &w, &h);
    addObject(new GUITexture(ANCHOR_CENTER, {-w/2, -h}, dataContainer->title_title, {w, h}, this));
    SDL_QueryTexture(dataContainer->title_subtitle, NULL, NULL, &w, &h);
    addObject(new GUITexture(ANCHOR_CENTER, {-w/2, 20}, dataContainer->title_subtitle, {w, h}, this));

    //sprites = new LinkedList<GUITexture*>();

    tickCount = 0;

    timingCompany = 3000;
    timingTitle = 8000;
    timingSubtitle = 9000;

    spanTiming = 2500;
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
}

void TitleScreen::tick(int ms)
{
    // tick all GUIObjects
    GUIContainer::tick(ms);

    if (tickCount <= timingCompany - spanTiming)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_company, 0);
    }
    else if (tickCount > timingCompany - spanTiming && tickCount <= timingCompany)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_company, (int)(((double)(tickCount - timingCompany + spanTiming)/spanTiming)*255.0));
    }
    else if (tickCount > timingCompany && tickCount <= timingCompany + spanTiming)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_company, (int)(((double)(timingCompany - tickCount + spanTiming)/spanTiming)*255.0));
    }
    else if (tickCount > timingCompany + spanTiming)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_company, 0);
    }

    if (tickCount <= timingTitle - spanTiming)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_title, 0);
    }
    else if (tickCount > timingTitle - spanTiming && tickCount <= timingTitle)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_title, (int)(((double)(tickCount - timingTitle + spanTiming)/spanTiming)*255.0));
    }
    else if (tickCount > timingTitle && tickCount <= timingTitle + spanTiming)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_title, (int)(((double)(timingTitle - tickCount + spanTiming)/spanTiming)*255.0));
    }
    else if (tickCount > timingTitle + spanTiming)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_title, 0);
    }

    if (tickCount <= timingSubtitle - spanTiming) // before fade in
    {
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, 0);
    }
    else if (tickCount > timingSubtitle - spanTiming && tickCount <= timingSubtitle) // fading in
    {
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, (int)(((double)(tickCount - timingSubtitle + spanTiming)/spanTiming)*255.0));
    }
    else if (tickCount > timingSubtitle && tickCount <= timingSubtitle + spanTiming) // fading out
    {
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, (int)(((double)(timingSubtitle - tickCount + spanTiming)/spanTiming)*255.0));
    }
    else if (tickCount > timingSubtitle + spanTiming)
    {
        SDL_SetTextureAlphaMod(dataContainer->title_subtitle, 0);
    }

    if (tickCount > timingSubtitle + spanTiming + 500)
    {
        endTitles();
    }

    tickCount += ms;
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

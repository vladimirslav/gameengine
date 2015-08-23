#include "uiimage.h"

UiImage::UiImage(int x,
    int y,
    size_t w,
    size_t h,
    Graph& g,
    SDL_Color borderColor,
    size_t borderWidth,
    sprite_id sprite)
    : UiObject(x, y, w, h, g, {}, borderColor, borderWidth, 0)
    , sprite(sprite)
    , showPart(false)
    , displayedArea({})
{

}

UiImage::UiImage(int x,
    int y,
    size_t w,
    size_t h,
    Graph& g,
    SDL_Color borderColor,
    SDL_Rect  displayedArea,
    size_t borderWidth,
    sprite_id sprite)
    : UiObject(x, y, w, h, g, {}, borderColor, borderWidth, 0)
    , sprite(sprite)
    , showPart(true)
    , displayedArea(displayedArea)
{

}

void UiImage::Draw()
{
    StartDraw();
    if (showPart)
    {
        g->DrawTexture(x, y, g->GetTexture(sprite), &displayedArea, 0, SDL_FLIP_NONE);
    }
    else
    {
        g->DrawTextureStretched(x, y, width, height, g->GetTexture(sprite));
    }
    UiObject::Draw();
    EndDraw();
}
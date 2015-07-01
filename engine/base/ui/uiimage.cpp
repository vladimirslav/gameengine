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
{

}

void UiImage::Draw()
{
    g->DrawTextureStretched(x, y, width, height, g->GetTexture(sprite));
}
#include "uibutton.h"

UiButton::UiButton(int x,
                   int y,
                   size_t w,
                   size_t h,
                   Graph& g,
                   SDL_Color color,
                   SDL_Color borderColor,
                   size_t borderWidth,
                   size_t fontId,
                   std::string text)
    : UiLabel(x, y, w, h, g, color, borderColor, borderWidth, fontId, text)
    , hasBg(false)
    , flip(SDL_FLIP_NONE)
{
}

UiButton::UiButton(int x,
                   int y,
                   size_t w,
                   size_t h,
                   Graph& g,
                   SDL_Color color,
                   SDL_Color borderColor,
                   size_t borderWidth,
                   size_t fontId,
                   std::string text,
                   sprite_id bg,
                   SDL_RendererFlip flip)
    : UiLabel(x, y, w, h, g, color, borderColor, borderWidth, fontId, text)
    , hasBg(true)
    , bg(bg)
    , flip(flip)
{

}

void UiButton::Draw()
{
    StartDraw();
    if (hasBg)
    {
        g->DrawTexture(x, y, bg, nullptr, 0, flip);
    }
    else
    {
        UiLabel::Draw();
    }
    EndDraw();
}
#include "uibutton.h"

UiButton::UiButton(int x,
                   int y,
                   size_t w,
                   size_t h,
                   Graph& g,
                   SDL_Color color,
				   SDL_Color mouseOverColor,
                   SDL_Color borderColor,
                   size_t borderWidth,
                   const FontDescriptor* fontId,
                   std::string text)
	: UiLabel(x, y, w, h, g, color, mouseOverColor, borderColor, borderWidth, fontId, text)
    , hasBg(false)
    , stretchBg(false)
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
                   sprite_id bg,
                   bool stretchBg,
                   SDL_RendererFlip flip)
	: UiLabel(x, y, w, h, g, color, {}, borderColor, borderWidth, 0, "")
    , hasBg(true)
    , bg(bg)
    , stretchBg(stretchBg)
    , flip(flip)
    , colorFilter({255, 255, 255, 255})
{

}

void UiButton::Draw()
{
    StartDraw();
    if (hasBg)
    {
        g->PushTextureColorValues(colorFilter.r, colorFilter.g, colorFilter.b);
        if (stretchBg)
        {
            g->DrawTextureStretched(x, y, width, height, g->GetTexture(bg));
        }
        else
        {
            g->DrawTexture(x, y, bg, nullptr, 0, flip);
        }
        g->PopTextureColorValue();
    }
    else
    {
        UiLabel::Draw();
    }
    EndDraw();
}

void UiButton::SetColorFilter(const SDL_Color& rgb)
{
    colorFilter.r = rgb.r;
    colorFilter.g = rgb.g;
    colorFilter.b = rgb.b;
}

void UiButton::RestoreColorFilter()
{
    colorFilter.r = 255;
    colorFilter.g = 255;
    colorFilter.b = 255;
}

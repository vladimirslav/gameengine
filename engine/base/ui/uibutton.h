#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "uilabel.h"

class UiButton : public UiLabel
{
protected:
    bool hasBg;
    bool stretchBg;
    sprite_id bg;
    SDL_RendererFlip flip;
    SDL_Color colorFilter;
public:
    UiButton(int x,
             int y,
             size_t w,
             size_t h,
             Graph& g,
             SDL_Color color,
             SDL_Color borderColor,
             size_t borderWidth,
             const FontDescriptor* fontId,
             std::string text);


    UiButton(int x,
             int y,
             size_t w,
             size_t h,
             Graph& g,
             SDL_Color color,
             SDL_Color borderColor,
             size_t borderWidth,
             sprite_id bg,
             bool stretchBg,
             SDL_RendererFlip flip);

    virtual void Draw();

    virtual void SetColorFilter(const SDL_Color& rgb);
    virtual void RestoreColorFilter();
};

#endif
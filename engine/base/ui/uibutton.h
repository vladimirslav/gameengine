#ifndef __UIBUTTON_H__
#define __UIBUTTON_H__

#include "uilabel.h"

class UiButton : public UiLabel
{
public:
    UiButton(int x,
             int y,
             size_t w,
             size_t h,
             Graph& g,
             SDL_Color color,
             SDL_Color borderColor,
             size_t borderWidth,
             size_t fontId,
             std::string text);
};

#endif
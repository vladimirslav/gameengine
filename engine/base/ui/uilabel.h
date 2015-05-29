#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "../uiobject.h"
#include <string>

class UiLabel : public UiObject
{
protected:
    std::string text;
public:
    UiLabel(int x,
            int y,
            size_t w,
            size_t h,
            Graph& g,
            SDL_Color color,
            SDL_Color borderColor,
            size_t borderWidth,
            size_t fontId,
            std::string text);

    virtual void Draw();

    virtual void setText(std::string newText);
};

#endif
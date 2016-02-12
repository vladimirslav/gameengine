#ifndef __UILABEL_H__
#define __UILABEL_H__

#include "../uiobject.h"
#include <string>

extern const SDL_Color UI_COLOR_WHITE;
extern const SDL_Color UI_COLOR_BLACK;
extern const SDL_Color UI_COLOR_RED;
extern const SDL_Color UI_COLOR_GREEN;
extern const SDL_Color UI_COLOR_DARK_GREEN;

class UiLabel : public UiObject
{
protected:
    std::string text;
	SDL_Color mouseOverColor;
public:
    UiLabel(int x,
            int y,
            size_t w,
            size_t h,
            Graph& g,
            SDL_Color color,
			SDL_Color mouseOverColor,
            SDL_Color borderColor,
            size_t borderWidth,
            const FontDescriptor* fontId,
            std::string text);

    virtual void Draw();

    virtual void setText(std::string newText);
};

#endif
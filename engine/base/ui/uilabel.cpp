#include "uilabel.h"

UiLabel::UiLabel(int x,
                 int y,
                 size_t w,
                 size_t h,
                 Graph& g,
                 SDL_Color color,
                 SDL_Color borderColor,
                 size_t borderWidth,
                 size_t fontId,
                 std::string text)
    : UiObject(x, y, w, h, g, color, borderColor, borderWidth, fontId)
    , text(text)
{

}

void UiLabel::Draw()
{
    g->WriteNormal(mainfont, text, x, y, color);
}
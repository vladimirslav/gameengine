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
{
}

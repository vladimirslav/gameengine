#include "uilabel.h"

UiLabel::UiLabel(int x,
                 int y,
                 size_t w,
                 size_t h,
                 Graph& g,
                 SDL_Color color,
                 SDL_Color borderColor,
                 size_t borderWidth,
                 const FontDescriptor* fontId,
                 std::string text)
    : UiObject(x, y, w, h, g, color, borderColor, borderWidth, fontId)
    , text(text)
{

}

void UiLabel::Draw()
{
    if (text.empty() == false)
    {
        g->WriteParagraph(*mainfont,
            text,
            x,
            y,
            width - 10,
            10,
            color);
        //g->WriteNormal(*mainfont, text, x, y, color);
    }
}

void UiLabel::setText(std::string newText)
{
    text = newText;
}
#include "uilabel.h"

const SDL_Color UI_COLOR_WHITE = {255, 255, 255, 255};
const SDL_Color UI_COLOR_BLACK = {0, 0, 0, 255};
const SDL_Color UI_COLOR_RED = {255, 0, 0, 255};
const SDL_Color UI_COLOR_GREEN = {0, 255, 0, 255};
const SDL_Color UI_COLOR_BLUE = { 0, 0, 255, 255 };
const SDL_Color UI_COLOR_DARK_GREEN = { 0, 128, 0, 255 };

UiLabel::UiLabel(int x,
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
    : UiObject(x, y, w, h, g, color, borderColor, borderWidth, fontId)
    , text(text)
    , mouseOverColor(mouseOverColor)
{

}

void UiLabel::Draw()
{
    if (text.empty() == false)
    {
		SDL_Color* chosenColor = &color;
		if (mouseOver)
		{
			chosenColor = &mouseOverColor;
		}
        g->WriteParagraph(*mainfont,
            text,
            x,
            y,
            width - 10,
            10,
			*chosenColor);
        //g->WriteNormal(*mainfont, text, x, y, color);
    }
}

void UiLabel::setText(std::string newText)
{
    text = newText;
}
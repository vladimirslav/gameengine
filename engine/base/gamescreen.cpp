/*
    Author: Vladimir Slav

    This is free and unencumbered software released into the public domain.

    Anyone is free to copy, modify, publish, use, compile, sell, or
    distribute this software, either in source code form or as a compiled
    binary, for any purpose, commercial or non-commercial, and by any
    means.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/

#include "gamescreen.h"

GameScreen::GameScreen(Graph& g, size_t fontId)
    : g(&g)
	, GameWindow(0, 0, g.GetWidth(), g.GetHeight(), 0, fontId, g, { 0, 0, 0, 0 }, { 0, 0, 0, 0}, false)
{
	memset(keyStates, 0, sizeof(keyStates));
}

GameScreen* GameScreen::Process()
{
    return this;
}

void GameScreen::ProcessEvent(SDL_Event& event)
{
    switch (event.type)
    {
        case SDL_KEYDOWN:
            keyStates[event.key.keysym.scancode] = true;
            break;
        case SDL_KEYUP:
            keyStates[event.key.keysym.scancode] = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseStates[MS_LEFT] = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseStates[MS_LEFT] = false;
            }
            break;
    }
}

void GameScreen::Draw()
{
	GameWindow::Draw();
	g->Flip();
}

size_t GameScreen::GetCurrentFont()
{
    return mainfont;
}

Graph* GameScreen::GetGraph()
{
    return g;
}

Button::Button(int x, int y, int width, int height, const std::string& label)
    : x(x)
    , y(y)
    , width(width)
    , height(height)
    , label(label)
{

}

TextButton::TextButton(int x, int y, int width, int height, const std::string& label)
    : Button(x, y, width, height, label)
{

}

OptionButton::OptionButton(int x, int y, int width, int height, const std::string& label, const std::string& value)
    : Button(x, y, width, height, label)
{

}

SelectionScreen::SelectionScreen(Graph& g,
                                 int max_pos,
                                 int initial_pos,
                                 size_t top_button_margin,
                                 size_t left_button_margin,
                                 size_t button_width,
                                 size_t button_height,
                                 const char* button_text[],
                                 size_t button_amount,
                                 font_id font)
    : GameScreen(g, font)
    , maxPos(max_pos)
    , currentPos(initial_pos)
    , topButtonMargin(top_button_margin)
    , leftButtonMargin(left_button_margin)
    , buttonWidth(button_width)
    , buttonHeight(button_height)
    , buttons(nullptr)
    , buttonText(button_text)
    , buttonAmount(button_amount)
{
    PopulateButtons();
}

void SelectionScreen::DrawButton(int index)
{
    g->WriteNormal(mainfont, buttons[index]->label, buttons[index]->x, buttons[index]->y);
}

void SelectionScreen::ProcessEvent(SDL_Event& event)
{
    GameScreen::ProcessEvent(event);
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.scancode == SDL_SCANCODE_DOWN ||
            event.key.keysym.scancode == SDL_SCANCODE_S)
        {
            if (currentPos >= maxPos)
            {
                currentPos = 0;
            }
            else
            {
                currentPos += 1;
            }
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_UP ||
                 event.key.keysym.scancode == SDL_SCANCODE_W)
        {
            if (currentPos <= 0)
            {
                currentPos = maxPos;
            }
            else
            {
                currentPos -= 1;
            }
        }
    }
    else if (event.type == SDL_MOUSEMOTION)
    {
        keyStates[SDL_SCANCODE_RETURN] = false;
        mousex = event.motion.x;
        mousey = event.motion.y;
        for (size_t i = 0; i < buttonAmount; i++)
        {
            if (buttons[i]->x <= mousex && buttons[i]->x + buttons[i]->width  >= mousex &&
                buttons[i]->y <= mousey && buttons[i]->y + buttons[i]->height >= mousey)
            {
                currentPos = i;
                break;
            }
        }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (buttons[currentPos]->x <= mousex && 
            buttons[currentPos]->x + buttons[currentPos]->width >= mousex &&
            buttons[currentPos]->y <= mousey && 
            buttons[currentPos]->y + buttons[currentPos]->height >= mousey)
        {
            keyStates[SDL_SCANCODE_RETURN] = true;
        }
    }
}

void SelectionScreen::PopulateButtons()
{
    if (buttons != nullptr)
    {
        delete [] buttons;
    }

    buttons = new Button*[buttonAmount];

    for (size_t i = 0; i < buttonAmount; i++)
    {
        buttons[i] = new TextButton(leftButtonMargin, topButtonMargin + i * buttonHeight, buttonWidth, buttonHeight, buttonText[i]);
    }
}

GameScreen* SelectionScreen::PushButton(size_t index)
{
    return this;
}

SelectionScreen::~SelectionScreen()
{
    delete [] buttons;
}
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

#ifndef __GAMESCREEN_H__
#define __GAMESCREEN_H__

#include "graph.h"
#include "window.h"

enum MouseStates
{
    MS_LEFT,
    MS_RIGHT,
    MS_MIDDLE,
    MS_MAX
};

class GameScreen : public EngineWindow::GameWindow
{
protected:
    Graph* g;

	bool keyStates[SDL_NUM_SCANCODES];
    bool mouseStates[MS_MAX];

    int mousex;
    int mousey;
public:
	GameScreen(Graph& g, const FontDescriptor* fontId);
    virtual GameScreen* Process();
    virtual void StartDraw();
    virtual void EndDraw();
    virtual void Draw();
    virtual const FontDescriptor* GetCurrentFont();
    virtual Graph* GetGraph();
    virtual ~GameScreen() {};

    virtual void ProcessEvent(SDL_Event& event);
};


class Button
{
public:
    Button(int x, int y, int width, int height, const std::string& label);
    int x;
    int y;
    int width;
    int height;
    std::string label;
};

class TextButton : public Button
{
public:
    TextButton(int x, int y, int width, int height, const std::string& label);
};

class OptionButton : public Button
{
public:
    std::string value;
    OptionButton(int x, int y, int width, int height, const std::string& label, const std::string& value);
};

class SelectionScreen : public GameScreen
{
protected:
    int maxPos;
    int currentPos;

    size_t topButtonMargin;
    size_t leftButtonMargin;
    size_t buttonWidth;
    size_t buttonHeight;

    size_t buttonAmount;

    Button** buttons;
    const char** buttonText;

public:
    SelectionScreen(Graph& g,
                    int max_pos, 
                    int initial_pos,
                    size_t top_button_margin,
                    size_t left_button_margin,
                    size_t button_width,
                    size_t button_height, 
                    const char* button_text[],
                    size_t button_amount,
                    FontDescriptor* desc);

    virtual void PopulateButtons();
    virtual void DrawButton(int index);
    virtual void ProcessEvent(SDL_Event& event);
    virtual GameScreen* PushButton(size_t index);
    virtual ~SelectionScreen();
};

#endif

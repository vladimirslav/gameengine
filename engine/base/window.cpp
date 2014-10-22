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

#include "window.h"
#include <vector>
#include <algorithm>

namespace EngineWindow
{

    static ConfirmationChoice lastChoice = ConfirmationChoice::NONE;

    static std::vector<GameWindow*> windows;

    GameWindow::GameWindow(int x,
                           int y,
                           size_t w,
                           size_t h,
                           size_t borderWidth,
                           size_t fontId,
                           Graph& g,
                           SDL_Color color,
                           SDL_Color borderColor)
        : x(x)
        , y(y)
        , width(w)
        , height(h)
        , g(&g)
        , color(color)
        , borderColor(borderColor)
        , borderWidth(borderWidth)
        , fontId(fontId)
    {
        int tmp = 0;
        g.GetTextSize(fontId, "Test", &tmp, &fontHeight);
        windows.push_back(this);
    }

    GameWindow::~GameWindow()
    {
        auto addr = std::find(windows.begin(), windows.end(), this);
        //technically, this should always happen
        if (addr != windows.end())
        {
            windows.erase(addr);
        }
    }

    void GameWindow::Draw()
    {
        g->SetViewPort(0, 0, g->GetWidth(), g->GetHeight());
        g->DrawRect(x, y, width, height, color);
        for (size_t i = 0; i < borderWidth; i++)
        {
            // horizontal lines
            g->DrawLine(x, y + i, x + width, y + i, borderColor);
            g->DrawLine(x, y + height - i, x + width, y + height - i, borderColor);

            // vertical lines
            g->DrawLine(x + i, y, x + i, y + height, borderColor);
            g->DrawLine(x + width - i, y, x + width - i, y + height, borderColor);
        }
    }

    void GameWindow::Update(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            delete this;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                delete this;
                break;
            }
        }
    }

    NotificationWindow::NotificationWindow(int x,
                                           int y,
                                           size_t w,
                                           size_t h,
                                           size_t borderWidth,
                                           Graph& g,
                                           SDL_Color color,
                                           SDL_Color borderColor,
                                           std::string message,
                                           SDL_Color textColor,
                                           size_t fontId)
        : GameWindow(x, y, w, h, borderWidth, fontId, g, color, borderColor)
        , message(message)
        , textColor(textColor)
        , fontBorder(10)
    {

    }

    NotificationWindow::NotificationWindow(int x,
                                           int y,
                                           size_t borderWidth,
                                           Graph& g,
                                           SDL_Color color,
                                           SDL_Color borderColor,
                                           size_t fontBorder,
                                           std::string message,
                                           SDL_Color textColor,
                                           size_t fontId)
        : GameWindow(x, y, 0, 0, borderWidth, fontId, g, color, borderColor)
        , message(message)
        , textColor(textColor)
        , fontBorder(fontBorder)
    {
        int newW;
        int newH;
        g.GetTextSize(fontId, message, &newW, &newH);

        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH;
    }

    void NotificationWindow::Draw()
    {
        GameWindow::Draw();
        g->WriteParagraph(fontId, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, textColor);
    }

    void NotificationWindow::Update(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            delete this;
            break;
        case SDL_KEYDOWN:
            delete this;
            break;
        }
    }



    BGNotificationWindow::BGNotificationWindow(int x,
        int y,
        size_t w,
        size_t h,
        size_t borderWidth,
        Graph& g,
        sprite_id bg,
        std::string message,
        SDL_Color textColor,
        size_t fontId)
        : GameWindow(x, y, w, h, borderWidth, fontId, g, { 0, 0, 0, 0 }, { 0, 0, 0, 0 })
        , message(message)
        , textColor(textColor)
        , fontBorder(borderWidth)
        , bg(bg)
    {

    }

    BGNotificationWindow::BGNotificationWindow(int x,
        int y,
        Graph& g,
        sprite_id bg,
        size_t fontBorder,
        std::string message,
        SDL_Color textColor,
        size_t fontId)
        : GameWindow(x, y, 0, 0, 0, fontId, g, { 0, 0, 0, 0 }, { 0, 0, 0, 0 })
        , message(message)
        , textColor(textColor)
        , fontBorder(fontBorder)
        , bg(bg)
    {
        int newW;
        int newH;
        g.GetTextSize(fontId, message, &newW, &newH);

        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH;
    }

    void BGNotificationWindow::Draw()
    {
        g->DrawTextureStretched(x, y, width, height, g->GetTexture(bg));
        g->WriteParagraph(fontId, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, textColor);
    }

    void BGNotificationWindow::Update(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            delete this;
            break;
        case SDL_KEYDOWN:
            delete this;
            break;
        }
    }

    ConfirmationWindow::ConfirmationWindow(int x,
                                           int y,
                                           size_t borderWidth,
                                           Graph& g,
                                           size_t fontBorder,
                                           std::string message,
                                           SDL_Color textColor,
                                           SDL_Color bgColor,
                                           SDL_Color borderColor,
                                           size_t fontId)
        : GameWindow(x, y, 0, 0, borderWidth ,fontId, g, bgColor, borderColor)
        , fontColor(textColor)
        , fontBorder(fontBorder)
        , message(message)
    {
        int newW;
        int newH;
        g.GetTextSize(fontId, message, &newW, &newH);
        int testW;
        g.GetTextSize(fontId, "OKCancel", &testW, &newH);
        testW += 40;
        if (testW > newW)
        {
            newW = testW;
        }
        width += fontBorder * 2 + newW;
        height += fontBorder * 2 + newH + 3 * fontHeight;
        g.GetTextSize(fontId, "Cancel", &newW, &cancelTextW);
    }

    void ConfirmationWindow::Draw()
    {
        GameWindow::Draw();
        g->WriteParagraph(fontId, message, x + fontBorder, y + height / 3, width - fontBorder * 2, fontBorder, fontColor);
        g->WriteNormal(fontId, "CANCEL", x + 20, y + height - fontHeight * 2, fontColor);
        g->WriteNormal(fontId, "OK", x + cancelTextW + 40, y + height - fontHeight * 2, fontColor);
        
    }

    void ConfirmationWindow::Update(const SDL_Event& event)
    {
        switch (event.type)
        {
        case SDL_QUIT:
            delete this;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                OnOK();
                delete this;
                break;
            case SDLK_RETURN:
                OnCancel();
                delete this;
                break;
            }
        }
    }

    void ConfirmationWindow::OnOK()
    {
        lastChoice = ConfirmationChoice::YES;
    }

    void ConfirmationWindow::OnCancel()
    {
        lastChoice = ConfirmationChoice::NO;
    }

    bool UpdateWindow(const SDL_Event& event)
    {
        if (windows.size() > 0)
        {
            windows.back()->Update(event);
            return true;
        }
        return false;
    }

    void DrawWindows(bool active_only)
    {
        if (active_only)
        {
            if (windows.size() > 0)
            {
                windows.back()->Draw();
            }
        }
        else
        {
            for (auto w : windows)
            {
                w->Draw();
            }
        }
    }

    void CloseAll()
    {

    }

    bool HasOpenWindows()
    {
        return windows.size() > 0;
    }

    void ResetLastChoice()
    {
        lastChoice = ConfirmationChoice::NONE;
    }

    ConfirmationChoice GetLastChoice()
    {
        return lastChoice;
    }
}

#include "window.h"
#include <vector>
#include <algorithm>

namespace EngineWindow
{

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
}
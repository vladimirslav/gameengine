#include "window.h"
#include <vector>
#include <algorithm>

namespace EngineWindow
{

    static std::vector<GameWindow*> windows;

    GameWindow::GameWindow(int x, int y, size_t w, size_t h, Graph& g, SDL_Color color)
        : x(x)
        , y(y)
        , width(w)
        , height(h)
        , g(&g)
        , color(color)
    {
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
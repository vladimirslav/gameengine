#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "graph.h"

namespace EngineWindow
{
    class GameWindow
    {
    protected:
        int x;
        int y;
        size_t width;
        size_t height;
        Graph* g;
        SDL_Color color;

    public:
        GameWindow(int x, int y, size_t w, size_t h, Graph& g, SDL_Color color);
        ~GameWindow();

        virtual void Draw();
        virtual void Update(const SDL_Event& event);
    };

    bool HasOpenWindows();
    void DrawWindows(bool active_only = true);
    bool UpdateWindow(const SDL_Event& event);
    void CloseAll();
}

#endif
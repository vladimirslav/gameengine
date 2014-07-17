#ifndef __GAMESCREEN_H__
#define __GAMESCREEN_H__

#include "graph.h"
#include "Timer.h"

class GameScreen
{
protected:
    Graph* g;
    Timer particle_timer;
    
public:
    GameScreen(Graph& g);
    virtual GameScreen* Process();
    virtual void Draw();
    virtual size_t GetCurrentFont();
    virtual int GetCurrentTime();
};

#endif
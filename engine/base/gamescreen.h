#ifndef __GAMESCREEN_H__
#define __GAMESCREEN_H__

#include "graph.h"

class GameScreen
{
protected:
    Graph* g;
public:
    GameScreen(Graph* g);
    virtual GameScreen* Process();
    virtual void Draw();
};

#endif
#include "SDL.h"

#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
private:
    int last_ticks;
    int pause_ticks;
    bool paused;
public:
    Timer();
    int GetTicks();
    void Reset();
    void Pause();
    void Unpause();
};



#endif

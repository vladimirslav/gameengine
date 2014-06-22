#include "SDL.h"

#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
private:
    size_t last_ticks;
    size_t pause_ticks;
    bool paused;
public:
    Timer();
    size_t GetTicks();
    void Reset();
    void Pause();
    void Unpause();
};



#endif

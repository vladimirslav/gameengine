#include "Timer.h"

Timer::Timer()
{
    last_ticks = SDL_GetTicks();
    paused = false;
}

int Timer::GetTicks()
{
    if (paused)
    {
        return pause_ticks;
    }
    else
    {
        return SDL_GetTicks() - last_ticks;
    }
}

void Timer::Reset()
{
    last_ticks = SDL_GetTicks();
}

void Timer::Pause()
{
    pause_ticks = SDL_GetTicks() - last_ticks;
    paused = true;
}

void Timer::Unpause()
{
    if (paused)
    {
        paused = false;
        last_ticks = SDL_GetTicks() - pause_ticks;
        pause_ticks = 0;
    }
}

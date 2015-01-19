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

#include "Timer.h"

Timer::Timer()
{
    last_ticks = SDL_GetTicks();
    paused = false;
}

size_t Timer::GetTicks() const
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

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

#include "countdown.h"
#include <map>

using namespace EngineTimer;

static std::map<const std::string, CountdownTimer> timers;

CountdownTimer::CountdownTimer()
    : counter()
    , start_time(0)
    , duration(0)
    , is_active(false)
{

}

CountdownTimer::CountdownTimer(size_t duration)
    : counter()
    , start_time(counter.GetTicks())
    , duration(duration)
    , is_active(true)
{

}

void CountdownTimer::Reset(size_t duration)
{
    this->start_time = counter.GetTicks();
    this->duration = duration;
    is_active = true;
}

size_t CountdownTimer::TimePassed()
{
    return (counter.GetTicks() - start_time);
}

double CountdownTimer::RemainingPart()
{
    if (IsActive())
    {
        return 1.0 - (TimePassed() / static_cast<double>(duration));
    }
    else return 0;
}

double CountdownTimer::GonePart()
{
    if (IsActive())
    {
        return TimePassed() / static_cast<double>(duration);
    }
    else return 1;
}

bool CountdownTimer::IsActive()
{
    if (counter.GetTicks() - start_time > duration)
    {
        is_active = false;
    }
    return is_active;
}


void CountdownTimer::Deactivate()
{
    start_time = 0;
    duration = 0;
    is_active = false;
}

void EngineTimer::StartTimer(const std::string& timerName, size_t duration)
{
    timers[timerName] = CountdownTimer(duration);
}

bool EngineTimer::IsActive(const std::string& timerName)
{
    if (timers.find(timerName) != timers.end())
    {
        return timers[timerName].IsActive();
    }
    return false;
}

double EngineTimer::RemainingPart(const std::string& timerName)
{
    if (timers.find(timerName) != timers.end())
    {
        return timers[timerName].RemainingPart();
    }
    return 0;
}

double EngineTimer::GonePart(const std::string &timerName)
{
    if (timers.find(timerName) != timers.end())
    {
        return timers[timerName].GonePart();
    }
    return 1;
}

void EngineTimer::DropTimers()
{
    timers.clear();
}
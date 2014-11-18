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

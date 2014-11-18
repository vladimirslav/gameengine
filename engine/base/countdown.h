#ifndef __COUNTDOWN_H__
#define __COUNTDOWN_H__

#include "Timer.h"
#include <string>

namespace EngineTimer
{

    void StartTimer(const std::string& timerName, size_t duration);
    bool IsActive(const std::string& timerName);
    double RemainingPart(const std::string& timerName);
    double GonePart(const std::string &timerName);


    void DropTimers();

    class CountdownTimer
    {
    private:
        Timer counter;
        size_t start_time;
        size_t duration;

        bool is_active;
    public:
        CountdownTimer();
        CountdownTimer(size_t duration);

        void Reset(size_t duration);

        size_t TimePassed();

        double RemainingPart();
        double GonePart();

        bool IsActive();
    };
}

#endif

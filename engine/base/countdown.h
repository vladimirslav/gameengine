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

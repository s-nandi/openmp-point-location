#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <chrono>
#include <string>

struct timer
{
    using Clock = std::chrono::steady_clock;
    using Time = std::chrono::time_point<Clock>;
    using Duration = std::chrono::milliseconds;

    Time start_time, end_time;

    void tick()
    {
        start_time = Clock::now();
    }

    int tock()
    {
        end_time = Clock::now();
        Duration d = std::chrono::duration_cast<Duration>(end_time - start_time);
        return d.count();
    }

    int tock(const std::string &s)
    {
        int duration = tock();
        printf("Time taken for %s : %d ms \n", s.c_str(), duration);
        return duration;
    }
};

#endif // TIMER_H

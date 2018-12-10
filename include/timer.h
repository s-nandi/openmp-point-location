#ifndef TIMER_H
#define TIMER_H

#include <stdio.h>
#include <chrono>
#include <string>

struct timer
{
    using Clock = std::chrono::high_resolution_clock;
    using Time = std::chrono::time_point<Clock>;
    using Duration = std::chrono::milliseconds;

    Time start_time, end_time;
    const std::string message;

    timer(const std::string &m = "") : start_time(Clock::now()), message(m) {}

    ~timer()
    {
	end_time = Clock::now();
        Duration d = std::chrono::duration_cast<Duration>(end_time - start_time);
        int time_taken = d.count();
	if (message.empty())
	    printf("Time taken: %d ms\n", time_taken);
	else
	    printf("Time taken for %s : %d ms\n", message.c_str(), time_taken);
    }
};

#endif // TIMER_H

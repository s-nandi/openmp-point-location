#ifndef TIMER_H
#define TIMER_H

#include <iostream>
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
	    std::cout << "Time taken: " << time_taken << " ms" << std::endl;
	else
	    std::cout << "Time taken for " << message << " : " << time_taken << " ms" << std::endl;
    }
};

#endif // TIMER_H

/**
 * @file Timer.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-11-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <Timer.h>

// Construct a new timer with a wait time in microseconds
SWTimer SWTimer_construct(uint64_t waitTime)
{
    SWTimer timer;

    timer.startCounter = 0;
    timer.waitTime_us = waitTime;

    return timer;
}

// Start a timer
void SWTimer_start(SWTimer *timer_p)
{
    timer_p->startCounter = micros();
}

// Returns number of elapsed microseconds
uint64_t SWTimer_elapsedTimeUS(SWTimer *timer_p)
{
    uint64_t elapsed_us = micros()-timer_p->startCounter;

    return elapsed_us;
}

// Returns true if the timer is expired
bool SWTimer_expired(SWTimer *timer_p)
{
    uint64_t elapsed_us = SWTimer_elapsedTimeUS(timer_p);
    return elapsed_us >= timer_p->waitTime_us;
}
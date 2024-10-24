#ifndef _I_STOPWATCH_H_
#define _I_STOPWATCH_H_

#include <ctime>

/**
 * Generic interface for an object that can be used as a stopwatch
 */
class IStopwatch {
public:
    virtual ~IStopwatch() = default;

    /**
     * Starts the stopwatch
     */
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual std::chrono::nanoseconds ElapsedTime() = 0;
};

#endif
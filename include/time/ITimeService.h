#ifndef _I_STOPWATCH_H_
#define _I_STOPWATCH_H_

#include <ctime>

/**
 * Generic interface for an object that can return current time
 */
class ITimeService {
public:
    virtual ~ITimeService() = default;
    virtual std::time_t GetCurrentTime() = 0;
};

#endif
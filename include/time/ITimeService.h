#ifndef _I_STOPWATCH_H_
#define _I_STOPWATCH_H_

#include <ctime>

class ITimeService {
public:
    ~ITimeService() = default;
    virtual std::time_t GetCurrentTime() = 0;
};

#endif
#ifndef _I_LOGGER_H_
#define _I_dLOGGER_H_

#include <chrono>
#include <vector>

typedef int MessageId;

struct RoundTripTime {
    MessageId id;
    std::time_t roundTripTime;
};

class ILogger {
public:
    virtual void LogRoundTripTimes(std::vector<RoundTripTime> recordedTimes) = 0;
};

#endif
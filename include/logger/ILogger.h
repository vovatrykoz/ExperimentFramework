#ifndef _I_LOGGER_H_
#define _I_LOGGER_H_

#include <chrono>
#include <vector>

typedef int MessageId;

struct RoundTripInfo {
    MessageId id;
    std::time_t roundTripTime;
};

class ILogger {
public:
    virtual void LogRoundTripTimes(std::vector<RoundTripInfo> recordedTimes) = 0;
};

#endif
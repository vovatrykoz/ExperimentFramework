#ifndef _I_LOGGER_H_
#define _I_LOGGER_H_

#include <chrono>
#include <vector>

typedef int MessageId;

struct RoundTripInfo {
    MessageId id;
    std::time_t roundTripTime;

    bool operator==(const RoundTripInfo& other) const {
        return id == other.id;
    }
};

class ILogger {
public:
    virtual void LogRoundTripTimes(
        const std::vector<RoundTripInfo>& recordedTimes) = 0;
};

#endif
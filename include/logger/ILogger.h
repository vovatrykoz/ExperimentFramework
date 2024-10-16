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
    virtual ~ILogger() = 0;
    virtual void LogRoundTripTimes(
        const std::list<RoundTripInfo>& recordedTimes) = 0;
};

#endif
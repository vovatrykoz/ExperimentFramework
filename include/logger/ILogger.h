#ifndef _I_LOGGER_H_
#define _I_LOGGER_H_

#include <chrono>
#include <list>

typedef int MessageId;

struct RoundTripInfo {
    MessageId id;
    std::chrono::milliseconds roundTripTime;

    bool operator==(const RoundTripInfo& other) const { return id == other.id; }
};

/**
 * Interface for a generic logger
 */
class ILogger {
public:
    virtual ~ILogger() = default;

    /**
     * Log all of the recorded round trip times
     * 
     * @param recordedTimes A list of the roundtrip times paired with message
     * ids
     */
    virtual void LogRoundTripTimes(
        const std::list<RoundTripInfo>& recordedTimes) = 0;
};

#endif
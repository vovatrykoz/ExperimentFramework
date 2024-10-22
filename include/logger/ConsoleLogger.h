#ifndef _CONSOLE_LOGGER_H_
#define _CONSOLE_LOGGER_H_

#include "ILogger.h"

/**
 * Logs roundtrip times to console
 */
class ConsoleLogger : public ILogger {
public:
    /**
     * Log roundtrip times to console
     *
     * @param recordedTimes A list of the roundtrip times paired with message
     * ids
     */
    virtual void LogRoundTripTimes(
        const std::list<RoundTripInfo>& recordedTimes) override;
};

#endif
#ifndef _CONSOLE_LOGGER_H_
#define _CONSOLE_LOGGER_H_

#include "ILogger.h"

class ConsoleLogger : public ILogger {
public:
    virtual void LogRoundTripTimes(
        const std::list<RoundTripInfo>& recordedTimes) override;
};

#endif
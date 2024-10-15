#ifndef _I_DATA_LOGGER_H_
#define _I_DATA_LOGGER_H_

#include <chrono>

typedef int MessageId;

class IRoundTripLogger {
public:
    virtual void LogRoundTripTimeForMessage(MessageId id,
                                            std::time_t roundTripTime) = 0;
};

#endif
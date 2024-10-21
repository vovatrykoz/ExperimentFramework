#ifndef _CHRONO_TIME_SERVICE_H_
#define _CHRONO_TIME_SERVICE_H_

#include "ITimeService.h"

#include <chrono>

/**
 * Uses std::chrono to return current time
 */
class ChronoTimeService : public ITimeService {
public:
    virtual std::time_t GetCurrentTime() override;
};

inline std::time_t ChronoTimeService::GetCurrentTime() {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::system_clock::to_time_t(now);
}

#endif
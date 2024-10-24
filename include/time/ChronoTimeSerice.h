#ifndef _CHRONO_TIME_SERVICE_H_
#define _CHRONO_TIME_SERVICE_H_

#include <chrono>

#include "ITimeService.h"

/**
 * Uses std::chrono to return current time
 */
class ChronoTimeService : public IStopwatch {
private:
    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point end;

public:
    /**
     * Returns current time using std::chrono
     */
    virtual void Start() override;
    virtual void Stop() override;
    virtual std::chrono::nanoseconds ElapsedTime() override;
};

inline void ChronoTimeService::Start() {
    this->start = std::chrono::high_resolution_clock::now();
}

inline void ChronoTimeService::Stop() {
    this->end = std::chrono::high_resolution_clock::now();
}

inline std::chrono::nanoseconds ChronoTimeService::ElapsedTime() {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(this->end -
                                                                this->start);
}

#endif
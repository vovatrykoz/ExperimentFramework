#include "logger/ConsoleLogger.h"

#include <iostream>

void ConsoleLogger::LogRoundTripTimes(
    const std::list<RoundTripInfo>& recordedTimes) {
    for (const auto& record : recordedTimes) {
        std::cout << "Message " << record.id << " round trip time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(
                         record.roundTripTime)
                         .count()
                  << std::endl;
    }
}
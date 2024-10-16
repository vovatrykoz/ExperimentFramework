#include "logger/ConsoleLogger.h"

#include <iostream>

void ConsoleLogger::LogRoundTripTimes(
    const std::list<RoundTripInfo>& recordedTimes) {
    for (const auto& record : recordedTimes) {
        std::cout << "Message " << record.id
                  << " round trip time: " << record.roundTripTime << std::endl;
    }
}
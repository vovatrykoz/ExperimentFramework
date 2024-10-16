#ifndef _PRIMARY_NODE_H_
#define _PRIMARY_NODE_H_

#include <list>
#include <memory>
#include <unordered_map>

#include "logger/ILogger.h"
#include "receiver/IReceiver.h"
#include "transmitter/ITransmitter.h"
#include "time/ITimeService.h"

class PrimaryNode {
private:
    std::unique_ptr<IReceiver> receiver;
    std::unique_ptr<ITransmitter> transmitter;
    std::unique_ptr<ILogger> logger;
    std::unique_ptr<ITimeService> timeService;

    std::unordered_map<ExperimentMessage, std::time_t> trackedMessages;
    std::list<RoundTripInfo> recordedTimes;
public:
    PrimaryNode(std::unique_ptr<IReceiver> receiver,
                std::unique_ptr<ITransmitter> transmitter,
                std::unique_ptr<ILogger> logger,
                std::unique_ptr<ITimeService> timeService);

    void Transmit(uint32_t numberOfMessages);
    void Receive();
    void LogResults();
};

inline void PrimaryNode::LogResults() {
    this->logger->LogRoundTripTimes(this->recordedTimes);
}

#endif
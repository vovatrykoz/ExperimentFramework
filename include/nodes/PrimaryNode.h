#ifndef _PRIMARY_NODE_H_
#define _PRIMARY_NODE_H_

#include <list>
#include <memory>

#include "logger/ILogger.h"
#include "receiver/IReceiver.h"
#include "transmitter/ITransmitter.h"

class PrimaryNode {
private:
    std::unique_ptr<IReceiver> receiver;
    std::unique_ptr<ITransmitter> transmitter;
    std::unique_ptr<ILogger> logger;

    std::list<RoundTripInfo> recordedTimes;

public:
    PrimaryNode(std::unique_ptr<IReceiver> receiver,
                std::unique_ptr<ITransmitter> transmitter,
                std::unique_ptr<ILogger> logger);

    void Transmit(uint32_t numberOfMessages);
    void Receive();
    void LogResults();
};

inline void PrimaryNode::LogResults() {
    this->logger->LogRoundTripTimes(this->recordedTimes);
}

#endif
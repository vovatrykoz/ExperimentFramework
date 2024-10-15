#ifndef _PRIMARY_NODE_H_
#define _PRIMARY_NODE_H_

#include <memory>

#include "logger/ILogger.h"
#include "receiver/IReceiver.h"
#include "transmitter/ITransmitter.h"

class PrimaryNode {
private:
    std::unique_ptr<IReceiver> receiver;
    std::unique_ptr<ITransmitter> transmitter;
    std::unique_ptr<ILogger> logger;

    std::vector<RoundTripTime> recordedTimes;

public:
    PrimaryNode(std::unique_ptr<IReceiver> receiver,
                std::unique_ptr<ITransmitter> transmitter,
                std::unique_ptr<ILogger> logger)
        : receiver(std::move(receiver)),
          transmitter(std::move(transmitter)),
          logger(std::move(logger)) {}

    void Transmit(uint32_t numberOfMessages);
    void Receive();
    void LogResults();
};

#endif
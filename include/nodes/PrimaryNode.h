#ifndef _PRIMARY_NODE_H_
#define _PRIMARY_NODE_H_

#include <list>
#include <memory>

#include "logger/ILogger.h"
#include "receiver/IReceiver.h"
#include "time/ITimeService.h"
#include "transmitter/ITransmitter.h"

/**
 * This class represents a so-called "primary node" (N1 on Björn's figure
 * in his project proposal).
 *
 * This node is responsible for sending the messages into the network, and
 * waiting for them to come back so that the roundtrip of each of the messages
 * can be recorded.
 */
class PrimaryNode {
private:
    std::unique_ptr<IReceiver> receiver;        // used to receive messages
    std::unique_ptr<ITransmitter> transmitter;  // used to send messages
    std::unique_ptr<ILogger> logger;  // used to log the experiment results
    std::unique_ptr<IStopwatch> stopwatch;  // used to create timestamps

    std::list<RoundTripInfo> recordedTimes;  // results of the experiment

public:
    PrimaryNode(std::unique_ptr<IReceiver> receiver,
                std::unique_ptr<ITransmitter> transmitter,
                std::unique_ptr<ILogger> logger,
                std::unique_ptr<IStopwatch> timeService);

    /**
     * Run the specified number of iterations
     *
     * @param numberOfMessages How many messages to transmit
     */
    void Run(uint32_t numberOfMessages);

    /**
     * Log the experiment results using the injected logger
     */
    void LogResults();
};

inline void PrimaryNode::LogResults() {
    this->logger->LogRoundTripTimes(this->recordedTimes);
}

#endif
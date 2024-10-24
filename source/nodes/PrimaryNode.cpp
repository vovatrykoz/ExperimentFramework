#include "nodes/PrimaryNode.h"

#include <stdexcept>

PrimaryNode::PrimaryNode(std::unique_ptr<IReceiver> receiver,
                         std::unique_ptr<ITransmitter> transmitter,
                         std::unique_ptr<ILogger> logger,
                         std::unique_ptr<ITimeService> timeService) {
    if (receiver == nullptr) {
        throw std::invalid_argument(
            "Invalid argument: the receiver must not be null");
    }

    if (transmitter == nullptr) {
        throw std::invalid_argument(
            "Invalid argument: the transmitter must not be null");
    }

    if (logger == nullptr) {
        throw std::invalid_argument(
            "Invalid argument: the logger must not be null");
    }

    if (timeService == nullptr) {
        throw std::invalid_argument(
            "Invalid argument: the logger must not be null");
    }

    this->receiver = std::move(receiver);
    this->transmitter = std::move(transmitter);
    this->logger = std::move(logger);
    this->timeService = std::move(timeService);
}

void PrimaryNode::Run(uint32_t numberOfMessages) {
    for (uint32_t i = 0; i < numberOfMessages; i++) {
        std::time_t messageSentTimestamp = this->timeService->GetCurrentTime();
        this->transmitter->Transmit(i);

        // Continuously attempt to receive a message until successful
        std::optional<ExperimentMessage> resultContainer;
        std::time_t messageReceivedTimestamp;

        do {
            resultContainer = this->receiver->Receive();
            messageReceivedTimestamp = this->timeService->GetCurrentTime();
        } while (!resultContainer.has_value());

        ExperimentMessage result = resultContainer.value();
        std::time_t roundTripTime = messageReceivedTimestamp - messageSentTimestamp;
        this->recordedTimes.push_back({result, roundTripTime});
    }
}
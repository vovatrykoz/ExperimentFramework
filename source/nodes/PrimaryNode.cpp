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

void PrimaryNode::Transmit(uint32_t numberOfMessages) {
    for (uint32_t i = 0; i < numberOfMessages; i++) {
        this->trackedMessages[i] = this->timeService->GetCurrentTime();
        this->transmitter->Transmit(i);
    }
}

void PrimaryNode::Receive() {
    std::optional<ExperimentMessage> resultContainer =
        this->receiver->Receive();
    std::time_t messageReceivedTimestamp = this->timeService->GetCurrentTime();

    if (!resultContainer.has_value()) {
        return;
    }

    ExperimentMessage result = resultContainer.value();
    auto trackedMessageIt = this->trackedMessages.find(result);

    if (trackedMessageIt == this->trackedMessages.end()) {
        return;
    }

    std::time_t sendTimestamp = trackedMessageIt->second;
    std::time_t roundTripTime = messageReceivedTimestamp - sendTimestamp;
    this->recordedTimes.push_back({result, roundTripTime});
}

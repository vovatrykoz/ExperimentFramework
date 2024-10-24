#include "nodes/PrimaryNode.h"

#include <stdexcept>

PrimaryNode::PrimaryNode(std::unique_ptr<IReceiver> receiver,
                         std::unique_ptr<ITransmitter> transmitter,
                         std::unique_ptr<ILogger> logger,
                         std::unique_ptr<IStopwatch> timeService) {
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
    this->stopwatch = std::move(timeService);
}

void PrimaryNode::Run(uint32_t numberOfMessages) {
    for (uint32_t i = 0; i < numberOfMessages; i++) {
        this->stopwatch->Start();
        this->transmitter->Transmit(i);

        std::optional<ExperimentMessage> resultContainer;
        std::time_t messageReceivedTimestamp;

        do {
            resultContainer = this->receiver->Receive();
        } while (!resultContainer.has_value());
        this->stopwatch->Stop();

        ExperimentMessage result = resultContainer.value();
        auto elapsedTime = this->stopwatch->ElapsedTime();
        this->recordedTimes.push_back({result, elapsedTime});
    }
}
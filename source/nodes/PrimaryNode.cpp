#include "nodes/PrimaryNode.h"

#include <stdexcept>

PrimaryNode::PrimaryNode(std::unique_ptr<IReceiver> receiver,
                         std::unique_ptr<ITransmitter> transmitter,
                         std::unique_ptr<ILogger> logger) {
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

    this->receiver = std::move(receiver);
    this->transmitter = std::move(transmitter);
    this->logger = std::move(logger);
}

void PrimaryNode::Transmit(uint32_t numberOfMessages) {
    for(uint32_t i = 0; i < numberOfMessages; i++) {
        this->transmitter->Transmit(i);
    }
}

void PrimaryNode::Receive() {
    std::optional<ExperimentMessage> resultContainer = this->receiver->Receive();

    if(!resultContainer.has_value()) {
        return;
    }

    ExperimentMessage result = resultContainer.value();
    this->recordedTimes.push_back({result, result});
}

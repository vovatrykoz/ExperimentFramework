#include "nodes/IntermediateNode.h"

#include <stdexcept>

IntermediateNode::IntermediateNode(std::unique_ptr<IReceiver> receiver,
                                   std::unique_ptr<ITransmitter> transmitter) {
    if (receiver == nullptr) {
        throw std::invalid_argument(
            "Invalid argument: the receiver must not be null");
    }

    if (transmitter == nullptr) {
        throw std::invalid_argument(
            "Invalid argument: the transmitter must not be null");
    }

    this->receiver = std::move(receiver);
    this->transmitter = std::move(transmitter);
}

void IntermediateNode::Run() {
    std::optional<ExperimentMessage> messageContainer =
        this->receiver->Receive();

    if (!messageContainer.has_value()) {
        return;
    }

    ExperimentMessage message = messageContainer.value();
    this->transmitter->Transmit(message);
}
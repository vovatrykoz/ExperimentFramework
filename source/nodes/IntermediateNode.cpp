#include "nodes/IntermediateNode.h"

#include <iostream>
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
    
    // this print to std out is just to help visualize the process
    // in real scenario, this needs to be removed to make the time measurements more accurate
    std::cout << "Message " << message
              << " received! Forwarding back to the sender" << std::endl;

    this->transmitter->Transmit(message);
}
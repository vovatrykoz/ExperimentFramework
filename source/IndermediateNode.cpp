#include "IndermediateNode.h"

void IntermediateNode::Run() {
    std::optional<ExperimentMessage> messageContainer = this->receiver->Receive();

    if(!messageContainer.has_value()) {
        return;
    }

    ExperimentMessage message = messageContainer.value();
    this->transmitter->Transmit(message);
}
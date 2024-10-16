#include "receiver/InterThreadReceiver.h"

std::optional<ExperimentMessage> InterThreadReceiver::Receive() {
    if(this->messageQueue == nullptr) {
        return std::nullopt;
    }

    return this->messageQueue->Pop();
}
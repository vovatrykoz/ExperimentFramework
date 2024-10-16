#include "transmitter/InterThreadTransmitter.h"

void InterThreadTransmitter::Transmit(ExperimentMessage message) {
    if(messageQueue == nullptr) {
        return;
    }

    this->messageQueue->Push(message);
}
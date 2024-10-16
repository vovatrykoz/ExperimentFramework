#include "transmitter/InterThreadTransmitter.h"

void InterThreadTransmitter::Transmit(ExperimentMessage message) {
    if(this->messageQueue == nullptr) {
        return;
    }

    this->messageQueue->Push(message);
}
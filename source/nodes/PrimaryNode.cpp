#include "nodes/PrimaryNode.h"

void PrimaryNode::Transmit(uint32_t numberOfMessages) {}

void PrimaryNode::Receive() {}

void PrimaryNode::LogResults() {
    this->logger->LogRoundTripTimes(this->recordedTimes);
}

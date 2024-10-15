#ifndef _PRIMARY_NODE_H_
#define _PRIMARY_NODE_H_

#include <memory>

#include "receiver/IReceiver.h"
#include "transmitter/ITransmitter.h"

class PrimaryNode {
private:
    std::unique_ptr<IReceiver> receiver;
    std::unique_ptr<ITransmitter> transmitter;

public:
    PrimaryNode(std::unique_ptr<IReceiver> receiver,
                std::unique_ptr<ITransmitter> transmitter)
        : receiver(std::move(receiver)), transmitter(std::move(transmitter)) {}

    void Transmit(int numberOfMessages);
    void Receive();
};

#endif
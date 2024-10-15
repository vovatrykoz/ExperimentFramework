#ifndef _INTERMEDIATE_NODE_H_
#define _INTERMEDIATE_NODE_H_

#include <memory>

#include "IReceiver.h"
#include "ITransmitter.h"

class IntermediateNode {
private:
    std::unique_ptr<IReceiver> receiver;
    std::unique_ptr<ITransmitter> transmitter;

public:
    IntermediateNode(std::unique_ptr<IReceiver> receiver,
                     std::unique_ptr<ITransmitter> transmitter)
        : receiver(std::move(receiver)), transmitter(std::move(transmitter)) {}

    void Run();
};

#endif
#ifndef _INTERMEDIATE_NODE_H_
#define _INTERMEDIATE_NODE_H_

#include <memory>

#include "receiver/IReceiver.h"
#include "transmitter/ITransmitter.h"

/**
 * This class represents a so-called "intermediate node" (N2 on Björn's figure
 * in his project proposal).
 *
 * The only purpose of this node is receiving a message and forwarding it back
 * to the sender (or to any other node if the roundtrip time) includes several
 * nodes
 */
class IntermediateNode {
private:
    std::unique_ptr<IReceiver> receiver;        // Used to listen for messages
    std::unique_ptr<ITransmitter> transmitter;  // Used to send messages

public:
    IntermediateNode(std::unique_ptr<IReceiver> receiver,
                     std::unique_ptr<ITransmitter> transmitter);

    /**
     * Check for messages using the receiver injected into this class
     * Once the receiver gets the message, it will pass it on to the transmitter
     * that will then forward it to the specified endpoint.
     *
     * The method only performs one iteration of the process described above.
     * For contious run, this method can be executed inside a loop, for example.
     */
    void Run();
};

#endif
#ifndef _INTER_THREAD_TRANSMITTER_H_
#define _INTER_THREAD_TRANSMITTER_H_

#include "ITransmitter.h"

#include "queue/ThreadSafeQueue.h"

class InterThreadTransmitter : public ITransmitter {
private:
    ThreadSafeQueue* messageQueue;
public:
    InterThreadTransmitter(ThreadSafeQueue* messageQueue) : messageQueue(messageQueue) {}

    virtual void Transmit(ExperimentMessage message) override;
};

#endif
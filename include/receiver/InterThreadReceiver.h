#ifndef _INTER_THREAD_RECEIVER_H_
#define _INTER_THREAD_RECEIVER_H_

#include "IReceiver.h"

#include "queue/ThreadSafeQueue.h"

class InterThreadReceiver : public IReceiver {
private:
    ThreadSafeQueue* messageQueue;
public:
    InterThreadReceiver(ThreadSafeQueue* messageQueue) : messageQueue(messageQueue) {}
    virtual std::optional<ExperimentMessage> Receive() override;
};

#endif
#ifndef _THREAD_SAFE_QUEUE_H_
#define _THREAD_SAFE_QUEUE_H_

#include <mutex>
#include <queue>

#include "ExperimentMessage.h"

class ThreadSafeQueue {
private:
    std::queue<ExperimentMessage> queue;
    mutable std::mutex queueMutex;
public:
    void Push(ExperimentMessage message);
    void Pop();
    size_t Size() const;
    bool IsEmpty() const;
};

#endif
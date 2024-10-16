#ifndef _THREAD_SAFE_QUEUE_H_
#define _THREAD_SAFE_QUEUE_H_

#include <mutex>
#include <queue>
#include <optional>

#include "ExperimentMessage.h"

class ThreadSafeQueue {
private:
    std::queue<ExperimentMessage> queue;
    mutable std::mutex queueMutex;
public:
    void Push(ExperimentMessage message);
    std::optional<ExperimentMessage> Pop();
    size_t Size() const;
    bool IsEmpty() const;
};

#endif
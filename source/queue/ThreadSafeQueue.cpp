#include "queue/ThreadSafeQueue.h"

void ThreadSafeQueue::Push(ExperimentMessage message) {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    this->queue.push(message);
}

void ThreadSafeQueue::Pop() {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    this->queue.pop();
}

size_t ThreadSafeQueue::Size() const {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    return this->queue.size();
}

bool ThreadSafeQueue::IsEmpty() const {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    return this->queue.empty();
}

#include "queue/ThreadSafeQueue.h"

void ThreadSafeQueue::Push(ExperimentMessage message) {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    this->queue.push(message);
}

void ThreadSafeQueue::Pop() {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    this->queue.pop();
}

size_t ThreadSafeQueue::Size() {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    return this->queue.size();
}

bool ThreadSafeQueue::IsEmpty() {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    return this->queue.empty();
}

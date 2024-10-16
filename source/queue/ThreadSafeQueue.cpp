#include "queue/ThreadSafeQueue.h"

void ThreadSafeQueue::Push(ExperimentMessage message) {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    this->queue.push(message);
}

std::optional<ExperimentMessage> ThreadSafeQueue::Pop() {
    std::optional<ExperimentMessage> result = std::nullopt;

    {
        std::lock_guard<std::mutex> lock(this->queueMutex);

        if (this->queue.empty()) {
            return result;
        }

        result = this->queue.front();
        this->queue.pop();
    }

    return result;
}

size_t ThreadSafeQueue::Size() const {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    return this->queue.size();
}

bool ThreadSafeQueue::IsEmpty() const {
    std::lock_guard<std::mutex> lock(this->queueMutex);
    return this->queue.empty();
}

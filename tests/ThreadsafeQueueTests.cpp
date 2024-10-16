#include <gtest/gtest.h>

#include <functional>

#include "queue/ThreadSafeQueue.h"

TEST(ThreadSafeQueueTest, ReturnsNulloptWhenEmpty) {
    ThreadSafeQueue queue;

    std::optional<ExperimentMessage> expected = std::nullopt;
    std::optional<ExperimentMessage> actual = queue.Pop();

    ASSERT_EQ(expected, actual);
}

TEST(ThreadSafeQueueTest, ReturnsSomeValueAfterPush) {
    ThreadSafeQueue queue;

    ExperimentMessage msg = 1;
    queue.Push(msg);

    std::optional<ExperimentMessage> expected = 1;
    std::optional<ExperimentMessage> actual = queue.Pop();

    ASSERT_TRUE(queue.IsEmpty());
    ASSERT_EQ(expected, actual);
}
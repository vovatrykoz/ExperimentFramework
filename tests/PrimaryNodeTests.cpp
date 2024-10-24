#include <gtest/gtest.h>

#include <functional>
#include <queue>

#include "nodes/PrimaryNode.h"

class MockTransmitterTransmitFunc : public ITransmitter {
private:
    std::function<void(ExperimentMessage)> transmit;

public:
    MockTransmitterTransmitFunc(std::function<void(ExperimentMessage)> transmit)
        : transmit(transmit) {}

    virtual void Transmit(ExperimentMessage message) override {
        this->transmit(message);
    }
};

class MockReceiverReceiveFunc : public IReceiver {
private:
    std::function<std::optional<ExperimentMessage>()> receive;

public:
    MockReceiverReceiveFunc(
        std::function<std::optional<ExperimentMessage>()> receive)
        : receive(receive) {}

    virtual std::optional<ExperimentMessage> Receive() override {
        return this->receive();
    }
};

class MockLogger : public ILogger {
private:
    std::function<void(const std::list<RoundTripInfo>&)> log;

public:
    MockLogger() : log([](const std::list<RoundTripInfo>&) {}) {}
    MockLogger(std::function<void(const std::list<RoundTripInfo>&)> log)
        : log(log) {}

    virtual void LogRoundTripTimes(
        const std::list<RoundTripInfo>& recordedTimes) override {
        this->log(recordedTimes);
    }
};

class MockTimeService : public ITimeService {
private:
    std::function<std::time_t()> time;

public:
    MockTimeService(std::function<std::time_t()> time) : time(time) {}

    virtual std::time_t GetCurrentTime() override { return this->time(); }
};

TEST(PrimaryNodeTest, ThrowsExceptionIfNoReceiverIsProvided) {
    ASSERT_THROW(
        PrimaryNode node(nullptr,
                         std::make_unique<MockTransmitterTransmitFunc>(
                             [](ExperimentMessage msg) {}),
                         std::make_unique<MockLogger>(),
                         std::make_unique<MockTimeService>([]() { return 0; })),
        std::invalid_argument);
}

TEST(PrimaryNodeTest, ThrowsExceptionIfNoTransmitterIsProvided) {
    ASSERT_THROW(
        PrimaryNode node(
            std::make_unique<MockReceiverReceiveFunc>([]() { return 0; }),
            nullptr, std::make_unique<MockLogger>(),
            std::make_unique<MockTimeService>([]() { return 0; })),
        std::invalid_argument);
}

TEST(PrimaryNodeTest, ThrowsExceptionIfNoLoggerIsProvided) {
    ASSERT_THROW(
        PrimaryNode node(
            std::make_unique<MockReceiverReceiveFunc>([]() { return 0; }),
            std::make_unique<MockTransmitterTransmitFunc>(
                [](ExperimentMessage msg) {}),
            nullptr, std::make_unique<MockTimeService>([]() { return 0; })),
        std::invalid_argument);
}

TEST(PrimaryNodeTest, ThrowsExceptionIfNoTimeServiceIsProvided) {
    ASSERT_THROW(PrimaryNode node(std::make_unique<MockReceiverReceiveFunc>(
                                      []() { return 0; }),
                                  std::make_unique<MockTransmitterTransmitFunc>(
                                      [](ExperimentMessage msg) {}),
                                  std::make_unique<MockLogger>(), nullptr),
                 std::invalid_argument);
}

TEST(PrimaryNodeTest, CanTransmitSeveralMessages) {
    std::vector<ExperimentMessage> actualTransmittedMessages;
    int numberOfMessagesToTransmit = 30;

    auto transmitFunc =
        [&actualTransmittedMessages](ExperimentMessage message) {
            actualTransmittedMessages.push_back(message);
        };

    auto mockReceiver =
        std::make_unique<MockReceiverReceiveFunc>([]() { return 0; });
    auto mockTransmitter =
        std::make_unique<MockTransmitterTransmitFunc>(transmitFunc);
    auto mockLogger = std::make_unique<MockLogger>();
    auto mockTimeService =
        std::make_unique<MockTimeService>([]() { return 0; });

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger), std::move(mockTimeService));

    node.Run(numberOfMessagesToTransmit);

    std::vector<ExperimentMessage> expectedTransmittedMessages;

    for (int i = 0; i < numberOfMessagesToTransmit; i++) {
        expectedTransmittedMessages.push_back(i);
    }

    ASSERT_EQ(expectedTransmittedMessages, actualTransmittedMessages);
}

TEST(PrimaryNodeTest, CanReceiveSeveralMessages) {
    std::vector<ExperimentMessage> actualReceivedMessages;
    int numberOfMessagesToReceive = 30;
    int currentMessage = 0;

    auto receiveFunc = [&currentMessage, &actualReceivedMessages]() {
        ExperimentMessage result = currentMessage;
        currentMessage++;

        actualReceivedMessages.push_back(result);
        return result;
    };

    auto mockReceiver = std::make_unique<MockReceiverReceiveFunc>(receiveFunc);
    auto mockTransmitter = std::make_unique<MockTransmitterTransmitFunc>(
        [](ExperimentMessage msg) {});
    auto mockLogger = std::make_unique<MockLogger>();
    auto mockTimeService =
        std::make_unique<MockTimeService>([]() { return 0; });

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger), std::move(mockTimeService));

    std::vector<ExperimentMessage> expectedReceivedMessages;
    for (int i = 0; i < numberOfMessagesToReceive; i++) {
        expectedReceivedMessages.push_back(i);
    }

    node.Run(numberOfMessagesToReceive);

    ASSERT_EQ(expectedReceivedMessages, actualReceivedMessages);
}

TEST(PrimaryNodeTest, AllReceivedMessagesAreLogged) {
    std::vector<RoundTripInfo> actualLoggedMessages;
    int numberOfMessagesToReceive = 30;
    int currentMessage = 0;

    auto receiveFunc = [&currentMessage]() { return currentMessage++; };

    auto loggingFunc =
        [&actualLoggedMessages](const std::list<RoundTripInfo>& timesToLog) {
            for (const auto& logItem : timesToLog) {
                actualLoggedMessages.push_back(logItem);
            }
        };

    auto mockReceiver = std::make_unique<MockReceiverReceiveFunc>(receiveFunc);
    auto mockTransmitter = std::make_unique<MockTransmitterTransmitFunc>(
        [](ExperimentMessage msg) {});
    auto mockLogger = std::make_unique<MockLogger>(loggingFunc);
    auto mockTimeService =
        std::make_unique<MockTimeService>([]() { return 0; });

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger), std::move(mockTimeService));

    node.Run(numberOfMessagesToReceive);

    std::vector<RoundTripInfo> expectedLoggedMessages;
    for (int i = 0; i < numberOfMessagesToReceive; i++) {
        expectedLoggedMessages.push_back(RoundTripInfo{i, 0});
    }

    node.LogResults();

    ASSERT_EQ(expectedLoggedMessages, actualLoggedMessages);
}

TEST(PrimaryNodeTest, NotingIsLoggedIfWeDoNotReceiveMessages) {
    std::vector<RoundTripInfo> actualLoggedMessages;
    int numberOfMessagesToReceive = 30;
    int currentMessage = 0;

    auto receiveFunc = [&currentMessage]() { return std::nullopt; };

    auto loggingFunc =
        [&actualLoggedMessages](const std::list<RoundTripInfo>& timesToLog) {
            for (const auto& logItem : timesToLog) {
                actualLoggedMessages.push_back(logItem);
            }
        };

    auto mockReceiver = std::make_unique<MockReceiverReceiveFunc>(receiveFunc);
    auto mockTransmitter = std::make_unique<MockTransmitterTransmitFunc>(
        [](ExperimentMessage msg) {});
    auto mockLogger = std::make_unique<MockLogger>(loggingFunc);
    auto mockTimeService =
        std::make_unique<MockTimeService>([]() { return 0; });

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger), std::move(mockTimeService));

    for (int i = 0; i < numberOfMessagesToReceive; i++) {
        node.Run(numberOfMessagesToReceive);
    }

    node.LogResults();

    ASSERT_TRUE(actualLoggedMessages.empty());
}

TEST(PrimaryNodeTest, RoundTripTimeForMessagesIsRecorded) {
    std::vector<RoundTripInfo> actualLoggedMessages;
    std::queue<ExperimentMessage> messageQueue;
    int numberOfMessagesToTransmitAndReceive = 30;
    int currentMessage = 0;
    int currentTime = 0;

    auto transmitFunc = [&messageQueue, &currentTime](ExperimentMessage msg) {
        messageQueue.push(msg);
        currentTime++;
    };

    auto receiveFunc = [&messageQueue, &currentTime]() {
        ExperimentMessage msg = messageQueue.front();
        messageQueue.pop();
        currentTime++;
        return msg;
    };

    auto loggingFunc =
        [&actualLoggedMessages](const std::list<RoundTripInfo>& timesToLog) {
            for (const auto& logItem : timesToLog) {
                actualLoggedMessages.push_back(logItem);
            }
        };

    auto timeFunc = [&currentTime]() { return currentTime; };

    auto mockReceiver = std::make_unique<MockReceiverReceiveFunc>(receiveFunc);
    auto mockTransmitter = std::make_unique<MockTransmitterTransmitFunc>(transmitFunc);
    auto mockLogger = std::make_unique<MockLogger>(loggingFunc);
    auto mockTimeService = std::make_unique<MockTimeService>(timeFunc);

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger), std::move(mockTimeService));

    std::vector<RoundTripInfo> expectedLoggedMessages;
    for (int i = 0; i < numberOfMessagesToTransmitAndReceive; i++) {
        expectedLoggedMessages.push_back(
            RoundTripInfo{i, i + numberOfMessagesToTransmitAndReceive});
    }

    node.Run(numberOfMessagesToTransmitAndReceive);

    for (int i = 0; i < numberOfMessagesToTransmitAndReceive; i++) {
        node.Run(numberOfMessagesToTransmitAndReceive);
    }

    node.LogResults();

    ASSERT_EQ(expectedLoggedMessages, actualLoggedMessages);
}
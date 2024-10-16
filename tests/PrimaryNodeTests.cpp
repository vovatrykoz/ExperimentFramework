#include <gtest/gtest.h>

#include <functional>

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
    MockLogger(std::function<void(const std::list<RoundTripInfo>&)> log) : log(log) {}

    virtual void LogRoundTripTimes(
        const std::list<RoundTripInfo>& recordedTimes) override {
        this->log(recordedTimes);
    }
};

TEST(PrimaryNodeTest, ThrowsExceptionIfNoReceiverIsProvided) {
    ASSERT_THROW(PrimaryNode node(nullptr,
                                  std::make_unique<MockTransmitterTransmitFunc>(
                                      [](ExperimentMessage msg) {}),
                                  std::make_unique<MockLogger>()),
                 std::invalid_argument);
}

TEST(PrimaryNodeTest, ThrowsExceptionIfNoTransmitterIsProvided) {
    ASSERT_THROW(PrimaryNode node(std::make_unique<MockReceiverReceiveFunc>(
                                      []() { return 0; }),
                                  nullptr, std::make_unique<MockLogger>()),
                 std::invalid_argument);
}

TEST(PrimaryNodeTest, ThrowsExceptionIfNoLoggerIsProvided) {
    ASSERT_THROW(PrimaryNode node(std::make_unique<MockReceiverReceiveFunc>(
                                      []() { return 0; }),
                                  std::make_unique<MockTransmitterTransmitFunc>(
                                      [](ExperimentMessage msg) {}),
                                  nullptr),
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

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger));

    node.Transmit(numberOfMessagesToTransmit);

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

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger));

    std::vector<ExperimentMessage> expectedReceivedMessages;
    for (int i = 0; i < numberOfMessagesToReceive; i++) {
        expectedReceivedMessages.push_back(i);
    }

    for (int i = 0; i < numberOfMessagesToReceive; i++) {
        node.Receive();
    }

    ASSERT_EQ(expectedReceivedMessages, actualReceivedMessages);
}

TEST(PrimaryNodeTest, AllReceivedMessagesAreLogged) {
    std::vector<RoundTripInfo> actualLoggedMessages;
    int numberOfMessagesToReceive = 30;
    int currentMessage = 0;

    auto receiveFunc = [&currentMessage]() {
        ExperimentMessage result = currentMessage;
        currentMessage++;
        return result;
    };

    auto loggingFunc = [&actualLoggedMessages](const std::list<RoundTripInfo>& timesToLog) {
        for(const auto& logItem : timesToLog) {
            actualLoggedMessages.push_back(logItem);
        }
    };

    auto mockReceiver = std::make_unique<MockReceiverReceiveFunc>(receiveFunc);
    auto mockTransmitter = std::make_unique<MockTransmitterTransmitFunc>(
        [](ExperimentMessage msg) {});
    auto mockLogger = std::make_unique<MockLogger>(loggingFunc);

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger));

    std::vector<RoundTripInfo> expectedLoggedMessages;
    for (int i = 0; i < numberOfMessagesToReceive; i++) {
        expectedLoggedMessages.push_back(RoundTripInfo{i, i});
    }

    for (int i = 0; i < numberOfMessagesToReceive; i++) {
        node.Receive();
    }

    node.LogResults();

    ASSERT_EQ(expectedLoggedMessages, actualLoggedMessages);
}

TEST(PrimaryNodeTest, NotingIsLoggedIfWeDoNotReceiveMessages) {
    std::vector<RoundTripInfo> actualLoggedMessages;
    int numberOfMessagesToReceive = 30;
    int currentMessage = 0;

    auto receiveFunc = [&currentMessage]() {
        return std::nullopt;
    };

    auto loggingFunc = [&actualLoggedMessages](const std::list<RoundTripInfo>& timesToLog) {
        for(const auto& logItem : timesToLog) {
            actualLoggedMessages.push_back(logItem);
        }
    };

    auto mockReceiver = std::make_unique<MockReceiverReceiveFunc>(receiveFunc);
    auto mockTransmitter = std::make_unique<MockTransmitterTransmitFunc>(
        [](ExperimentMessage msg) {});
    auto mockLogger = std::make_unique<MockLogger>(loggingFunc);

    PrimaryNode node(std::move(mockReceiver), std::move(mockTransmitter),
                     std::move(mockLogger));

    for (int i = 0; i < numberOfMessagesToReceive; i++) {
        node.Receive();
    }

    node.LogResults();

    ASSERT_TRUE(actualLoggedMessages.empty());
}
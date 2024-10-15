#include <gtest/gtest.h>

#include <functional>

#include "nodes/PrimaryNode.h"

class MockTransmitter : public ITransmitter {
private:
    std::function<void(ExperimentMessage)> transmit;

public:
    MockTransmitter(std::function<void(ExperimentMessage)> transmit)
        : transmit(transmit) {}

    virtual void Transmit(ExperimentMessage message) override {
        this->transmit(message);
    }
};

class MockReceiver : public IReceiver {
private:
    std::function<std::optional<ExperimentMessage>()> receive;

public:
    MockReceiver(std::function<std::optional<ExperimentMessage>()> receive)
        : receive(receive) {}

    virtual std::optional<ExperimentMessage> Receive() override {
        return this->receive();
    }
};

class MockLogger : public ILogger {
private:
    std::vector<RoundTripInfo> log;

public:
    virtual void LogRoundTripTimes(
        std::vector<RoundTripInfo> recordedTimes) override {
        this->log = recordedTimes;
    }

    std::vector<RoundTripInfo> GetLog() { return this->log; }
};
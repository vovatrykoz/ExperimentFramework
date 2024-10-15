#include <gtest/gtest.h>

#include <functional>

#include "nodes/IntermediateNode.h"

class MockReceiverAlwaysReceives : public IReceiver {
public:
    virtual std::optional<ExperimentMessage> Receive() override { return 1; }
};

class MockReceiverNeverReceives : public IReceiver {
public:
    virtual std::optional<ExperimentMessage> Receive() override {
        return std::nullopt;
    }
};

class MockTransmitter : public ITransmitter {
private:
    std::function<bool()> getTransmittedStatusValue;
    std::function<void()> setTransmittedStatusValueToTrue;

public:
    MockTransmitter(std::function<bool()> getTransmittedStatusValue,
                    std::function<void()> setTransmittedStatusValueToTrue)
        : getTransmittedStatusValue(getTransmittedStatusValue),
          setTransmittedStatusValueToTrue(setTransmittedStatusValueToTrue) {}

    virtual void Transmit(ExperimentMessage message) override {
        this->setTransmittedStatusValueToTrue();
    }

    bool GetTransmitStatus() { return this->getTransmittedStatusValue(); }
};

TEST(IntermediateNodeTest, IntermediateNodeTransmitsMessageOnReceive) {
    bool actualTransmitStatus = false;

    auto setTransmittedStatusValueToTrue = [&actualTransmitStatus]() {
        actualTransmitStatus = true;
    };

    auto getTransmittedStatusValue = [&actualTransmitStatus]() {
        return actualTransmitStatus;
    };

    IntermediateNode node(
        std::make_unique<MockReceiverAlwaysReceives>(),
        std::make_unique<MockTransmitter>(getTransmittedStatusValue,
                                          setTransmittedStatusValueToTrue));

    node.Run();

    bool expectedTransmissionStatus = true;

    ASSERT_EQ(expectedTransmissionStatus, actualTransmitStatus);
}

TEST(IntermediateNodeTest, IntermediateNodeTransmitsMessageIfNothingHasBeenReceivedReceive) {
    bool actualTransmitStatus = false;

    auto setTransmittedStatusValueToTrue = [&actualTransmitStatus]() {
        actualTransmitStatus = true;
    };

    auto getTransmittedStatusValue = [&actualTransmitStatus]() {
        return actualTransmitStatus;
    };

    IntermediateNode node(
        std::make_unique<MockReceiverNeverReceives>(),
        std::make_unique<MockTransmitter>(getTransmittedStatusValue,
                                          setTransmittedStatusValueToTrue));

    node.Run();

    bool expectedTransmissionStatus = false;

    ASSERT_EQ(expectedTransmissionStatus, actualTransmitStatus);
}
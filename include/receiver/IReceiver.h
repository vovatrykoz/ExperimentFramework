#ifndef _I_RECEIVER_H_
#define _I_RECEIVER_H_

#include <optional>

#include "ExperimentMessage.h"

/**
 * Interface for a class that can receive experiment messages
 */
class IReceiver {
public:
    virtual ~IReceiver() = default;

    /**
     * Used for attempting to receive messages.
     * If the receiver fails to receive any messages, it should return
     * std::nullopt.
     */
    virtual std::optional<ExperimentMessage> Receive() = 0;
};

#endif
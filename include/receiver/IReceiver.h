#ifndef _I_RECEIVER_H_
#define _I_RECEIVER_H_

#include <optional>

#include "ExperimentMessage.h"

class IReceiver {
public:
    virtual ~IReceiver() = default;
    virtual std::optional<ExperimentMessage> Receive() = 0;
};

#endif
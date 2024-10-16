#ifndef _I_TRANSMIT_H_
#define _I_TRANSMIT_H_

#include "ExperimentMessage.h"

class ITransmitter {
public:
    virtual ~ITransmitter() = default;
    virtual void Transmit(ExperimentMessage message) = 0;
};

#endif
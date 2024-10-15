#ifndef _I_TRANSMIT_H_
#define _I_TRANSMIT_H_

#include "ExperimentMessage.h"

class ITransmitter {
public:
    ~ITransmitter() = default;
    virtual void Transmit(ExperimentMessage) = 0;
};

#endif
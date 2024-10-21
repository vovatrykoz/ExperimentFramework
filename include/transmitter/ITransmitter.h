#ifndef _I_TRANSMIT_H_
#define _I_TRANSMIT_H_

#include "ExperimentMessage.h"

/**
 * An interface for an object that is capable of transmitting messages
 */
class ITransmitter {
public:
    virtual ~ITransmitter() = default;

    /**
     * Transmit provided message
     */
    virtual void Transmit(ExperimentMessage message) = 0;
};

#endif
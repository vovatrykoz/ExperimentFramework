#ifndef _INTER_THREAD_TRANSMITTER_H_
#define _INTER_THREAD_TRANSMITTER_H_

#include "ITransmitter.h"

class InterThreadTransmitter : public ITransmitter {
private:
    
public:
    virtual void Transmit(ExperimentMessage message) override;
};

#endif
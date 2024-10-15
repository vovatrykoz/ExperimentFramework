#ifndef _I_TRANSMIT_H_
#define _I_TRANSMIT_H_

class ITransmitter {
public:
    ~ITransmitter() = default;
    virtual void Transmit() = 0;
};

#endif
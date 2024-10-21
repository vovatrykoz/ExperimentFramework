#ifndef _SOCKET_TRANSMITTER_H_
#define _SOCKET_TRANSMITTER_H_

#include <exception>

#include "ITransmitter.h"

#include <arpa/inet.h>
#include <string>

class UdpTransmitter : public ITransmitter {
private:
    int socketDescriptor;
    sockaddr_in transmittionAddr;
public:
    UdpTransmitter(const std::string& ipAddr, uint16_t port);
    ~UdpTransmitter() override;
    virtual void Transmit(ExperimentMessage message) override;
};

class UdpTransmitterException : public std::exception {
private:
    const std::string message;

public:
    UdpTransmitterException(const std::string& message) : message(message) {}
    virtual const char* what() const noexcept override { return message.c_str(); }
};

#endif
#ifndef _SOCKET_TRANSMITTER_H_
#define _SOCKET_TRANSMITTER_H_

#include <exception>

#include "ITransmitter.h"

#include <arpa/inet.h>
#include <string>

class UdpTransmitterException : public std::exception {
private:
    const char* message;

public:
    UdpTransmitterException(const char* msg) : message(msg) {}
    virtual const char* what() const noexcept override { return message; }
};

class UdpTransmitter : public ITransmitter {
private:
    int socketDescriptor;
    sockaddr_in transmittionAddr;
public:
    UdpTransmitter(const std::string& ipAddr, uint16_t port);
    ~UdpTransmitter() override;
    virtual void Transmit(ExperimentMessage message) override;
};

#endif
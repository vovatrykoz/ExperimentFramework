#ifndef _SOCKET_TRANSMITTER_H_
#define _SOCKET_TRANSMITTER_H_

#include <exception>

#include "ITransmitter.h"

#include <arpa/inet.h>
#include <string>

class SocketTransmitterException : public std::exception {
private:
    const char* message;

public:
    SocketTransmitterException(const char* msg) : message(msg) {}
    virtual const char* what() const noexcept override { return message; }
};

class SocketTransmitter : public ITransmitter {
private:
    int socketDescriptor;
    sockaddr_in transmittionAddr;
public:
    SocketTransmitter(const std::string& ipAddr, uint16_t port);
    ~SocketTransmitter() override;
    virtual void Transmit(ExperimentMessage message) override;
};

#endif
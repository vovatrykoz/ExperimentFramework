#ifndef _SOCKET_RECEIVER_H_
#define _SOCKET_RECEIVER_H_

#include <arpa/inet.h>

#include <string>

#include "IReceiver.h"

class SocketReceiverException : public std::exception {
private:
    const char* message;

public:
    SocketReceiverException(const char* msg) : message(msg) {}
    virtual const char* what() const noexcept override { return message; }
};


class SocketReceiver : public IReceiver {
private:
    int socketDescriptor;
    sockaddr_in transmitterAddr;

public:
    SocketReceiver(const std::string& ipAddr, uint16_t port);
    ~SocketReceiver();
    virtual std::optional<ExperimentMessage> Receive() override;
};

#endif
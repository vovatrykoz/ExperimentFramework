#ifndef _SOCKET_RECEIVER_H_
#define _SOCKET_RECEIVER_H_

#include <arpa/inet.h>

#include <string>

#include "IReceiver.h"

class UdpReceiver : public IReceiver {
private:
    int socketDescriptor;
    sockaddr_in transmitterAddr;

public:
    UdpReceiver(const std::string& ipAddr, uint16_t port);
    ~UdpReceiver();
    virtual std::optional<ExperimentMessage> Receive() override;
};


class UdpReceiverException : public std::exception {
private:
    const std::string message;

public:
    UdpReceiverException(const std::string& message) : message(message) {}
    virtual const char* what() const noexcept override { return message.c_str(); }
};

#endif
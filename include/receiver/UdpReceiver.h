#ifndef _SOCKET_RECEIVER_H_
#define _SOCKET_RECEIVER_H_

#include <arpa/inet.h>

#include <string>

#include "IReceiver.h"

/**
 * A reciver that is able to receive UDP messages
 */
class UdpReceiver : public IReceiver {
private:
    int socketDescriptor;         // socket used to listen for messages
    sockaddr_in transmitterAddr;  // IP address of the sender

public:
    UdpReceiver(const std::string& ipAddr, uint16_t port);
    ~UdpReceiver();

    /**
     * Listens to the socket configured in the constructor.
     *
     * Returns the message in host order, or std::nullopt if nothing arrived on
     * the socket.
     */
    virtual std::optional<ExperimentMessage> Receive() override;
};

class UdpReceiverException : public std::exception {
private:
    const std::string message;

public:
    UdpReceiverException(const std::string& message) : message(message) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
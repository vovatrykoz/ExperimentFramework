

#ifndef _SOCKET_TRANSMITTER_H_
#define _SOCKET_TRANSMITTER_H_

#include <arpa/inet.h>

#include <exception>
#include <string>

#include "ITransmitter.h"

/**
 * Transmits messages into the network using the UDP protocol
 */
class UdpTransmitter : public ITransmitter {
private:
    int socketDescriptor; // socket used for sending messages
    sockaddr_in transmittionAddr; // destination address

public:
    UdpTransmitter(const std::string& ipAddr, uint16_t port);
    ~UdpTransmitter() override;

    /**
     * Transmits provided message using the UDP protocol
     * 
     * @param message A message that should be transmitted
     */
    virtual void Transmit(ExperimentMessage message) override;
};

class UdpTransmitterException : public std::exception {
private:
    const std::string message;

public:
    UdpTransmitterException(const std::string& message) : message(message) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // _SOCKET_TRANSMITTER_H_



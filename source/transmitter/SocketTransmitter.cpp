#include "transmitter/SocketTransmitter.h"

#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include <string>

#include "transmitter/SocketTransmitter.h"

SocketTransmitter::SocketTransmitter(const std::string& ipAddr, uint16_t port) {
    this->socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->socketDescriptor < 0) {
        throw SocketTransmitterException(
            "Could not create a socket transmitter");
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    this->transmittionAddr = serverAddr;
}

SocketTransmitter::~SocketTransmitter() { close(this->socketDescriptor); }

void SocketTransmitter::Transmit(ExperimentMessage message) {
    int32_t messageInNetworkOrder = htonl(message);
    sendto(this->socketDescriptor, &messageInNetworkOrder,
           sizeof(messageInNetworkOrder), 0,
           (struct sockaddr*)&(this->transmittionAddr),
           sizeof(this->transmittionAddr));
}
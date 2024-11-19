

#include "transmitter/UdpTransmitter.h"

#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>

UdpTransmitter::UdpTransmitter(const std::string& ipAddr, uint16_t port) {
    this->socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->socketDescriptor < 0) {
        throw UdpTransmitterException("Could not create a UDP socket");
    }

    int flags = fcntl(this->socketDescriptor, F_GETFL, 0);
    if (flags < 0) {
        close(this->socketDescriptor);
        throw UdpTransmitterException("Could not get flags for a UDP socket");
    }

    if (fcntl(this->socketDescriptor, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(this->socketDescriptor);
        throw UdpTransmitterException(
            "Could not create a non-blocking UDP socket");
    }

    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    this->transmittionAddr = serverAddr;
}

UdpTransmitter::~UdpTransmitter() { close(this->socketDescriptor); }

void UdpTransmitter::Transmit(ExperimentMessage message) {
    int32_t messageInNetworkOrder = htonl(message);
    int res = sendto(this->socketDescriptor, &messageInNetworkOrder,
                     sizeof(messageInNetworkOrder), 0,
                     (struct sockaddr*)&(this->transmittionAddr),
                     sizeof(this->transmittionAddr));
}


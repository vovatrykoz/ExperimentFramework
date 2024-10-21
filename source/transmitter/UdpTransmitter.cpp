#include "transmitter/UdpTransmitter.h"

#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include <string>

UdpTransmitter::UdpTransmitter(const std::string& ipAddr, uint16_t port) {
    this->socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->socketDescriptor < 0) {
        throw UdpTransmitterException(
            "Could not create a socket transmitter");
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
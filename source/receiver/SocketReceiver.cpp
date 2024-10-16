#include "receiver/SocketReceiver.h"

#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>

SocketReceiver::SocketReceiver(const std::string& ipAddr, uint16_t port) {
    this->socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (this->socketDescriptor < 0) {
        throw SocketReceiverException("Could not create a socket receiver");
    }

    int flags = fcntl(this->socketDescriptor, F_GETFL, 0);
    if (flags < 0) {
        close(this->socketDescriptor);
        throw SocketReceiverException("Could not create a socket receiver");
    }

    if (fcntl(this->socketDescriptor, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(this->socketDescriptor);
        throw SocketReceiverException("Could not create a socket receiver");
    }

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    this->transmitterAddr = serverAddr;

    int bindResult =
        bind(this->socketDescriptor, (struct sockaddr*)&(this->transmitterAddr),
             sizeof(this->transmitterAddr));

    if (bindResult < 0) {
        close(this->socketDescriptor);
        throw SocketReceiverException("Could not create a socket receiver");
    }
}

SocketReceiver::~SocketReceiver() { close(this->socketDescriptor); }

std::optional<ExperimentMessage> SocketReceiver::Receive() {
    int32_t messageInNetworkOrder;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int recvLen = recvfrom(this->socketDescriptor, &messageInNetworkOrder,
                           sizeof(messageInNetworkOrder), 0,
                           (struct sockaddr*)&clientAddr, &clientAddrLen);

    if (recvLen < 0) {
        return std::nullopt;
    }
    
    return ntohl(messageInNetworkOrder);
}

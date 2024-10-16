#include <atomic>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "nodes/IntermediateNode.h"
#include "receiver/SocketReceiver.h"
#include "transmitter/SocketTransmitter.h"

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << std::endl << "Shutting down the receiver" << std::endl;
    running = false;
}

void mainLoop(IntermediateNode& node) {
    while(running) {
        node.Run();
    }
}

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting setup..." << std::endl;

    auto receiver = std::make_unique<SocketReceiver>("127.0.0.1", 8081);
    auto transmitter = std::make_unique<SocketTransmitter>("127.0.0.1", 8080);

    IntermediateNode node(std::move(receiver), std::move(transmitter));

    std::cout << "Setup done!" << std::endl;

    while(running) {
        node.Run();
    }

    std::cout << "Starting cleanup..." << std::endl;
    std::cout << "Cleanup done!" << std::endl;

    return 0;
}
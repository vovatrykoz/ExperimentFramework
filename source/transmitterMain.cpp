#include <logger/ConsoleLogger.h>
#include <termios.h>
#include <time/ChronoTimeSerice.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "nodes/PrimaryNode.h"
#include "receiver/SocketReceiver.h"
#include "transmitter/SocketTransmitter.h"

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << std::endl << "Shutting down the transmitter" << std::endl;
    running = false;
}

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting setup..." << std::endl;

    auto receiver = std::make_unique<SocketReceiver>("127.0.0.1", 8080);
    auto transmitter = std::make_unique<SocketTransmitter>("127.0.0.1", 8080);
    auto logger = std::make_unique<ConsoleLogger>();
    auto timeService = std::make_unique<ChronoTimeService>();

    PrimaryNode node(std::move(receiver), std::move(transmitter),
                     std::move(logger), std::move(timeService));

    std::cout << "Setup done!" << std::endl;

    node.Transmit(10);
    node.LogResults();

    std::cout << "Starting cleanup..." << std::endl;
    std::cout << "Cleanup done!" << std::endl;

    return 0;
}
#include <atomic>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "logger/ConsoleLogger.h"
#include "time/ChronoTimeSerice.h"
#include "nodes/PrimaryNode.h"
#include "receiver/SocketReceiver.h"
#include "transmitter/SocketTransmitter.h"

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << std::endl << "Shutting down the transmitter" << std::endl;
    running = false;
}

void receiverLoop(PrimaryNode& node) {
    while(running) {
        node.Receive();
    }
}

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting setup..." << std::endl;

    auto receiver = std::make_unique<SocketReceiver>("127.0.0.1", 8080);
    auto transmitter = std::make_unique<SocketTransmitter>("127.0.0.1", 8081);
    auto logger = std::make_unique<ConsoleLogger>();
    auto timeService = std::make_unique<ChronoTimeService>();

    PrimaryNode node(std::move(receiver), std::move(transmitter),
                     std::move(logger), std::move(timeService));


    std::cout << "Setup done!" << std::endl;

    int numberOfMessages = -1;

    while(numberOfMessages < 0) {
        std::cout << "How many messages to send: ";
        std::cin >> numberOfMessages;

        if (std::cin.fail()) {
            std::cout << "Invalid input! The input is not an integer." << std::endl;
        } else {
            break;
        }
    }

    std::cout << std::unitbuf;
    std::cout << "Sending " << numberOfMessages << " messages" << std::endl;

    node.Transmit(numberOfMessages);
    std::thread receiverThread(receiverLoop, std::ref(node));

    std::cout << "Sent!" << std::endl;

    receiverThread.join();

    std::cout << "Experiment results: " << std::endl;
    node.LogResults();
    std::cout << std::endl;

    std::cout << "Starting cleanup..." << std::endl;
    std::cout << "Cleanup done!" << std::endl;

    return 0;
}
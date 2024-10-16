#include <termios.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "receiver/InterThreadReceiver.h"
#include "transmitter/SocketTransmitter.h"

std::atomic<bool> running(true);

void signalHandler(int signum) {
    std::cout << std::endl << "Shutting down the transmitter" << std::endl;
    running = false;
}

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting setup..." << std::endl;
    
    SocketTransmitter transmitter("127.0.0.1", 8080);

    std::cout << "Setup done!" << std::endl;

    transmitter.Transmit(1);

    std::cout << "Starting cleanup..." << std::endl;
    std::cout << "Cleanup done!" << std::endl;

    return 0;
}
#include <termios.h>

#include <atomic>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "receiver/InterThreadReceiver.h"
#include "transmitter/InterThreadTransmitter.h"

std::atomic<bool> running(true);

void setNonCanonicalMode() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);    // Get current terminal attributes
    term.c_lflag &= ~(ICANON | ECHO);  // Turn off canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &term);  // Apply settings
}

int keyPressed() {
    setNonCanonicalMode();
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(STDIN_FILENO + 1, &set, NULL, NULL, &timeout) == 1;
}

void signalHandler(int signum) {
    std::cout << std::endl << "Shutting down the transmitter" << std::endl;
    running = false;
}

void receiverMain(InterThreadReceiver& receiver) {
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::optional<ExperimentMessage> msgContainer = receiver.Receive();
        if (!msgContainer.has_value()) {
            continue;
        }

        ExperimentMessage msg = msgContainer.value();
        std::cout << "Receiver: Message received: " << msg << std::endl;
    }
}

void transmitterMain(InterThreadTransmitter& transmitter) {
    int startingMsgValue = 0;
    std::string numberOfMessagesToSendStr;


    while (running) {
        std::cout << "Transmitter: Enter how many messages to send: ";
        std::getline(std::cin, numberOfMessagesToSendStr);

        try {
            int numberOfMessagesToSend = std::stoi(numberOfMessagesToSendStr);
            std::cout << "Transmitter: Transmitting " << numberOfMessagesToSend
                      << " messages..." << std::endl;

            for (int i = 0; i < numberOfMessagesToSend; i++) {
                transmitter.Transmit(startingMsgValue++);
            }

            return;
        } catch (const std::invalid_argument& e) {
            std::cerr << "Please enter a number!" << std::endl;
        } catch (const std::out_of_range& e) {
            std::cerr << "The number is too large!" << std::endl;
        }
    }
}

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting setup..." << std::endl;
    ThreadSafeQueue mailbox;
    InterThreadTransmitter transmitter(&mailbox);
    InterThreadReceiver receiver(&mailbox);

    std::cout << "Setup done!" << std::endl;

    std::thread receiverThread(receiverMain, std::ref(receiver));
    std::thread transmitterThread(transmitterMain, std::ref(transmitter));

    receiverThread.join();
    transmitterThread.join();

    std::cout << "Starting cleanup..." << std::endl;
    std::cout << "Cleanup done!" << std::endl;

    return 0;
}
#include <atomic>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "SupportedProtocols.h"
#include "configurator/IPRimaryNodeConfigurator.h"
#include "configurator/PrimaryNodeConfiguratorUdp.h"
#include "logger/ConsoleLogger.h"
#include "nodes/PrimaryNode.h"
#include "receiver/UdpReceiver.h"
#include "time/ChronoTimeSerice.h"
#include "transmitter/UdpTransmitter.h"

std::atomic<bool> running(true);

std::unique_ptr<IPrimaryNodeConfigurator> getConfiguratorForProtocol(
    SupportedProtocols protocol);

std::optional<SupportedProtocols> stringToProtocol(
    const std::string& protocolName);

void signalHandler(int signum) {
    std::cout << std::endl << "Shutting down the transmitter" << std::endl;
    running = false;
}

void receiverLoop(PrimaryNode& node) {
    while (running) {
        node.Receive();
    }
}

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::string protocolString;

    std::cout << "Starting setup..." << std::endl;
    std::cout << "Enter which protocol you want to test: ";

    std::optional<SupportedProtocols> protocolContainer = std::nullopt;

    // ask user which protocol to use
    // loop until the user provides a protocol that we support
    while (true) {
        std::cin >> protocolString;
        protocolContainer = stringToProtocol(protocolString);

        if (protocolContainer.has_value()) {
            break;
        }

        std::cout
            << "The protocol you entered is not supported, please try again"
            << std::endl;
        std::cout << "Enter which protocol you want to test: ";
    }

    SupportedProtocols protocolUnderTest = protocolContainer.value();
    // get the configurator object for the specified protocol
    std::unique_ptr<IPrimaryNodeConfigurator> configurator =
        getConfiguratorForProtocol(protocolUnderTest);

    try {
        // configure the node to work with the protocol the user has provided
        PrimaryNode node = configurator->Configure();

        std::cout << "Setup done!" << std::endl;
        int numberOfMessages = -1;

        // how may messsages do we send?
        while (numberOfMessages < 0) {
            std::cout << "How many messages to send: ";
            std::cin >> numberOfMessages;

            if (std::cin.fail()) {
                std::cout << "Invalid input! The input is not an integer."
                          << std::endl;
            } else {
                break;
            }
        }

        // do not buffer console output
        std::cout << std::unitbuf;
        std::cout << "Sending " << numberOfMessages << " messages" << std::endl;

        // send the messages and listen for them on a separate thread
        node.Transmit(numberOfMessages);
        std::thread receiverThread(receiverLoop, std::ref(node));

        std::cout << "Sent!" << std::endl;

        // wait for the user to stop execuion
        receiverThread.join();

        // log experiment results
        std::cout << "Experiment results: " << std::endl;
        node.LogResults();
        std::cout << std::endl;

        std::cout << "Starting cleanup..." << std::endl;
        std::cout << "Cleanup done!" << std::endl;

        return 0;
    } catch (const PrimaryNodeConfigurationException& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    std::cout << "Setup done!" << std::endl;
}

std::unique_ptr<IPrimaryNodeConfigurator> getConfiguratorForProtocol(
    SupportedProtocols protocol) {
    switch (protocol) {
        case SupportedProtocols::UDP:
            return std::make_unique<PrimaryNodeConfiguratorUdp>();
            break;
        default:
            return nullptr;
            break;
    }
}

std::optional<SupportedProtocols> stringToProtocol(
    const std::string& protocolName) {
    if (protocolName == "UDP") {
        return SupportedProtocols::UDP;
    }

    return std::nullopt;
}
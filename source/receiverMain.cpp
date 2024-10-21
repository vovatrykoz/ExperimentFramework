#include <atomic>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "SupportedProtocols.h"
#include "configurator/IIntermediateNodeConfigurator.h"
#include "configurator/IntermediateNodeConfiguratorUdp.h"

std::atomic<bool> running(true);

std::unique_ptr<IIntermediateNodeConfigurator> getConfiguratorForProtocol(
    SupportedProtocols protocol);
std::optional<SupportedProtocols> stringToProtocol(
    const std::string& protocolName);

void signalHandler(int signum) {
    std::cout << std::endl << "Shutting down the receiver" << std::endl;
    running = false;
}

void mainLoop(IntermediateNode& node) {
    while (running) {
        // to simulate network latency
        std::this_thread::sleep_for(std::chrono::seconds(1));
        node.Run();
    }
}

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::string protocolString;

    std::cout << "Starting setup..." << std::endl;
    std::cout << "Enter which protocol you want to test: ";

    std::optional<SupportedProtocols> protocolContainer = std::nullopt;

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
    std::unique_ptr<IIntermediateNodeConfigurator> configurator =
        getConfiguratorForProtocol(protocolUnderTest);

    try {
        IntermediateNode node = configurator->Configure();

        std::cout << "Setup done!" << std::endl;

        std::thread receiverThread(mainLoop, std::ref(node));

        receiverThread.join();

        std::cout << "Starting cleanup..." << std::endl;
        std::cout << "Cleanup done!" << std::endl;

        return 0;
    } catch (const InermediateNodeConfigurationException& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }
}

std::unique_ptr<IIntermediateNodeConfigurator> getConfiguratorForProtocol(
    SupportedProtocols protocol) {
    switch (protocol) {
        case SupportedProtocols::UDP:
            return std::make_unique<IntermediateNodeConfiguratorUdp>();
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

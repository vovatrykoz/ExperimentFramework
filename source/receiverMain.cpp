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

SupportedProtocols getProtocolFromUser();
void signalHandler(int signum);
void mainLoop(IntermediateNode& node);

int main(void) {
    std::signal(SIGINT, signalHandler);

    std::cout << "Starting setup..." << std::endl;
    std::cout << "Enter which protocol you want to test: ";

    // find out which protocol the user wants to use
    SupportedProtocols protocolUnderTest = getProtocolFromUser();

    // get the configurator object for the specified protocol
    std::unique_ptr<IIntermediateNodeConfigurator> configurator =
        getConfiguratorForProtocol(protocolUnderTest);

    // create an empty container for the node
    // this trickery is done to avoid wrapping the code that will execute the
    // benchmarking code in a try-catch block. Doing so can create a preformance
    // penalty, even if no exceptions are thrown, thus distorting the final
    // results.
    std::optional<IntermediateNode> nodeContainer = std::nullopt;

    // with this approach, only the configuration part needs to be wrapped in a
    // try-catch block
    try {
        // configure the node to work with the protocol the user has provided
        // and place the configured node inside the container created above
        nodeContainer = configurator->Configure();
    } catch (const InermediateNodeConfigurationException& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    // move the configured node outside of the std::optional container
    IntermediateNode node = std::move(nodeContainer.value());
    std::cout << "Setup done!" << std::endl;

    // run the receiver
    std::thread receiverThread(mainLoop, std::ref(node));

    receiverThread.join();

    std::cout << "Starting cleanup..." << std::endl;
    std::cout << "Cleanup done!" << std::endl;

    return 0;
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
    std::string protocolNameLowerCase = "";

    for (char ch : protocolName) {
        protocolNameLowerCase += std::tolower(ch);
    }

    if (protocolNameLowerCase == "udp") {
        return SupportedProtocols::UDP;
    }

    return std::nullopt;
}

SupportedProtocols getProtocolFromUser() {
    std::string protocolString;
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

    return protocolContainer.value();
}

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
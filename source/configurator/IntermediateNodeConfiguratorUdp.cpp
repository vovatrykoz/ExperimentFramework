#include "configurator/IntermediateNodeConfiguratorUdp.h"

#include <iostream>

#include "receiver/UdpReceiver.h"
#include "transmitter/UdpTransmitter.h"

IntermediateNodeConfiguratorUdp::IntermediateNodeConfiguratorUdp() {
    std::string inPort, outPort;

    std::cout << "Enter the ip address to receive from: ";
    std::cin >> this->ipAddressToReceiveFrom;

    while (true) {
        std::cout
            << "Enter the port number on which you want to receive the data: ";
        std::cin >> inPort;

        try {
            this->portToReceiveOn = std::stoi(inPort);
            break;
        } catch (...) {
            std::cout << "Invalid input, please try again!" << std::endl;
        }
    }

    std::cout << "Enter the ip address to send to: ";
    std::cin >> this->ipAddressToSendTo;

    while (true) {
        std::cout << "Enter the port number through which you want to send the "
                     "data: ";
        std::cin >> outPort;

        try {
            this->portToSendFrom = std::stoi(outPort);
            break;
        } catch (...) {
            std::cout << "Invalid input, please try again!" << std::endl;
        }
    }
}

IntermediateNode IntermediateNodeConfiguratorUdp::Configure() {
    try {
        auto receiver = std::make_unique<UdpReceiver>(
            this->ipAddressToReceiveFrom, this->portToReceiveOn);
        auto transmitter = std::make_unique<UdpTransmitter>(
            this->ipAddressToSendTo, this->portToSendFrom);

        IntermediateNode node(std::move(receiver), std::move(transmitter));

        return node;
    } catch (const UdpReceiverException& e) {
        std::string message =
            "Configuration of the UDP receiver failed for the following "
            "reason: " +
            std::string(e.what());
        throw InermediateNodeConfigurationException(message);
    } catch (const UdpTransmitterException& e) {
        std::string message =
            "Configuration of the UDP transmitter failed for the following "
            "reason: " +
            std::string(e.what());
        throw InermediateNodeConfigurationException(message);
    } catch (const std::invalid_argument& e) {
        std::string message =
            "Configuration of the UDP node failed for the following "
            "reason: " +
            std::string(e.what());
        throw InermediateNodeConfigurationException(message);
    }
}
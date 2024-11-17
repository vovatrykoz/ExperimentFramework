#ifdef __linux__

#include "configurator/IntermediateNodeConfiguratorUdp.h"

#include <iostream>

#include "ip/IpAddress.h"
#include "receiver/UdpReceiver.h"
#include "transmitter/UdpTransmitter.h"

IntermediateNodeConfiguratorUdp::IntermediateNodeConfiguratorUdp() {
    this->ipAddressToReceiveFrom =
        ReadIpAddress("Enter the ip address to receive from: ");

    this->portToReceiveOn = ReadPortNumber(
        "Enter the port number on which you want to receive the data: ");

    this->ipAddressToSendTo =
        ReadIpAddress("Enter the ip address to send to: ");
        
    this->portToSendFrom = ReadPortNumber(
        "Enter the port number through which you want to send the data: ");
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

std::string IntermediateNodeConfiguratorUdp::ReadIpAddress(
    const std::string& prompt) {
    std::string ip;
    std::cout << prompt;
    std::cin >> ip;

    while (true) {
        if (std::regex_match(ip, ipRegex)) {
            break;
        }

        std::cout << "Invalid ip address!" << std::endl;
        std::cout << prompt;
        std::cin >> ip;
    }

    return ip;
}
uint32_t IntermediateNodeConfiguratorUdp::ReadPortNumber(
    const std::string& prompt) {
    int result = -1;

    while (true) {
        std::cout << prompt;
        std::cin >> result;

        if (!std::cin.fail()) {
            break;
        }

        std::cout << "Invalid input, please try again!" << std::endl;
    }

    return result;
}

#endif // __linux__

#include "configurator/PrimaryNodeConfiguratorUdp.h"

#include <iostream>
#include "logger/ConsoleLogger.h"
#include "time/ChronoTimeSerice.h"
#include "receiver/UdpReceiver.h"
#include "transmitter/UdpTransmitter.h"

PrimaryNodeConfiguratorUdp::PrimaryNodeConfiguratorUdp() {
    std::string inPort, outPort, logger, timeService;

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

    while (true) {
        std::cout << "Enter what kind of logger do you want to use: ";
        std::cin >> logger;

        std::optional<LoggerType> loggerTypeContainer = StringToLoggerType(logger);
        if(loggerTypeContainer.has_value()) {
            this->loggerType = loggerTypeContainer.value();
            break;
        }

        std::cout << "This logger type is not supported" << std::endl;
    }

    while (true) {
        std::cout << "Enter what kind of time service do you want to use: ";
        std::cin >> timeService;

        std::optional<TimeServiceType> timeServiceTypeContainer = StringToTimeServiceType(timeService);
        if(timeServiceTypeContainer.has_value()) {
            this->timeServiceType = timeServiceTypeContainer.value();
            break;
        }

        std::cout << "This time service type is not supported" << std::endl;
    }
}

PrimaryNode PrimaryNodeConfiguratorUdp::Configure() {
    try {
        auto receiver = std::make_unique<UdpReceiver>(this->ipAddressToReceiveFrom, this->portToReceiveOn);
        auto transmitter = std::make_unique<UdpTransmitter>(this->ipAddressToSendTo, this->portToSendFrom);
        auto logger = GetLoggerFromType(this->loggerType);
        auto timeService = GetTimeServiceFromType(this->timeServiceType);

        PrimaryNode node(std::move(receiver), std::move(transmitter),
                        std::move(logger), std::move(timeService));

        return node;
    } catch (const UdpReceiverException& e) {
        std::string message =
            "Configuration of the UDP receiver failed for the following "
            "reason: " +
            std::string(e.what());
        throw PrimaryNodeConfigurationException(message);
    } catch (const UdpTransmitterException& e) {
        std::string message =
            "Configuration of the UDP transmitter failed for the following "
            "reason: " +
            std::string(e.what());
        throw PrimaryNodeConfigurationException(message);
    } catch (const std::invalid_argument& e) {
        std::string message =
            "Configuration of the UDP node failed for the following "
            "reason: " +
            std::string(e.what());
        throw PrimaryNodeConfigurationException(message);
    }
}

std::optional<LoggerType> PrimaryNodeConfiguratorUdp::StringToLoggerType(
    const std::string& loggerStr) {
    if(loggerStr == "console") {
        return LoggerType::Console;
    }

    return std::nullopt;
}

std::optional<TimeServiceType> PrimaryNodeConfiguratorUdp::StringToTimeServiceType(
    const std::string& timeServiceStr) {
    if(timeServiceStr == "chrono") {
        return TimeServiceType::Chrono;
    }

    return std::nullopt;
}

std::unique_ptr<ILogger> PrimaryNodeConfiguratorUdp::GetLoggerFromType(
    LoggerType loggerType) {
    switch (loggerType) {
        case LoggerType::Console:
            return std::make_unique<ConsoleLogger>();
            break;
        default:
            return nullptr;
            break;
    }
}

std::unique_ptr<ITimeService> PrimaryNodeConfiguratorUdp::GetTimeServiceFromType(
    TimeServiceType timeServiceType) {
    switch (timeServiceType) {
        case TimeServiceType::Chrono:
            return std::make_unique<ChronoTimeService>();
            break;
        default:
            return nullptr;
            break;
    }
}
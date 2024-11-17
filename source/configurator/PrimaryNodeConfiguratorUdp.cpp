

#include "configurator/PrimaryNodeConfiguratorUdp.h"

#include <iostream>

#include "ip/IpAddress.h"
#include "logger/ConsoleLogger.h"
#include "receiver/UdpReceiver.h"
#include "time/ChronoTimeSerice.h"
#include "transmitter/UdpTransmitter.h"

PrimaryNodeConfiguratorUdp::PrimaryNodeConfiguratorUdp() {
    std::string logger, timeService;

    this->ipAddressToReceiveFrom =
        ReadIpAddress("Enter the ip address to receive from: ");

    this->portToReceiveOn = ReadPortNumber(
        "Enter the port number on which you want to receive the data: ");

    this->ipAddressToSendTo =
        ReadIpAddress("Enter the ip address to send to: ");

    this->portToSendFrom = ReadPortNumber(
        "Enter the port number through which you want to send the data: ");

    this->loggerType =
        ReadLoggerType("Enter what kind of logger do you want to use: ");

    this->timeServiceType = ReadTimeServiceType(
        "Enter what kind of time service do you want to use: ");
}

PrimaryNode PrimaryNodeConfiguratorUdp::Configure() {
    try {
        auto receiver = std::make_unique<UdpReceiver>(
            this->ipAddressToReceiveFrom, this->portToReceiveOn);
        auto transmitter = std::make_unique<UdpTransmitter>(
            this->ipAddressToSendTo, this->portToSendFrom);
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
    std::string loggerStrLowerCase = "";

    for (char ch : loggerStr) {
        loggerStrLowerCase += std::tolower(ch);
    }

    if (loggerStrLowerCase == "console") {
        return LoggerType::Console;
    }

    return std::nullopt;
}

std::optional<TimeServiceType>
PrimaryNodeConfiguratorUdp::StringToTimeServiceType(
    const std::string& timeServiceStr) {
    std::string timeServiceStrLowerCase = "";

    for (char ch : timeServiceStr) {
        timeServiceStrLowerCase += std::tolower(ch);
    }

    if (timeServiceStrLowerCase == "chrono") {
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

std::unique_ptr<IStopwatch>
PrimaryNodeConfiguratorUdp::GetTimeServiceFromType(
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

std::string PrimaryNodeConfiguratorUdp::ReadIpAddress(
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
uint32_t PrimaryNodeConfiguratorUdp::ReadPortNumber(const std::string& prompt) {
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

LoggerType PrimaryNodeConfiguratorUdp::ReadLoggerType(
    const std::string& prompt) {
    std::string logger;
    LoggerType output;

    while (true) {
        std::cout << prompt;
        std::cin >> logger;

        std::optional<LoggerType> loggerTypeContainer =
            StringToLoggerType(logger);
        if (loggerTypeContainer.has_value()) {
            output = loggerTypeContainer.value();
            break;
        }

        std::cout << "This logger type is not supported" << std::endl;
    }

    return output;
}

TimeServiceType PrimaryNodeConfiguratorUdp::ReadTimeServiceType(
    const std::string& prompt) {
    std::string timeService;
    TimeServiceType output;

    while (true) {
        std::cout << "Enter what kind of time service do you want to use: ";
        std::cin >> timeService;

        std::optional<TimeServiceType> timeServiceTypeContainer =
            StringToTimeServiceType(timeService);
        if (timeServiceTypeContainer.has_value()) {
            output = timeServiceTypeContainer.value();
            break;
        }

        std::cout << "This time service type is not supported" << std::endl;
    }

    return output;
}


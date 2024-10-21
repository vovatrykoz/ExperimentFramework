#ifndef _PRIMARY_NODE_CONFIGURATOR_UDP_H_
#define _PRIMARY_NODE_CONFIGURATOR_UDP_H_

#include <string>

#include "IPRimaryNodeConfigurator.h"

enum class LoggerType { Console };

enum class TimeServiceType { Chrono };

class PrimaryNodeConfiguratorUdp : public IPrimaryNodeConfigurator {
private:
    std::string ipAddressToReceiveFrom;
    uint32_t portToReceiveOn;
    std::string ipAddressToSendTo;
    uint32_t portToSendFrom;
    LoggerType loggerType;
    TimeServiceType timeServiceType;

    std::string ReadIpAddress(const std::string& prompt);
    uint32_t ReadPortNumber(const std::string& prompt);
    LoggerType ReadLoggerType(const std::string& prompt);
    TimeServiceType ReadTimeServiceType(const std::string& prompt);

public:
    PrimaryNodeConfiguratorUdp();
    virtual PrimaryNode Configure() override;

    // helpers
    static std::optional<LoggerType> StringToLoggerType(
        const std::string& loggerStr);

    static std::optional<TimeServiceType> StringToTimeServiceType(
        const std::string& timeServiceStr);

    static std::unique_ptr<ILogger> GetLoggerFromType(LoggerType loggerType);

    static std::unique_ptr<ITimeService> GetTimeServiceFromType(
        TimeServiceType timeServiceType);
};

#endif
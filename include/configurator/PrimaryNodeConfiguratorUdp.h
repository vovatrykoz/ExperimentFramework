#ifndef _PRIMARY_NODE_CONFIGURATOR_UDP_H_
#define _PRIMARY_NODE_CONFIGURATOR_UDP_H_

#include <string>

#include "IPRimaryNodeConfigurator.h"

// supported logger types
enum class LoggerType { Console };

// supported time service types
enum class TimeServiceType { Chrono };

/**
 * A configurator that will configure a node object that uses UDP to send and
 * receive data
 */
class PrimaryNodeConfiguratorUdp : public IPrimaryNodeConfigurator {
private:
    std::string ipAddressToReceiveFrom;  // ip address of the sender
    uint32_t portToReceiveOn;            // port to listen on
    std::string ipAddressToSendTo;  // ip address wehere we should forward the
                                    // received messages
    uint32_t portToSendFrom;        // port used for sending messages
    LoggerType loggerType;  // logger type that the resulting node should use
    TimeServiceType timeServiceType;  // time service type that the resulting
                                      // node should use

    /**
     * Reads an ip address from the console
     *
     * @param prompt A prompt that will be printed to the user's console asking
     * them for input
     */
    std::string ReadIpAddress(const std::string& prompt);

    /**
     * Reads a port number from the console
     *
     * @param prompt A prompt that will be printed to the user's console asking
     * them for input
     */
    uint32_t ReadPortNumber(const std::string& prompt);

    /**
     * Reads a logger type from the console
     *
     * @param prompt A prompt that will be printed to the user's console asking
     * them for input
     */
    LoggerType ReadLoggerType(const std::string& prompt);

    /**
     * Reads a time service type from the console
     *
     * @param prompt A prompt that will be printed to the user's console asking
     * them for input
     */
    TimeServiceType ReadTimeServiceType(const std::string& prompt);

public:
    PrimaryNodeConfiguratorUdp();

    /**
     * Configure a new primary node that will work using the UDP
     * protcol. Will throw an exception if the configuration fails
     *
     * @return A new intermediate node configured for use with the UDP protocol
     */
    virtual PrimaryNode Configure() override;

    /**
     * A helper function that will try to convert a string into a value from the
     * LoggerType enum
     *
     * @param loggerStr A string to convert
     *
     * @return std::optional either containing a logger type or std::nullopt if
     * the conversion failed
     */
    static std::optional<LoggerType> StringToLoggerType(
        const std::string& loggerStr);

    /**
     * A helper function that will try to convert a string into a value from the
     * TimeServiceType enum
     *
     * @param timeServiceStr A string to convert
     *
     * @return std::optional either containing a time service type or
     * std::nullopt if the conversion failed
     */
    static std::optional<TimeServiceType> StringToTimeServiceType(
        const std::string& timeServiceStr);

    /**
     * A helper function that will match a logger type to a concrete
     * implementation
     *
     * @param loggerType The implementation type that the user wants to get back
     *
     * @return A unique pointer to the logger implementation, if such
     * implementation was found. If no matching implementation was found, a
     * nullptr will be returned instead
     */
    static std::unique_ptr<ILogger> GetLoggerFromType(LoggerType loggerType);

    /**
     * A helper function that will match a time service type to a concrete
     * implementation
     *
     * @param timeServiceType The implementation type that the user wants to get
     * back
     *
     * @return A unique pointer to the time service implementation, if such
     * implementation was found. If no matching implementation was found, a
     * nullptr will be returned instead
     */
    static std::unique_ptr<ITimeService> GetTimeServiceFromType(
        TimeServiceType timeServiceType);
};

#endif
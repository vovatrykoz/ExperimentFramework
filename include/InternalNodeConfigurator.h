#ifndef _INTERMEDIATE_NODE_CONFIGURATOR_H_
#define _INTERMEDIATE_NODE_CONFIGURATOR_H_

#include <string>

#include "nodes/IntermediateNode.h"

class IntermediateNodeConfigurator {
public:
    static IntermediateNode ConfigureUDP(
        const std::string& ipAddressToReceiveFrom, uint32_t portToReceiveOn,
        const std::string& ipAddressToSendTo, uint32_t portToSendFrom);
};

class ConfigurationException : public std::exception {
private:
    const std::string message;

public:
    ConfigurationException(const std::string& message) : message(message) {}
    virtual const char* what() const noexcept override { return message.c_str(); }
};

#endif
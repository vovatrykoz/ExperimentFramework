#ifndef _I_PRIMARY_NODE_CONFIGURATOR_H_
#define _I_PRIMARY_NODE_CONFIGURATOR_H_

#include "nodes/PrimaryNode.h"

class IPrimaryNodeConfigurator {
public:
    virtual PrimaryNode Configure() = 0;
    virtual ~IPrimaryNodeConfigurator() = default;
};

class PrimaryNodeConfigurationException : public std::exception {
private:
    const std::string message;

public:
    PrimaryNodeConfigurationException(const std::string& message)
        : message(message) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
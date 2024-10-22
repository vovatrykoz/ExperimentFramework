#ifndef _I_PRIMARY_NODE_CONFIGURATOR_H_
#define _I_PRIMARY_NODE_CONFIGURATOR_H_

#include "nodes/PrimaryNode.h"

/**
 * An interface for a class that can configure primary nodes
 */
class IPrimaryNodeConfigurator {
public:
    /**
     * Performs all neccesarry configuration steps to create a functioning node
     * object that is ready to use.
     */
    virtual PrimaryNode Configure() = 0;
    virtual ~IPrimaryNodeConfigurator() = default;
};

/**
 * An exception that can be thrown if the configuration fails
 */
class PrimaryNodeConfigurationException : public std::exception {
private:
    const std::string message;

public:
    PrimaryNodeConfigurationException(const std::string& message)
        : message(message) {}

    // a message that describes the cause of the exception
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
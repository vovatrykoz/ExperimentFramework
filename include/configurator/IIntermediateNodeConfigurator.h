#ifndef _I_INTERMEDIATE_NODE_CONFIGURATOR_H_
#define _I_INTERMEDIATE_NODE_CONFIGURATOR_H_

#include <string>

#include "nodes/IntermediateNode.h"

/**
 * An interface for a class that can configure intermediate nodes
 */
class IIntermediateNodeConfigurator {
public:
    virtual ~IIntermediateNodeConfigurator() = default;

    /**
     * Performs all neccesarry configuration steps to create a functioning node
     * object that is ready to use.
     */
    virtual IntermediateNode Configure() = 0;
};

/**
 * An exception that can be thrown if the configuration fails
 */
class InermediateNodeConfigurationException : public std::exception {
private:
    const std::string message;

public:
    InermediateNodeConfigurationException(const std::string& message)
        : message(message) {}

    // a message that describes the cause of the exception
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
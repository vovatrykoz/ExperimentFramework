#ifndef _I_INTERMEDIATE_NODE_CONFIGURATOR_H_
#define _I_INTERMEDIATE_NODE_CONFIGURATOR_H_

#include <string>
#include "nodes/IntermediateNode.h"

class IIntermediateNodeConfigurator {
public:
    virtual ~IIntermediateNodeConfigurator() = default;
    virtual IntermediateNode Configure() = 0;
};

class InermediateNodeConfigurationException : public std::exception {
private:
    const std::string message;

public:
    InermediateNodeConfigurationException(const std::string& message) : message(message) {}
    virtual const char* what() const noexcept override { return message.c_str(); }
};

#endif
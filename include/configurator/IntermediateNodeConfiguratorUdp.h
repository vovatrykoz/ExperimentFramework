#ifndef _INTERMEDIATE_NODE_CONFIGURATOR_UDP_H_
#define _INTERMEDIATE_NODE_CONFIGURATOR_UDP_H_

#include "IIntermediateNodeConfigurator.h"
#include "nodes/IntermediateNode.h"

class IntermediateNodeConfiguratorUdp : public IIntermediateNodeConfigurator {
private:
    std::string ipAddressToReceiveFrom;
    uint32_t portToReceiveOn;
    std::string ipAddressToSendTo;
    uint32_t portToSendFrom;

public:
    IntermediateNodeConfiguratorUdp();
    virtual IntermediateNode Configure() override;
};

#endif
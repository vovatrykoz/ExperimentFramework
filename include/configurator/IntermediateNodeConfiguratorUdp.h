#ifndef _INTERMEDIATE_NODE_CONFIGURATOR_UDP_H_
#define _INTERMEDIATE_NODE_CONFIGURATOR_UDP_H_

#include "IIntermediateNodeConfigurator.h"
#include "nodes/IntermediateNode.h"

/**
 * A configurator that will configure a node object that uses UDP to send and
 * receive data
 */
class IntermediateNodeConfiguratorUdp : public IIntermediateNodeConfigurator {
private:
    std::string ipAddressToReceiveFrom;  // ip address of the sender
    uint32_t portToReceiveOn;            // port to listen on
    std::string ipAddressToSendTo;  // ip address wehere we should forward the
                                    // received messages
    uint32_t portToSendFrom;        // port used for sending messages

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

public:
    IntermediateNodeConfiguratorUdp();

    /**
     * Configure a new intermediate node that will work using the UDP
     * protcol. Will throw an exception if the configuration fails
     *
     * @return A new intermediate node configured for use with the UDP protocol
     */
    virtual IntermediateNode Configure() override;
};

#endif
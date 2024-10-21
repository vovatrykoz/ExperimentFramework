#include "InternalNodeConfigurator.h"

#include "receiver/UdpReceiver.h"
#include "transmitter/UdpTransmitter.h"

IntermediateNode IntermediateNodeConfigurator::ConfigureUDP(
    const std::string& ipAddressToReceiveFrom, uint32_t portToReceiveOn,
    const std::string& ipAddressToSendTo, uint32_t portToSendFrom) {
    try {
        auto receiver = std::make_unique<UdpReceiver>(ipAddressToReceiveFrom,
                                                      portToReceiveOn);
        auto transmitter =
            std::make_unique<UdpTransmitter>(ipAddressToSendTo, portToSendFrom);

        IntermediateNode node(std::move(receiver), std::move(transmitter));

        return node;
    } catch (const UdpReceiverException& e) {
        std::string message =
            "Configuration of the UDP receiver failed for the following "
            "reason: " +
            std::string(e.what());
        throw ConfigurationException(message);
    } catch (const UdpTransmitterException& e) {
        std::string message =
            "Configuration of the UDP transmitter failed for the following "
            "reason: " +
            std::string(e.what());
        throw ConfigurationException(message);
    }
}
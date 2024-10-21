#include "ip/IpAddress.h"
#include <sstream>

IpAddress::IpAddress(std::byte firstOctet, std::byte secondOctet,
                     std::byte thirdOctet, std::byte fourthOctet)
    : firstOctet(firstOctet),
      secondOctet(secondOctet),
      thirdOctet(thirdOctet),
      fourthOctet(fourthOctet) {}

std::string IpAddress::ToString() { 
    std::stringstream ss;

    int o1 = std::to_integer<int>(this->firstOctet);
    int o2 = std::to_integer<int>(this->secondOctet);
    int o3 = std::to_integer<int>(this->thirdOctet);
    int o4 = std::to_integer<int>(this->fourthOctet);

    ss << o1 << "." << o2 << "." << o3 << "." << o4;
    return ss.str();
}
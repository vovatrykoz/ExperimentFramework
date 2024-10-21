#ifndef _IP_ADDRESS_H_
#define _IP_ADDRESS_H_

#include <cstddef>
#include <string>

#include <regex>

class IpAddress {
private:
    std::byte firstOctet;
    std::byte secondOctet;
    std::byte thirdOctet;
    std::byte fourthOctet;

public:
    IpAddress(std::byte firstOctet, std::byte secondOctet, std::byte thirdOctet,
              std::byte fourthOctet);
    std::string ToString();
};

static const std::regex ipRegex(R"#(^((25[0-5]|(2[0-4]|1\d|[1-9]|)\d)\.?\b){4}$)#");

#endif
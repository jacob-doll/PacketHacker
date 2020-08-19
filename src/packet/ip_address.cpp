#include "ip_address.h"

#ifdef _WIN32
#include <WS2tcpip.h>
#endif

namespace PacketHacker {
namespace Utils {
  std::string IPv4ToString(const uint32_t ipv4Address)
  {
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipv4Address, str, INET_ADDRSTRLEN);
    return str;
  }

  uint32_t StringToIPv4(const std::string &ipv4Address)
  {
    int result;
    uint32_t IPv4Identifier = 0;
    struct in_addr addr;
    result = inet_pton(AF_INET, ipv4Address.c_str(), &(addr));
    if (result != 0) {
      IPv4Identifier = ntohl(*((uint32_t *)&(addr)));
    }
    return IPv4Identifier;
  }
}// namespace Utils

IPv4Address::IPv4Address(const uint32_t address)
  : m_data(address)
{
}

IPv4Address::IPv4Address(const std::string &address)
{
  m_data = Utils::StringToIPv4(address);
}

bool IPv4Address::IsIpv4AddressValid(const std::string &hwAddress)
{
  uint32_t b1, b2, b3, b4 = 0;
  int rc = sscanf(hwAddress.c_str(), "%u.%u.%u.%u", &b1, &b2, &b3, &b4);
  if (rc != 4 || b1 > 255 || b2 > 255 || b3 > 255 || b4 > 255) {
    return false;
  }
  return true;
}

}// namespace PacketHacker
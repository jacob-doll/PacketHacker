#include "ip_address.h"

#ifdef _WIN32
#include <WS2tcpip.h>
#endif

#include <sstream>

namespace PacketHacker {
namespace Utils {
  uint32_t StringToIPv4(const std::string &ipv4Address)
  {
    int result;
    uint32_t IPv4Identifier = 0;
    struct in_addr addr;
    result = inet_pton(AF_INET, ipv4Address.c_str(), &(addr));
    if (result != 0) {
      // IPv4Identifier = ntohl(*((uint32_t *)&(addr)));
      IPv4Identifier = addr.S_un.S_addr;
    }
    return IPv4Identifier;
  }
}// namespace Utils

IPv4Address::IPv4Address()
  : m_data(0)
{
}

IPv4Address::IPv4Address(const uint8_t b1,
  const uint8_t b2,
  const uint8_t b3,
  const uint8_t b4)
  : m_b1(b1), m_b2(b2), m_b3(b3), m_b4(b4)
{
}

IPv4Address::IPv4Address(const std::array<uint8_t, 4> &data)
  : m_b1(data.at(0)), m_b2(data.at(1)), m_b3(data.at(2)), m_b4(data.at(3))
{
}

IPv4Address::IPv4Address(const uint32_t address)
  : m_data(address)
{
}

IPv4Address::IPv4Address(const std::string &address)
{
  m_data = Utils::StringToIPv4(address);
}

std::string IPv4Address::ToString() const
{
  std::ostringstream string_stream;
  string_stream << (*this);
  return string_stream.str();
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

IPv4Address IPv4Address::operator&(const IPv4Address &mask) const
{
  return IPv4Address(m_data & mask.m_data);
}

IPv4Address IPv4Address::operator|(const IPv4Address &mask) const
{
  return IPv4Address(m_data | mask.m_data);
}

std::ostream &operator<<(std::ostream &output, const IPv4Address &ipAddress)
{
  output << (int)ipAddress.m_b1 << ".";
  output << (int)ipAddress.m_b2 << ".";
  output << (int)ipAddress.m_b3 << ".";
  output << (int)ipAddress.m_b4;
  return output;
}

}// namespace PacketHacker
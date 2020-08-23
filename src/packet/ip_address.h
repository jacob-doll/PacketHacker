#pragma once

#include <string>

namespace PacketHacker {
namespace Utils {
  std::string IPv4ToString(const uint32_t ipv4Address);
  uint32_t StringToIPv4(const std::string &ipv4Address);
}// namespace Utils

class IPv4Address
{
public:
  IPv4Address();
  explicit IPv4Address(const uint32_t address);
  explicit IPv4Address(const std::string &address);

  uint32_t GetData() { return m_data; }
  std::string ToString() const { return Utils::IPv4ToString(m_data); }

  static bool IsIpv4AddressValid(const std::string &hwAddress);

private:
  uint32_t m_data;
};

}// namespace PacketHacker
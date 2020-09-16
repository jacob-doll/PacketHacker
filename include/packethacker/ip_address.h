#pragma once

#include <string>
#include <array>

namespace PacketHacker {
namespace Utils {
  uint32_t stringToIPv4(const std::string &ipv4Address);
}// namespace Utils

class IPv4Address
{
public:
  IPv4Address();
  IPv4Address(const uint8_t b1, const uint8_t b2, const uint8_t b3, const uint8_t b4);
  IPv4Address(const std::array<uint8_t, 4> &data);
  IPv4Address(const std::string &address);
  explicit IPv4Address(const uint32_t address);

  const uint32_t data() const { return m_data; }
  std::string toString() const;

  static bool isIpv4AddressValid(const std::string &ipAddress);

  bool operator<(const IPv4Address &rhs) const
  {
    return m_data < rhs.m_data;
  }

  bool operator<=(const IPv4Address &rhs) const
  {
    return !operator>(rhs);
  }

  bool operator>(const IPv4Address &rhs) const
  {
    return m_data > rhs.m_data;
  }

  bool operator>=(const IPv4Address &rhs) const
  {
    return !operator<(rhs);
  }

  bool operator==(const IPv4Address &rhs) const
  {
    return m_data == rhs.m_data;
  }

  bool operator!=(const IPv4Address &rhs) const
  {
    return !((*this) == rhs);
  }

  IPv4Address operator&(const IPv4Address &mask) const;
  IPv4Address operator|(const IPv4Address &mask) const;

  friend std::ostream &operator<<(std::ostream &output, const IPv4Address &ipAddress);

private:
  union {
    struct
    {
      uint8_t m_b1;
      uint8_t m_b2;
      uint8_t m_b3;
      uint8_t m_b4;
    };
    uint32_t m_data;
  };
};


}// namespace PacketHacker
#pragma once

#include <string>
#include <array>

#define PHYSICAL_ADDR_LEN 6

namespace PacketHacker {
namespace Utils {
  void stringToHardwareAddress(const std::string &hwAddress, uint8_t *buf, size_t bufSize);
}// namespace Utils

class HardwareAddress
{
public:
  HardwareAddress();
  HardwareAddress(const uint8_t b1,
    const uint8_t b2,
    const uint8_t b3,
    const uint8_t b4,
    const uint8_t b5,
    const uint8_t b6);
  HardwareAddress(const std::array<uint8_t, PHYSICAL_ADDR_LEN> &data);
  HardwareAddress(const std::string &address);
  explicit HardwareAddress(const uint8_t *data);

  const uint8_t *ptr() const { return m_data.data(); }
  const std::array<uint8_t, PHYSICAL_ADDR_LEN> &data() const { return m_data; }
  std::string toString() const;

  static bool isHardwareAddressValid(const std::string &hwAddress);

  bool operator==(const HardwareAddress &rhs) const
  {
    return m_data == rhs.m_data;
  }

  bool operator!=(const HardwareAddress &rhs) const
  {
    return !((*this) == rhs);
  }

  friend std::ostream &operator<<(std::ostream &output, const HardwareAddress &hwAddress);

private:
  std::array<uint8_t, PHYSICAL_ADDR_LEN> m_data;
};


}// namespace PacketHacker
#include "hardware_address.h"

#include <iostream>
#include <sstream>
#include "packet/utils/buffer_utils.h"

namespace PacketHacker {
namespace Utils {
  std::string HardwareAddressToString(const uint8_t *hwAddress, size_t size)
  {
    constexpr uint32_t bufferSize = 6 * 3;
    char buffer[bufferSize];
    snprintf(buffer, bufferSize, "%02x:%02x:%02x:%02x:%02x:%02x", hwAddress[0], hwAddress[1], hwAddress[2], hwAddress[3], hwAddress[4], hwAddress[5]);
    return std::string(buffer);
  }
  void StringToHardwareAddress(const std::string &hwAddress, uint8_t *buf, size_t bufSize)
  {
    unsigned char a[6];
    int last = -1;
    int rc = sscanf(hwAddress.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n", a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, &last);
    if (rc != 6 || hwAddress.size() != last) {
      std::memcpy(buf, 0, bufSize);
    } else {
      std::memcpy(buf, a, bufSize);
    }
  }
}// namespace Utils

HardwareAddress::HardwareAddress()
{
  m_data.fill(0);
}

HardwareAddress::HardwareAddress(const uint8_t b1,
  const uint8_t b2,
  const uint8_t b3,
  const uint8_t b4,
  const uint8_t b5,
  const uint8_t b6)
{
  m_data.at(0) = b1;
  m_data.at(1) = b2;
  m_data.at(2) = b3;
  m_data.at(3) = b4;
  m_data.at(4) = b5;
  m_data.at(5) = b6;
}

HardwareAddress::HardwareAddress(const std::array<uint8_t, PHYSICAL_ADDR_LEN> &data)
  : m_data(data)
{
}

HardwareAddress::HardwareAddress(const std::string &address)
{
  Utils::StringToHardwareAddress(address, m_data.data(), PHYSICAL_ADDR_LEN);
}

HardwareAddress::HardwareAddress(const uint8_t *data)
{
  if (data) {
    Utils::Write(m_data.data(), data, PHYSICAL_ADDR_LEN);
  } else {
    m_data.fill(0);
  }
}

std::string HardwareAddress::ToString() const
{
  std::ostringstream string_stream;
  string_stream << (*this);
  return string_stream.str();
}

bool HardwareAddress::IsHardwareAddressValid(const std::string &hwAddress)
{
  uint8_t a[6];
  int last = -1;
  int rc = sscanf(hwAddress.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n", a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, &last);
  if (rc != 6 || hwAddress.size() != last)
    return false;
  return true;
}

std::ostream &operator<<(std::ostream &output, const HardwareAddress &hwAddress)
{
  constexpr uint32_t bufferSize = 6 * 3;
  char buffer[bufferSize];
  snprintf(buffer,
    bufferSize,
    "%02x:%02x:%02x:%02x:%02x:%02x",
    hwAddress.m_data.at(0),
    hwAddress.m_data.at(1),
    hwAddress.m_data.at(2),
    hwAddress.m_data.at(3),
    hwAddress.m_data.at(4),
    hwAddress.m_data.at(5));
  output << buffer;
  return output;
}

}// namespace PacketHacker
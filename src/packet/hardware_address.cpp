#include "hardware_address.h"

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
  : HardwareAddress(0)
{
}

HardwareAddress::HardwareAddress(const uint8_t *address)
{
  if (address) {
    std::memcpy(m_data, address, PHYSICAL_ADDR_LEN);
  } else {
    std::memset(m_data, 0, PHYSICAL_ADDR_LEN);
  }
}

HardwareAddress::HardwareAddress(const std::string &address)
{
  Utils::StringToHardwareAddress(address, m_data, PHYSICAL_ADDR_LEN);
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

const HardwareAddress HardwareAddress::s_hardwareAddress = HardwareAddress("ff:ff:ff:ff:ff:ff");

}// namespace PacketHacker
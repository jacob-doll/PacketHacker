#pragma once

#include <string>

#define PHYSICAL_ADDR_LEN 6

namespace PacketHacker {
namespace Utils {
  std::string HardwareAddressToString(const uint8_t *hwAddress, size_t size);
  void StringToHardwareAddress(const std::string &hwAddress, uint8_t *buf, size_t bufSize);
}// namespace Utils

class HardwareAddress
{
public:
  HardwareAddress();
  explicit HardwareAddress(const uint8_t *address);
  explicit HardwareAddress(const std::string &address);

  uint8_t *GetData() { return m_data; }
  std::string ToString() const { return Utils::HardwareAddressToString(m_data, PHYSICAL_ADDR_LEN); }

  static bool IsHardwareAddressValid(const std::string &hwAddress);
  const static HardwareAddress &GetBroadCastAddress() { return s_hardwareAddress; }

private:
  uint8_t m_data[PHYSICAL_ADDR_LEN];

private:
  const static HardwareAddress s_hardwareAddress;
};


}// namespace PacketHacker
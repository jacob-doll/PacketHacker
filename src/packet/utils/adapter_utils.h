#pragma once

#include <string>

namespace PacketHacker {
namespace Utils {

  uint64_t HardwareToLong(const char *hwAddress);
  std::string HardwareAddressToString(const uint8_t *hwAddress);
  uint32_t IPv4ToLong(const char *ipAddress);
  std::string IPv4ToString(const uint8_t *ipv4Address);
  std::string IPv4ToString(const uint32_t ipv4Address);

  bool IsHardwareAddressValid(const char *address);
  bool IsIpv4AddressValid(const char *address);

  uint16_t CalcChecksum(void *vdata, uint32_t size);

}// namespace Utils
}// namespace PacketHacker
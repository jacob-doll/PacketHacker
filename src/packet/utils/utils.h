#pragma once

#include <string>
#include <vector>

#define PHYSICAL_ADDR_LEN 6

#if defined(_MSC_VER)
#define BYTE_SWAP_16(data) _byteswap_ushort(data)
#define BYTE_SWAP_32(data) _byteswap_ulong(data)
#define BYTE_SWAP_64(data) _byteswap_uint64(data)
#else
#define BYTE_SWAP_16(data) __builtin_bswap16(data)
#define BYTE_SWAP_32(data) __builtin_bswap32(data)
#define BYTE_SWAP_64(data) __builtin_bswap64(data)
#endif

namespace PacketHacker {

struct AdapterInfo
{
  uint32_t index;
  std::string name;
  std::string unicastAddress;
  std::string anycastAddress;
  std::string multicastAddress;
  std::string dnsServerAddress;
  std::wstring dnsSuffix;
  std::wstring description;
  std::wstring friendlyName;
  uint8_t address[PHYSICAL_ADDR_LEN];
  std::string gatewayAddress;
};

namespace Utils {

  std::vector<AdapterInfo> GetAdapters();

  uint64_t HardwareToLong(const char *hwAddress);
  std::string HardwareAddressToString(const uint8_t *hwAddress);
  uint32_t IPv4ToLong(const char *ipAddress);
  std::string IPv4ToString(const uint8_t *ipv4Address);
  std::string IPv4ToString(uint32_t ipv4Address);

  template<size_t N>
  bool BufferEquals(const uint8_t *buffer1, const uint8_t *buffer2)
  {
    for (size_t i = 0; i < N; i++) {
      if (buffer1[i] != buffer2[i]) {
        return false;
      }
    }
    return true;
  }

  inline void Write(uint8_t *buffer, const uint8_t *ptr, size_t size)
  {
    std::memcpy(buffer, ptr, size);
  }

  template<typename T>
  void ReadValue(const uint8_t *buffer, T &value)
  {
    std::memcpy(&value, buffer, sizeof(value));
  }

  template<typename T>
  void WriteValue(uint8_t *buffer, const T &value)
  {
    std::memcpy(buffer, &value, sizeof(value));
  }

}// namespace Utils
}// namespace PacketHacker
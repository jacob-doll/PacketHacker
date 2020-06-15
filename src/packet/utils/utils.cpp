#include "utils.h"

#include <cstdio>

#ifdef _WIN32
#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>
#endif


namespace PacketHacker {
namespace Utils {
#ifdef _WIN32
  std::vector<AdapterInfo> GetAdapters()
  {
    std::vector<AdapterInfo> output;
    ULONG size = 0;
    PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
    PIP_ADAPTER_ADDRESSES pCurrAddresses = nullptr;

    ::GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &size);
    pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(size);
    if (::GetAdaptersAddresses(AF_INET, 0, nullptr, pAddresses, &size) == NO_ERROR) {
      pCurrAddresses = pAddresses;
      while (pCurrAddresses) {
        AdapterInfo info{};
        info.index = pCurrAddresses->IfIndex;
        info.name = "\\Device\\NPF_" + std::string(pCurrAddresses->AdapterName);
        if (pCurrAddresses->FirstUnicastAddress) {
          struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstUnicastAddress->Address.lpSockaddr;
          info.unicastAddress = IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
        }
        if (pCurrAddresses->FirstAnycastAddress) {
          struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstAnycastAddress->Address.lpSockaddr;
          info.anycastAddress = IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
        }
        if (pCurrAddresses->FirstMulticastAddress) {
          struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstMulticastAddress->Address.lpSockaddr;
          info.multicastAddress = IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
        }
        if (pCurrAddresses->FirstDnsServerAddress) {
          struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstDnsServerAddress->Address.lpSockaddr;
          info.dnsServerAddress = IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
        }
        if (pCurrAddresses->FirstGatewayAddress) {
          struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstGatewayAddress->Address.lpSockaddr;
          info.gatewayAddress = IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
        }
        info.dnsSuffix = pCurrAddresses->DnsSuffix;
        info.description = pCurrAddresses->Description;
        info.friendlyName = pCurrAddresses->FriendlyName;
        memcpy(info.address, pCurrAddresses->PhysicalAddress, pCurrAddresses->PhysicalAddressLength);
        output.emplace_back(info);
        pCurrAddresses = pCurrAddresses->Next;
      }
    }

    if (pAddresses) {
      free(pAddresses);
    }
    return output;
  }
#endif

  uint32_t IPv4ToLong(const char *ipAddress)
  {
    int result;
    uint32_t IPv4Identifier = 0;
    struct in_addr addr;
    result = inet_pton(AF_INET, ipAddress, &(addr));
    if (result != 0) {
      IPv4Identifier = ntohl(*((uint32_t *)&(addr)));
    }
    return IPv4Identifier;
  }

  std::string IPv4ToString(const uint8_t *ipv4Address)
  {
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, ipv4Address, str, INET_ADDRSTRLEN);
    return str;
  }

  std::string IPv4ToString(uint32_t ipv4Address)
  {
    char str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ipv4Address, str, INET_ADDRSTRLEN);
    return str;
  }

  uint64_t HardwareToLong(const char *hwAddress)
  {
    unsigned char a[6];
    int last = -1;
    int rc = sscanf(hwAddress, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n", a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, &last);
    if (rc != 6 || strlen(hwAddress) != last)
      return 0;

    return uint64_t(a[0]) << 40 | uint64_t(a[1]) << 32 | (uint32_t(a[2]) << 24 | uint32_t(a[3]) << 16 | uint32_t(a[4]) << 8 | uint32_t(a[5]));
  }

  std::string HardwareAddressToString(const uint8_t *hwAddress)
  {
    // 6 bytes * 2 chars per byte + 5 colons
    constexpr uint32_t bufferSize = 6 * 3;
    char buffer[bufferSize];
    snprintf(buffer, bufferSize, "%02x:%02x:%02x:%02x:%02x:%02x", hwAddress[0], hwAddress[1], hwAddress[2], hwAddress[3], hwAddress[4], hwAddress[5]);
    return std::string(buffer);
  }

  bool IsHardwareAddressValid(const char *address)
  {
    uint8_t a[6];
    int last = -1;
    int rc = sscanf(address, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n", a + 0, a + 1, a + 2, a + 3, a + 4, a + 5, &last);
    if (rc != 6 || strlen(address) != last)
      return false;
    return true;
  }

  bool IsIpv4AddressValid(const char *address)
  {
    uint32_t b1, b2, b3, b4 = 0;
    int rc = sscanf(address, "%u.%u.%u.%u", &b1, &b2, &b3, &b4);
    if (rc != 4 || b1 > 255 || b2 > 255 || b3 > 255 || b4 > 255) {
      return false;
    }
    return true;
  }

  uint16_t CalcChecksum(void *vdata, uint32_t size)
  {
    uint8_t *data = (uint8_t *)vdata;

    // Initialise the accumulator.
    uint64_t acc = 0xffff;

    // Handle any partial block at the start of the data.
    unsigned int offset = ((uintptr_t)data) & 3;
    if (offset) {
      size_t count = 4 - offset;
      if (count > size) count = size;
      uint32_t word = 0;
      memcpy(offset + (char *)&word, data, count);
      acc += ntohl(word);
      data += count;
      size -= count;
    }

    // Handle any complete 32-bit blocks.
    uint8_t *data_end = data + (size & ~3);
    while (data != data_end) {
      uint32_t word;
      memcpy(&word, data, 4);
      acc += ntohl(word);
      data += 4;
    }
    size &= 3;

    // Handle any partial block at the end of the data.
    if (size) {
      uint32_t word = 0;
      memcpy(&word, data, size);
      acc += ntohl(word);
    }

    // Handle deferred carries.
    acc = (acc & 0xffffffff) + (acc >> 32);
    while (acc >> 16) {
      acc = (acc & 0xffff) + (acc >> 16);
    }

    // If the data began at an odd byte address
    // then reverse the byte order to compensate.
    if (offset & 1) {
      acc = ((acc & 0xff00) >> 8) | ((acc & 0x00ff) << 8);
    }

    // Return the checksum in network byte order.
    return ~acc;
  }

}// namespace Utils
}// namespace PacketHacker

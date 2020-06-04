#ifndef _WIN32
#else
#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>
#endif

#include "utils.h"

#include <pcap.h>
#include <cstdio>

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

  bool SendPacket(AdapterInfo info, const uint8_t *data, int size, char *errbuf)
  {
    pcap_t *fp;
    char pcapErrbuf[PCAP_ERRBUF_SIZE];

    if ((fp = pcap_create(info.name.c_str(), pcapErrbuf)) == NULL) {
      sprintf(errbuf, "Unable to open the adapter. %s is not supported by Npcap", info.name.c_str());
      return false;
    }
    pcap_set_promisc(fp, 1);

    if (pcap_activate(fp) != 0) {
      sprintf(errbuf, "Error activating handle: %s", pcap_geterr(fp));
      pcap_close(fp);
      return false;
    }

    if (pcap_sendpacket(fp, data, size) != 0) {
      sprintf(errbuf, "Error sending the packet: %s", pcap_geterr(fp));
      return false;
    }

    pcap_close(fp);

    return true;
  }

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

}// namespace Utils
}// namespace PacketHacker

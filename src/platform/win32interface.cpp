#include "interface_table.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#include "utils/adapter_utils.h"

namespace PacketHacker {

void InterfaceTable::refreshTable()
{
  ULONG size = 0;
  PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
  PIP_ADAPTER_ADDRESSES pCurrAddresses = nullptr;

  ::GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &size);
  pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(size);
  if (::GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_GATEWAYS, nullptr, pAddresses, &size) == NO_ERROR) {
    pCurrAddresses = pAddresses;
    while (pCurrAddresses) {
      Interface info{};
      info.index = pCurrAddresses->IfIndex;
      info.name = "\\Device\\NPF_" + std::string(pCurrAddresses->AdapterName);
      if (pCurrAddresses->FirstUnicastAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstUnicastAddress->Address.lpSockaddr;
        info.unicastAddress = IPv4Address(ipaddr->sin_addr.s_addr);
      }
      if (pCurrAddresses->FirstAnycastAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstAnycastAddress->Address.lpSockaddr;
        info.anycastAddress = IPv4Address(ipaddr->sin_addr.s_addr);
      }
      if (pCurrAddresses->FirstMulticastAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstMulticastAddress->Address.lpSockaddr;
        info.multicastAddress = IPv4Address(ipaddr->sin_addr.s_addr);
      }
      if (pCurrAddresses->FirstDnsServerAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstDnsServerAddress->Address.lpSockaddr;
        info.dnsServerAddress = IPv4Address(ipaddr->sin_addr.s_addr);
      }
      if (pCurrAddresses->FirstGatewayAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstGatewayAddress->Address.lpSockaddr;
        info.gatewayAddress = IPv4Address(ipaddr->sin_addr.s_addr);
      }
      info.dnsSuffix = pCurrAddresses->DnsSuffix;
      info.description = pCurrAddresses->Description;
      info.friendlyName = pCurrAddresses->FriendlyName;
      info.address = HardwareAddress(pCurrAddresses->PhysicalAddress);
      m_interfaces.emplace_back(info);
      pCurrAddresses = pCurrAddresses->Next;
    }
  }

  if (pAddresses) {
    free(pAddresses);
  }
}

Interface *InterfaceTable::bestInterface(IPv4Address &address)
{
  DWORD dwBestIfIndex;
  GetBestInterface((IPAddr)address.data(), &dwBestIfIndex);
  return getInterface(dwBestIfIndex);
}

}// namespace PacketHacker
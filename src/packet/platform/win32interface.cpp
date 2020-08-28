#include "packet/interface.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#include <pcap.h>

#include "packet/utils/adapter_utils.h"

namespace PacketHacker {

const std::vector<Interface> GetInterfaces()
{
  std::vector<Interface> output;
  ULONG size = 0;
  PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
  PIP_ADAPTER_ADDRESSES pCurrAddresses = nullptr;

  ::GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &size);
  pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(size);
  if (::GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_GATEWAYS, nullptr, pAddresses, &size) == NO_ERROR) {
    pCurrAddresses = pAddresses;
    while (pCurrAddresses) {
      InterfaceInfo info{};
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
      output.emplace_back(info);
      pCurrAddresses = pCurrAddresses->Next;
    }
  }

  if (pAddresses) {
    free(pAddresses);
  }
  return output;
}

const std::vector<Interface> Interface::s_availableInterfaces = GetInterfaces();

bool Interface::OpenPacketStream(char *errbuf)
{
  if (m_streamOpen) {
    sprintf(errbuf, "Stream already opened!");
    return false;
  }

  char pcapErrbuf[PCAP_ERRBUF_SIZE];

  if ((m_handle = pcap_create(m_info.name.c_str(), pcapErrbuf)) == nullptr) {
    sprintf(errbuf, "Unable to open the adapter. %s is not supported by Npcap", m_info.name.c_str());
    return false;
  }

  pcap_set_snaplen(m_handle, 100);
  pcap_set_promisc(m_handle, PCAP_OPENFLAG_PROMISCUOUS);
  pcap_set_timeout(m_handle, 20);

  if (pcap_activate(m_handle) != 0) {
    sprintf(errbuf, "Error activating handle: %s", pcap_geterr(m_handle));
    pcap_close(m_handle);
    return false;
  }

  m_streamOpen = true;

  return true;
}

void Interface::ClosePacketStream()
{
  if (m_streamOpen) pcap_close(m_handle);
  m_streamOpen = false;
}

bool Interface::SendPacket(Packet *packet, char *errbuf)
{
  if (!m_streamOpen) {
    sprintf(errbuf, "Stream not opened!");
    return false;
  }

  const uint32_t size = packet->Size();
  std::vector<uint8_t> data(size);
  packet->WriteToBuf(data.data(), size);

  if (pcap_sendpacket(m_handle, data.data(), size) != 0) {
    sprintf(errbuf, "Error sending the packet: %s", pcap_geterr(m_handle));
    return false;
  }

  return true;
}

const uint8_t *Interface::GetNextPacket(uint32_t *size, char *errbuf)
{
  if (!m_streamOpen) {
    sprintf(errbuf, "Stream not opened!");
    return false;
  }

  int res;
  struct pcap_pkthdr *header;
  const uint8_t *pkt_data;

  while ((res = pcap_next_ex(m_handle, &header, &pkt_data)) >= 0) {
    if (res == 0) {
      continue;
    }
    *size = header->caplen;
    return pkt_data;
  }

  if (res == -1) {
    sprintf(errbuf, "Error reading the packets: %s\n", pcap_geterr(m_handle));
  }

  return nullptr;
}

const std::vector<ArpEntry> Interface::GetArpTable()
{
  std::vector<ArpEntry> output;
  ULONG size = 0;
  PMIB_IPNETTABLE IpNetTable = nullptr;

  GetIpNetTable(nullptr, &size, FALSE);
  IpNetTable = (MIB_IPNETTABLE *)malloc(size);

  if (GetIpNetTable(IpNetTable, &size, FALSE) == NO_ERROR) {
    for (int i = 0; i < IpNetTable->dwNumEntries; i++) {
      MIB_IPNETROW IpNetRow = IpNetTable->table[i];
      if (IpNetRow.dwIndex != m_info.index) continue;
      ArpEntry entry{};
      entry.hwAddress = HardwareAddress(IpNetRow.bPhysAddr);
      entry.ipAddress = IPv4Address(IpNetRow.dwAddr);
      switch (IpNetRow.dwType) {
      case MIB_IPNET_TYPE_OTHER:
        entry.type = "OTHER";
        break;
      case MIB_IPNET_TYPE_INVALID:
        entry.type = "INVALID";
        break;
      case MIB_IPNET_TYPE_DYNAMIC:
        entry.type = "DYNAMIC";
        break;
      case MIB_IPNET_TYPE_STATIC:
        entry.type = "STATIC";
        break;
      }
      output.emplace_back(entry);
    }
  }

  if (IpNetTable) {
    free(IpNetTable);
  }

  return output;
}

const Interface Interface::BestInterface(IPv4Address &address)
{
  DWORD dwBestIfIndex;
  GetBestInterface((IPAddr)address.GetData(), &dwBestIfIndex);
  auto iterator = s_availableInterfaces.begin();
  for (auto it = s_availableInterfaces.begin(); it != s_availableInterfaces.end(); it++) {
    if (it->m_info.index == dwBestIfIndex) return *it;
  }
  return Interface();
}

}// namespace PacketHacker
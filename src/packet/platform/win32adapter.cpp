#include "packet/adapter.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#include <pcap.h>

#include "packet/utils/adapter_utils.h"

namespace PacketHacker {

const std::vector<AdapterInfo> GetAdapters()
{
  std::vector<AdapterInfo> output;
  ULONG size = 0;
  PIP_ADAPTER_ADDRESSES pAddresses = nullptr;
  PIP_ADAPTER_ADDRESSES pCurrAddresses = nullptr;

  ::GetAdaptersAddresses(AF_INET, 0, nullptr, nullptr, &size);
  pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(size);
  if (::GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_GATEWAYS, nullptr, pAddresses, &size) == NO_ERROR) {
    pCurrAddresses = pAddresses;
    while (pCurrAddresses) {
      AdapterInfo info{};
      info.index = pCurrAddresses->IfIndex;
      info.name = "\\Device\\NPF_" + std::string(pCurrAddresses->AdapterName);
      if (pCurrAddresses->FirstUnicastAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstUnicastAddress->Address.lpSockaddr;
        info.unicastAddress = Utils::IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
      }
      if (pCurrAddresses->FirstAnycastAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstAnycastAddress->Address.lpSockaddr;
        info.anycastAddress = Utils::IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
      }
      if (pCurrAddresses->FirstMulticastAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstMulticastAddress->Address.lpSockaddr;
        info.multicastAddress = Utils::IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
      }
      if (pCurrAddresses->FirstDnsServerAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstDnsServerAddress->Address.lpSockaddr;
        info.dnsServerAddress = Utils::IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
      }
      if (pCurrAddresses->FirstGatewayAddress) {
        struct sockaddr_in *ipaddr = (sockaddr_in *)pCurrAddresses->FirstGatewayAddress->Address.lpSockaddr;
        info.gatewayAddress = Utils::IPv4ToString(reinterpret_cast<uint8_t *>(&ipaddr->sin_addr));
      }
      info.dnsSuffix = pCurrAddresses->DnsSuffix;
      info.description = pCurrAddresses->Description;
      info.friendlyName = pCurrAddresses->FriendlyName;
      info.address = Utils::HardwareAddressToString(pCurrAddresses->PhysicalAddress);
      output.emplace_back(info);
      pCurrAddresses = pCurrAddresses->Next;
    }
  }

  if (pAddresses) {
    free(pAddresses);
  }
  return output;
}

const std::vector<AdapterInfo> Adapter::s_availableAdapters = GetAdapters();

bool Adapter::OpenPacketStream(char *errbuf)
{
  char pcapErrbuf[PCAP_ERRBUF_SIZE];

  if ((m_handle = pcap_create(m_name.c_str(), pcapErrbuf)) == nullptr) {
    sprintf(errbuf, "Unable to open the adapter. %s is not supported by Npcap", m_name.c_str());
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

  return true;
}

void Adapter::ClosePacketStream()
{
  pcap_close(m_handle);
}

bool Adapter::SendPacket(Packet *packet, char *errbuf)
{
  const uint32_t size = packet->Size();
  std::vector<uint8_t> data(size);
  packet->WriteToBuf(data.data(), size);

  if (pcap_sendpacket(m_handle, data.data(), size) != 0) {
    sprintf(errbuf, "Error sending the packet: %s", pcap_geterr(m_handle));
    return false;
  }

  return true;
}

// TODO: save pointer to last data and delete
const uint8_t *Adapter::GetNextPacket(uint32_t *size, char *errbuf)
{
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

}// namespace PacketHacker
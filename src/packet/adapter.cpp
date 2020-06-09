#include "adapter.h"
#include "packets.h"

namespace PacketHacker {

Adapter::Adapter(std::string name)
  : m_name(name)
{
}

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
  uint8_t *data = new uint8_t[size];
  packet->WriteToBuf(data, size);

  if (pcap_sendpacket(m_handle, data, size) != 0) {
    sprintf(errbuf, "Error sending the packet: %s", pcap_geterr(m_handle));
    return false;
  }

  delete[] data;

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
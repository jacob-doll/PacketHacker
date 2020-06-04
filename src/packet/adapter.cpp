#include "adapter.h"

namespace PacketHacker {

Adapter::Adapter(std::string name)
  : m_name(name)
{
}

bool Adapter::OpenPacketStream(char *errbuf)
{
  char pcapErrbuf[PCAP_ERRBUF_SIZE];

  if ((m_handle = pcap_create(m_name.c_str(), pcapErrbuf)) == NULL) {
    sprintf(errbuf, "Unable to open the adapter. %s is not supported by Npcap", m_name.c_str());
    return false;
  }
  pcap_set_snaplen(m_handle, 65536);
  pcap_set_promisc(m_handle, 1);
  pcap_set_timeout(m_handle, 1000);

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

  return true;
}

Packet *Adapter::GetNextPacket()
{
  printf("Getting Data\n");
  int res;
  struct pcap_pkthdr *header;
  const uint8_t *pkt_data;

  while ((res = pcap_next_ex(m_handle, &header, &pkt_data)) >= 0) {

    if (res == 0) {
      /* Timeout elapsDed */
      continue;
    }

    /* print pkt timestamp and pkt len */
    printf("%ld:%ld (%ld)\n", header->ts.tv_sec, header->ts.tv_usec, header->len);

    /* Print the packet */
    for (int i = 1; (i < header->caplen + 1); i++) {
      printf("%.2x ", pkt_data[i - 1]);
      if ((i % 16) == 0) printf("\n");
    }

    printf("\n\n");
  }

  if (res == -1) {
    printf("Error reading the packets: %s\n", pcap_geterr(m_handle));
  }

  printf("\n");
  return nullptr;
}

}// namespace PacketHacker
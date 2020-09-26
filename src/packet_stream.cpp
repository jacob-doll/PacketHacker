#include "packet_stream.h"

namespace PacketHacker {

PacketStream::PacketStream(Interface *streamInterface)
  : m_streamInterface(streamInterface), m_streamOpen(false)
{
}

void PacketStream::openPacketStream()
{
  if (m_streamOpen) {
    throw StreamException("Stream already opened!");
  }

  char pcapErrbuf[PCAP_ERRBUF_SIZE];

  if ((m_handle = pcap_create(m_streamInterface->name.c_str(), pcapErrbuf)) == nullptr) {
    throw StreamException("Unable to open the adapter.");
  }

  pcap_set_snaplen(m_handle, 100);
  pcap_set_promisc(m_handle, PCAP_OPENFLAG_PROMISCUOUS);
  pcap_set_timeout(m_handle, 20);

  if (pcap_activate(m_handle) != 0) {
    pcap_close(m_handle);
    throw StreamException(pcap_geterr(m_handle));
  }

  m_streamOpen = true;
}

void PacketStream::closePacketStream()
{
  if (m_streamOpen) pcap_close(m_handle);
  m_streamOpen = false;
}

void PacketStream::sendPacket(Packet *packet)
{
  if (!m_streamOpen) {
    throw StreamException("Stream not opened!");
  }

  const uint32_t size = packet->size();
  std::vector<uint8_t> data(size);
  packet->writeToBuf(data.data(), size);

  if (pcap_sendpacket(m_handle, data.data(), size) != 0) {
    throw StreamException(pcap_geterr(m_handle));
  }
}

const uint8_t *PacketStream::getNextPacket(uint32_t *size)
{
  if (!m_streamOpen) {
    throw StreamException("Stream not opened!");
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
    throw StreamException(pcap_geterr(m_handle));
  }

  return nullptr;
}

}// namespace PacketHacker
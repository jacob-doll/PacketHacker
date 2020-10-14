#include "packet_stream.h"

namespace PacketHacker {

PacketStream::PacketStream(Interface *streamInterface)
  : m_streamInterface(streamInterface), m_streamOpen(false)
{
}

bool PacketStream::openPacketStream()
{
  if (m_streamOpen) {
    return false;
  }

  char pcapErrbuf[PCAP_ERRBUF_SIZE];

  if ((m_handle = pcap_create(m_streamInterface->name.c_str(), pcapErrbuf)) == nullptr) {
    return false;
  }

  pcap_set_snaplen(m_handle, 100);
  pcap_set_promisc(m_handle, PCAP_OPENFLAG_PROMISCUOUS);
  pcap_set_timeout(m_handle, 20);

  if (pcap_activate(m_handle) != 0) {
    pcap_close(m_handle);
    return false;
  }

  m_streamOpen = true;
  return true;
}

void PacketStream::closePacketStream()
{
  if (m_streamOpen) pcap_close(m_handle);
  m_streamOpen = false;
}

bool PacketStream::sendPacket(Packet &packet)
{
  if (!m_streamOpen) {
    return false;
  }

  const uint32_t size = packet.size();
  std::vector<uint8_t> data(size);
  packet.writeToBuf(data.data(), size);

  if (pcap_sendpacket(m_handle, data.data(), size) != 0) {
    return false;
  }

  return true;
}

const DataType *PacketStream::getNextPacket(SizeType *size)
{
  if (!m_streamOpen) {
    return nullptr;
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
    return nullptr;
  }

  return nullptr;
}

}// namespace PacketHacker
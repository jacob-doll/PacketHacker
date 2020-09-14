#pragma once

#include <pcap.h>

#include "packet.h"
#include "interface_table.h"

namespace PacketHacker {

class PacketStream
{
public:
  PacketStream(Interface *streamInterface);

  bool OpenPacketStream(char *errbuf);
  void ClosePacketStream();
  bool SendPacket(Packet *packet, char *errbuf);
  const uint8_t *GetNextPacket(uint32_t *size, char *errbuf);
  bool IsStreamOpen() const { return m_streamOpen; }

private:
  Interface *m_streamInterface;
  pcap *m_handle;
  bool m_streamOpen;
};

}// namespace PacketHacker
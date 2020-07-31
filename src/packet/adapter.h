#pragma once

#include <string>
#include <stdint.h>
#include <pcap.h>

#include "packet/packet.h"

namespace PacketHacker {

class Adapter
{
public:
  Adapter(std::string name);

  bool OpenPacketStream(char *errbuf);
  void ClosePacketStream();
  bool SendPacket(Packet *packet, char *errbuf);
  const uint8_t *GetNextPacket(uint32_t *size, char *errbuf);

private:
  const std::string m_name;
  pcap_t *m_handle;
};

}// namespace PacketHacker
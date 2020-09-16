#pragma once

#include "packet.h"

namespace PacketHacker {

class UdpPacket : public Packet
{
public:
  UdpPacket();
  UdpPacket(const uint8_t *data, uint32_t size);

  void SetSrcPort(const uint16_t srcPort);
  void SetDstPort(const uint16_t dstPort);
  void SetLength(const uint16_t length);
  void SetChecksum(const uint16_t checksum);

  virtual const PacketType GetPacketType() const override { return PacketType::UDP; }
  virtual const std::string GetName() const override { return "Udp"; }
  virtual const uint32_t HeaderSize() const override { return sizeof(UdpHeader); }
  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  virtual void DoWriteToBuf(uint8_t *buffer) override;

private:
#pragma pack(push, 1)
  struct UdpHeader
  {
    uint16_t srcPort;
    uint16_t dstPort;
    uint16_t length;
    uint16_t checksum;
  };
#pragma pack(pop)

  UdpHeader m_header;
};

}// namespace PacketHacker
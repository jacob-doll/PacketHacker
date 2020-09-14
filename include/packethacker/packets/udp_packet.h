#pragma once

#include "packet.h"

namespace PacketHacker {

class UdpPacket : public Packet
{
public:
  UdpPacket();
  UdpPacket(const uint8_t *data, uint32_t size);

  void SetSrcPort(const FieldData &val);
  void SetDstPort(const FieldData &val);
  void SetLength(const FieldData &val);
  void SetChecksum(const FieldData &val);

  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;
  virtual uint32_t HeaderSize() const override;
  virtual std::string GetName() const override { return "Udp"; }
  virtual PacketType GetPacketType() const override { return PacketType::UDP; }

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
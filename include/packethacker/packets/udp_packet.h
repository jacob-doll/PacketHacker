#pragma once

#include "packet.h"

#include "utils/buffer_utils.h"

namespace PacketHacker {

class UdpPacket : public Packet
{
public:
  UdpPacket();
  UdpPacket(const uint8_t *data, uint32_t size);

  uint16_t srcPort() { return BYTE_SWAP_16(m_header.srcPort); }
  uint16_t dstPort() { return BYTE_SWAP_16(m_header.dstPort); }
  uint16_t length() { return BYTE_SWAP_16(m_header.length); }
  uint16_t checksum() { return BYTE_SWAP_16(m_header.checksum); }

  void srcPort(const uint16_t srcPort);
  void dstPort(const uint16_t dstPort);
  void length(const uint16_t length);
  void checksum(const uint16_t checksum);

  virtual const PacketType packetType() const override { return PacketType::UDP; }
  virtual const std::string name() const override { return "Udp"; }
  virtual const uint32_t headerSize() const override { return sizeof(UdpHeader); }
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  virtual void doWriteToBuf(uint8_t *buffer) override;

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
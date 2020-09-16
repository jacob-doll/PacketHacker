#pragma once

#include "packet.h"

#include "utils/buffer_utils.h"

namespace PacketHacker {

class UdpPacket : public Packet
{
public:
  UdpPacket();
  UdpPacket(const uint8_t *data, uint32_t size);

  uint16_t GetSrcPort() { return BYTE_SWAP_16(m_header.srcPort); }
  uint16_t GetDstPort() { return BYTE_SWAP_16(m_header.dstPort); }
  uint16_t GetLength() { return BYTE_SWAP_16(m_header.length); }
  uint16_t GetChecksum() { return BYTE_SWAP_16(m_header.checksum); }

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
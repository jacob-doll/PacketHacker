#pragma once

#include "packet.h"

#include "utils/buffer_utils.h"

namespace PacketHacker {

class IcmpPacket : public Packet
{
public:
  IcmpPacket();
  IcmpPacket(const uint8_t *data, uint32_t size);

  uint8_t type() { return m_header.type; }
  uint8_t code() { return m_header.code; }
  uint16_t checksum() { return BYTE_SWAP_16(m_header.checksum); }
  uint32_t data() { return BYTE_SWAP_32(m_header.data); }

  void type(const uint8_t type);
  void code(const uint8_t code);
  void checksum(const uint16_t checksum);
  void data(const uint32_t data);

  virtual const PacketType packetType() const override { return PacketType::ICMP; }
  virtual const std::string name() const override { return "ICMP"; }
  virtual const uint32_t headerSize() const override { return sizeof(IcmpHeader); }
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  virtual void doWriteToBuf(uint8_t *buffer) override;

private:
#pragma pack(push, 1)
  struct IcmpHeader
  {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t data;
  };
#pragma pack(pop)

  IcmpHeader m_header;
};

}// namespace PacketHacker
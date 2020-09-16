#pragma once

#include "packet.h"

#include "utils/buffer_utils.h"

namespace PacketHacker {

class IcmpPacket : public Packet
{
public:
  IcmpPacket();
  IcmpPacket(const uint8_t *data, uint32_t size);

  uint8_t GetType() { return m_header.type; }
  uint8_t GetCode() { return m_header.code; }
  uint16_t GetChecksum() { return BYTE_SWAP_16(m_header.checksum); }
  uint32_t GetData() { return BYTE_SWAP_32(m_header.data); }

  void SetType(const uint8_t type);
  void SetCode(const uint8_t code);
  void SetChecksum(const uint16_t checksum);
  void SetData(const uint32_t data);

  virtual const PacketType GetPacketType() const override { return PacketType::ICMP; }
  virtual const std::string GetName() const override { return "ICMP"; }
  virtual const uint32_t HeaderSize() const override { return sizeof(IcmpHeader); }
  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  virtual void DoWriteToBuf(uint8_t *buffer) override;

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
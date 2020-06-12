#pragma once

#include "packet.h"

namespace PacketHacker {

class IcmpPacket : public Packet
{
public:
  IcmpPacket();
  IcmpPacket(const uint8_t *data, uint32_t size);

  void SetType(const char *val);
  void SetCode(const char *val);
  void SetChecksum(const char *val);
  void SetData(const char *val);

  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;
  virtual uint32_t HeaderSize() const override;
  virtual std::string GetName() const override { return "ICMP"; }

protected:
  virtual void DoWriteToBuf(uint8_t *buffer, uint32_t &offset) override;

private:
#pragma pack(push, 1)
  struct IcmpHeader
  {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint64_t data;
  };
#pragma pack(pop)

  IcmpHeader m_header;
};

}// namespace PacketHacker
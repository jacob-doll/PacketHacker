#pragma once

#include "packet/packet.h"

namespace PacketHacker {

class EthernetPacket : public Packet
{
public:
  EthernetPacket();
  EthernetPacket(const uint8_t *data, uint32_t size);

  void SetDst(const char *val);
  void SetSrc(const char *val);
  void SetType(const char *val);

  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;
  virtual uint32_t HeaderSize() const override { return sizeof(EthernetHeader); }
  virtual std::string GetName() const override { return "Ethernet"; }
  virtual PacketType GetPacketType() const override { return PacketType::ETHERNET; }

protected:
  virtual void DoWriteToBuf(uint8_t *buffer) override;

private:
  struct EthernetHeader
  {
    uint8_t dstMac[6];
    uint8_t srcMac[6];
    uint16_t type;
  };

  EthernetHeader m_header;
};

}// namespace PacketHacker
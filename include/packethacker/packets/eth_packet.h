#pragma once

#include "hardware_address.h"

#include "packet.h"

namespace PacketHacker {

class EthernetPacket : public Packet
{
public:
  EthernetPacket();
  EthernetPacket(const uint8_t *data, uint32_t size);

  void SetDst(const HardwareAddress &dst);
  void SetSrc(const HardwareAddress &src);
  void SetType(const uint16_t type);

  virtual const PacketType GetPacketType() const override { return PacketType::ETHERNET; }
  virtual const std::string GetName() const override { return "Ethernet"; }
  virtual const uint32_t HeaderSize() const override { return sizeof(EthernetHeader); }
  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;

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
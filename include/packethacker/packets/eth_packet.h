#pragma once

#include "hardware_address.h"
#include "utils/buffer_utils.h"

#include "packet.h"

namespace PacketHacker {

class EthernetPacket : public Packet
{
public:
  EthernetPacket();
  EthernetPacket(const uint8_t *data, uint32_t size);

  HardwareAddress GetDst() { return HardwareAddress(m_header.dstMac); }
  HardwareAddress GetSrc() { return HardwareAddress(m_header.srcMac); }
  uint16_t GetType() { return BYTE_SWAP_16(m_header.type); }

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
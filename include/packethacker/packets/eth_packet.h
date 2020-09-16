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

  HardwareAddress dstMac() { return HardwareAddress(m_header.dstMac); }
  HardwareAddress srcMac() { return HardwareAddress(m_header.srcMac); }
  uint16_t type() { return BYTE_SWAP_16(m_header.type); }

  void dstMac(const HardwareAddress &dst);
  void srcMac(const HardwareAddress &src);
  void type(const uint16_t type);

  virtual const PacketType packetType() const override { return PacketType::ETHERNET; }
  virtual const std::string name() const override { return "Ethernet"; }
  virtual const uint32_t headerSize() const override { return sizeof(EthernetHeader); }
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  virtual void doWriteToBuf(uint8_t *buffer) override;

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
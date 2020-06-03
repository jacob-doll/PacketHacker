#pragma once

#include "packet.h"

namespace PacketHacker {

class EthernetPacket : public Packet
{
public:
  EthernetPacket();

  void SetDst(const char *val);
  void SetSrc(const char *val);
  void SetType(const char *val);

  virtual uint32_t HeaderSize() const override { return sizeof(EthernetHeader); }
  virtual std::string GetName() const override { return "Ethernet"; }
  virtual std::string ToString() override;

protected:
  virtual void DoParse(uint8_t *buffer) override;
  virtual void DoWriteToBuf(uint8_t *buffer, uint32_t &offset) override;

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
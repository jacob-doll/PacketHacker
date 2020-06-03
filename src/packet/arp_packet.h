#pragma once

#include "packet.h"

namespace PacketHacker {

class ArpPacket : public Packet
{
public:
  ArpPacket();
  ArpPacket(const uint8_t *data, uint32_t size);

  void SetHardwareType(const char *val);
  void SetProtocolType(const char *val);
  void SetHardwareLength(const char *val);
  void SetProtocolLength(const char *val);
  void SetOpcode(const char *val);
  void SetSenderMac(const char *val);
  void SetSenderIp(const char *val);
  void SetTargetMac(const char *val);
  void SetTargetIp(const char *val);

  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;
  virtual uint32_t HeaderSize() const override { return sizeof(ArpHeader); }
  virtual std::string GetName() const override { return "ARP"; }
  virtual std::string ToString() override;

protected:
  virtual void DoWriteToBuf(uint8_t *buffer, uint32_t &offset) override;

private:
  struct ArpHeader
  {
    uint16_t hardwareType;
    uint16_t protocolType;
    uint8_t hardwareLength;
    uint8_t protocolLength;
    uint16_t opcode;
    uint8_t senderMac[6];
    uint8_t senderIp[4];
    uint8_t targetMac[6];
    uint8_t targetIp[4];
  };

  ArpHeader m_header;
};

}// namespace PacketHacker
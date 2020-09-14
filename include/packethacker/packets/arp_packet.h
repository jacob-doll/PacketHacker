#pragma once

#include "packet.h"

namespace PacketHacker {

class ArpPacket : public Packet
{
public:
  ArpPacket();
  ArpPacket(const uint8_t *data, uint32_t size);

  void SetHardwareType(const FieldData &val);
  void SetProtocolType(const FieldData &val);
  void SetHardwareLength(const FieldData &val);
  void SetProtocolLength(const FieldData &val);
  void SetOpcode(const FieldData &val);
  void SetSenderMac(const FieldData &val);
  void SetSenderIp(const FieldData &val);
  void SetTargetMac(const FieldData &val);
  void SetTargetIp(const FieldData &val);

  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;
  virtual uint32_t HeaderSize() const override;
  virtual std::string GetName() const override { return "ARP"; }
  virtual PacketType GetPacketType() const override { return PacketType::ARP; }

protected:
  virtual void DoWriteToBuf(uint8_t *buffer) override;

private:
#pragma pack(push, 1)
  struct ArpHeader
  {
    uint16_t hardwareType;
    uint16_t protocolType;
    uint8_t hardwareLength;
    uint8_t protocolLength;
    uint16_t opcode;
    uint8_t senderMac[6];
    uint32_t senderIp;
    uint8_t targetMac[6];
    uint32_t targetIp;
  };
#pragma pack(pop)

  ArpHeader m_header;
};

}// namespace PacketHacker
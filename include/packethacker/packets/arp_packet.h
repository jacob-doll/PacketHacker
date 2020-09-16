#pragma once

#include "hardware_address.h"
#include "ip_address.h"
#include "utils/buffer_utils.h"

#include "packet.h"

namespace PacketHacker {

class ArpPacket : public Packet
{
public:
  ArpPacket();
  ArpPacket(const uint8_t *data, uint32_t size);

  uint16_t hardwareType() { return BYTE_SWAP_16(m_header.hardwareType); }
  uint16_t protocolType() { return BYTE_SWAP_16(m_header.protocolType); }
  uint8_t hardwareLength() { return m_header.hardwareLength; }
  uint8_t protocolLength() { return m_header.protocolLength; }
  uint16_t opcode() { return BYTE_SWAP_16(m_header.opcode); }
  HardwareAddress senderMac() { return HardwareAddress(m_header.senderMac); }
  IPv4Address senderIp() { return IPv4Address(m_header.senderIp); }
  HardwareAddress targetMac() { return HardwareAddress(m_header.targetMac); }
  IPv4Address targetIp() { return IPv4Address(m_header.targetIp); }

  void hardwareType(const uint16_t hardwareType);
  void protocolType(const uint16_t protocolType);
  void hardwareLength(const uint8_t hardwareLength);
  void protocolLength(const uint8_t protocolLength);
  void opcode(const uint16_t opcode);
  void senderMac(const HardwareAddress &senderMac);
  void senderIp(const IPv4Address &senderIp);
  void targetMac(const HardwareAddress &targetMac);
  void targetIp(const IPv4Address &targetIp);

  virtual const PacketType packetType() const override { return PacketType::ARP; }
  virtual const std::string name() const override { return "ARP"; }
  virtual const uint32_t headerSize() const override { return sizeof(ArpHeader); }
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  virtual void doWriteToBuf(uint8_t *buffer) override;

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
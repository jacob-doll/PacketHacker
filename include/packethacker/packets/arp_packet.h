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

  uint16_t GetHardwareType() { return BYTE_SWAP_16(m_header.hardwareType); }
  uint16_t GetProtocolType() { return BYTE_SWAP_16(m_header.protocolType); }
  uint8_t GetHardwareLength() { return m_header.hardwareLength; }
  uint8_t GetProtocolLength() { return m_header.protocolLength; }
  uint16_t GetOpcode() { return BYTE_SWAP_16(m_header.opcode); }
  HardwareAddress GetSenderMac() { return HardwareAddress(m_header.senderMac); }
  IPv4Address GetSenderIp() { return IPv4Address(m_header.senderIp); }
  HardwareAddress GetTargetMac() { return HardwareAddress(m_header.targetMac); }
  IPv4Address GetTargetIp() { return IPv4Address(m_header.targetIp); }

  void SetHardwareType(const uint16_t hardwareType);
  void SetProtocolType(const uint16_t protocolType);
  void SetHardwareLength(const uint8_t hardwareLength);
  void SetProtocolLength(const uint8_t protocolLength);
  void SetOpcode(const uint16_t opcode);
  void SetSenderMac(const HardwareAddress &senderMac);
  void SetSenderIp(const IPv4Address &senderIp);
  void SetTargetMac(const HardwareAddress &targetMac);
  void SetTargetIp(const IPv4Address &targetIp);

  virtual const PacketType GetPacketType() const override { return PacketType::ARP; }
  virtual const std::string GetName() const override { return "ARP"; }
  virtual const uint32_t HeaderSize() const override { return sizeof(ArpHeader); }
  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;

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
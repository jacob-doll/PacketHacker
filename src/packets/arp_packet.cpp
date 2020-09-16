#include "packets/arp_packet.h"
#include "utils/buffer_utils.h"
#include "utils/adapter_utils.h"

namespace PacketHacker {

ArpPacket::ArpPacket()
  : m_header(), Packet()
{
  // HeaderField *hardwareType = new HeaderFieldImpl<ArpPacket>(this, "Hardware Type", "0x0001", &ArpPacket::SetHardwareType);
  // HeaderField *protocolType = new HeaderFieldImpl<ArpPacket>(this, "Protocol Type", "0x0800", &ArpPacket::SetProtocolType);
  // HeaderField *hardwareLength = new HeaderFieldImpl<ArpPacket>(this, "Hardware Length", "0x06", &ArpPacket::SetHardwareLength);
  // HeaderField *protocolLength = new HeaderFieldImpl<ArpPacket>(this, "Protocol Length", "0x04", &ArpPacket::SetProtocolLength);
  // HeaderField *opcode = new HeaderFieldImpl<ArpPacket>(this, "Opcode", "0x0001", &ArpPacket::SetOpcode);
  // HeaderField *senderMac = new HeaderFieldImpl<ArpPacket>(this, "Sender Hardware Address", "00:00:00:00:00:00", &ArpPacket::SetSenderMac, true, FieldType::FIELD_HARDWARE);
  // HeaderField *senderIp = new HeaderFieldImpl<ArpPacket>(this, "Sender Protocol Address", "0.0.0.0", &ArpPacket::SetSenderIp, true, FieldType::FIELD_IP);
  // HeaderField *targetMac = new HeaderFieldImpl<ArpPacket>(this, "Target Hardware Address", "00:00:00:00:00:00", &ArpPacket::SetTargetMac, true, FieldType::FIELD_HARDWARE);
  // HeaderField *targetIp = new HeaderFieldImpl<ArpPacket>(this, "Target Protocol Address", "0.0.0.0", &ArpPacket::SetTargetIp, true, FieldType::FIELD_IP);

  // HeaderField *hardwareType = HEADER_FIELD_INT16(ArpPacket, "Hardware Type", 0x0001u, true, SetHardwareType);
  // HeaderField *protocolType = HEADER_FIELD_INT16(ArpPacket, "Protocol Type", 0x0800u, true, SetProtocolType);
  // HeaderField *hardwareLength = HEADER_FIELD_INT8(ArpPacket, "Hardware Length", 0x06u, true, SetHardwareLength);
  // HeaderField *protocolLength = HEADER_FIELD_INT8(ArpPacket, "Protocol Length", 0x04u, true, SetProtocolLength);
  // HeaderField *opcode = HEADER_FIELD_INT16(ArpPacket, "Opcode", 0x0001u, true, SetOpcode);
  // HeaderField *senderMac = HEADER_FIELD_HARDWARE(ArpPacket, "Sender Hardware Address", HardwareAddress(), true, SetSenderMac);
  // HeaderField *senderIp = HEADER_FIELD_IPv4(ArpPacket, "Sender Protocol Address", IPv4Address(), true, SetSenderIp);
  // HeaderField *targetMac = HEADER_FIELD_HARDWARE(ArpPacket, "Target Hardware Address", HardwareAddress(), true, SetTargetMac);
  // HeaderField *targetIp = HEADER_FIELD_IPv4(ArpPacket, "Target Protocol Address", IPv4Address(), true, SetTargetIp);

  // m_fields = { hardwareType, protocolType, hardwareLength, protocolLength, opcode, senderMac, senderIp, targetMac, targetIp };
  // Init();
}

ArpPacket::ArpPacket(const uint8_t *data, uint32_t size)
  : ArpPacket()
{
  if (size < HeaderSize()) {
    return;
  }
  Utils::ReadValue(data, m_header);

  // char buf[8];
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.hardwareType));
  // GetField("Hardware Type")->SetValue(BYTE_SWAP_16(m_header.hardwareType));
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.protocolType));
  // GetField("Protocol Type")->SetValue(BYTE_SWAP_16(m_header.protocolType));
  // sprintf(buf, "0x%02x", m_header.hardwareLength);
  // GetField("Hardware Length")->SetValue(m_header.hardwareLength);
  // sprintf(buf, "0x%02x", m_header.protocolLength);
  // GetField("Protocol Length")->SetValue(m_header.protocolLength);
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.opcode));
  // GetField("Opcode")->SetValue(BYTE_SWAP_16(m_header.opcode));
  // GetField("Sender Hardware Address")->SetValue(HardwareAddress(m_header.senderMac));
  // GetField("Sender Protocol Address")->SetValue(IPv4Address(m_header.senderIp));
  // GetField("Target Hardware Address")->SetValue(HardwareAddress(m_header.targetMac));
  // GetField("Target Protocol Address")->SetValue(IPv4Address(m_header.targetIp));
}

void ArpPacket::SetHardwareType(const uint16_t hardwareType)
{
  m_header.hardwareType = BYTE_SWAP_16(hardwareType);
}

void ArpPacket::SetProtocolType(const uint16_t protocolType)
{
  m_header.protocolType = BYTE_SWAP_16(protocolType);
}

void ArpPacket::SetHardwareLength(const uint8_t hardwareLength)
{
  m_header.hardwareLength = hardwareLength;
}

void ArpPacket::SetProtocolLength(const uint8_t protocolLength)
{
  m_header.protocolLength = protocolLength;
}

void ArpPacket::SetOpcode(const uint16_t opcode)
{
  m_header.opcode = BYTE_SWAP_16(opcode);
}

void ArpPacket::SetSenderMac(const HardwareAddress &senderMac)
{
  Utils::Write(m_header.senderMac, senderMac.GetData().data(), PHYSICAL_ADDR_LEN);
}

void ArpPacket::SetSenderIp(const IPv4Address &senderIp)
{
  m_header.senderIp = senderIp.GetData();
}

void ArpPacket::SetTargetMac(const HardwareAddress &targetMac)
{
  Utils::Write(m_header.targetMac, targetMac.GetData().data(), PHYSICAL_ADDR_LEN);
}

void ArpPacket::SetTargetIp(const IPv4Address &targetIp)
{
  m_header.targetIp = targetIp.GetData();
}

bool ArpPacket::DoesReplyMatch(const uint8_t *buffer, uint32_t size)
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return false;
  }
  const ArpHeader *header = (const ArpHeader *)buffer;
  return (m_header.targetIp == header->senderIp) && (m_header.senderIp == header->targetIp);
}

void ArpPacket::DoWriteToBuf(uint8_t *buffer)
{
  Utils::WriteValue(buffer, m_header);
}

}// namespace PacketHacker
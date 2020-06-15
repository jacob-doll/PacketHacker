#include "arp_packet.h"
#include "utils/utils.h"

namespace PacketHacker {

ArpPacket::ArpPacket()
  : m_header(), Packet()
{
  HeaderField *hardwareType = new HeaderFieldImpl<ArpPacket>(this, "Hardware Type", "0x0001", &ArpPacket::SetHardwareType);
  m_fields.emplace_back(hardwareType);
  HeaderField *protocolType = new HeaderFieldImpl<ArpPacket>(this, "Protocol Type", "0x0800", &ArpPacket::SetProtocolType);
  m_fields.emplace_back(protocolType);
  HeaderField *hardwareLength = new HeaderFieldImpl<ArpPacket>(this, "Hardware Length", "0x06", &ArpPacket::SetHardwareLength);
  m_fields.emplace_back(hardwareLength);
  HeaderField *protocolLength = new HeaderFieldImpl<ArpPacket>(this, "Protocol Length", "0x04", &ArpPacket::SetProtocolLength);
  m_fields.emplace_back(protocolLength);
  HeaderField *opcode = new HeaderFieldImpl<ArpPacket>(this, "Opcode", "0x0001", &ArpPacket::SetOpcode);
  m_fields.emplace_back(opcode);
  HeaderField *senderMac = new HeaderFieldImpl<ArpPacket>(this, "Sender Hardware Address", "00:00:00:00:00:00", &ArpPacket::SetSenderMac, true, FieldType::FIELD_HARDWARE);
  m_fields.emplace_back(senderMac);
  HeaderField *senderIp = new HeaderFieldImpl<ArpPacket>(this, "Sender Protocol Address", "0.0.0.0", &ArpPacket::SetSenderIp, true, FieldType::FIELD_IP);
  m_fields.emplace_back(senderIp);
  HeaderField *targetMac = new HeaderFieldImpl<ArpPacket>(this, "Target Hardware Address", "00:00:00:00:00:00", &ArpPacket::SetTargetMac, true, FieldType::FIELD_HARDWARE);
  m_fields.emplace_back(targetMac);
  HeaderField *targetIp = new HeaderFieldImpl<ArpPacket>(this, "Target Protocol Address", "0.0.0.0", &ArpPacket::SetTargetIp, true, FieldType::FIELD_IP);
  m_fields.emplace_back(targetIp);
  Init();
}

ArpPacket::ArpPacket(const uint8_t *data, uint32_t size)
  : ArpPacket()
{
  if (size < HeaderSize()) {
    return;
  }
  Utils::ReadValue(data, m_header);

  char buf[8];
  sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.hardwareType));
  GetField("Hardware Type")->SetValue(buf);
  sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.protocolType));
  GetField("Protocol Type")->SetValue(buf);
  sprintf(buf, "0x%02x", m_header.hardwareLength);
  GetField("Hardware Length")->SetValue(buf);
  sprintf(buf, "0x%02x", m_header.protocolLength);
  GetField("Protocol Length")->SetValue(buf);
  sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.opcode));
  GetField("Opcode")->SetValue(buf);
  GetField("Sender Hardware Address")->SetValue(Utils::HardwareAddressToString(m_header.senderMac).c_str());
  GetField("Sender Protocol Address")->SetValue(Utils::IPv4ToString(m_header.senderIp).c_str());
  GetField("Target Hardware Address")->SetValue(Utils::HardwareAddressToString(m_header.targetMac).c_str());
  GetField("Target Protocol Address")->SetValue(Utils::IPv4ToString(m_header.targetIp).c_str());
}

void ArpPacket::SetHardwareType(const char *val)
{
  uint16_t data = std::stoi(val, 0, 16);
  m_header.hardwareType = BYTE_SWAP_16(data);
  GetField("Hardware Type")->SetValue(val);
}

void ArpPacket::SetProtocolType(const char *val)
{
  uint16_t data = std::stoi(val, 0, 16);
  m_header.protocolType = BYTE_SWAP_16(data);
  GetField("Protocol Type")->SetValue(val);
}

void ArpPacket::SetHardwareLength(const char *val)
{
  uint8_t data = std::stoi(val, 0, 16);
  m_header.hardwareLength = data;
  GetField("Hardware Length")->SetValue(val);
}

void ArpPacket::SetProtocolLength(const char *val)
{
  uint8_t data = std::stoi(val, 0, 16);
  m_header.protocolLength = data;
  GetField("Protocol Length")->SetValue(val);
}

void ArpPacket::SetOpcode(const char *val)
{
  uint16_t data = std::stoi(val, 0, 16);
  m_header.opcode = BYTE_SWAP_16(data);
  GetField("Opcode")->SetValue(val);
}

void ArpPacket::SetSenderMac(const char *val)
{
  uint64_t data = Utils::HardwareToLong(val);
  for (int i = 0; i < 6; i++) {
    m_header.senderMac[6 - 1 - i] = (data >> (i * 8));
  }
  GetField("Sender Hardware Address")->SetValue(val);
}

void ArpPacket::SetSenderIp(const char *val)
{
  uint32_t data = Utils::IPv4ToLong(val);
  m_header.senderIp = BYTE_SWAP_32(data);
  GetField("Sender Protocol Address")->SetValue(val);
}

void ArpPacket::SetTargetMac(const char *val)
{
  uint64_t data = Utils::HardwareToLong(val);
  for (int i = 0; i < 6; i++) {
    m_header.targetMac[6 - 1 - i] = (data >> (i * 8));
  }
  GetField("Target Hardware Address")->SetValue(val);
}

void ArpPacket::SetTargetIp(const char *val)
{
  uint32_t data = Utils::IPv4ToLong(val);
  m_header.targetIp = BYTE_SWAP_32(data);
  GetField("Target Protocol Address")->SetValue(val);
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

uint32_t ArpPacket::HeaderSize() const
{
  return sizeof(ArpHeader);
}

void ArpPacket::DoWriteToBuf(uint8_t *buffer, uint32_t &offset)
{
  buffer += offset;
  Utils::WriteValue(buffer, m_header);
  offset += HeaderSize();
}

}// namespace PacketHacker
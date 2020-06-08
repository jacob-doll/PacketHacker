#include "arp_packet.h"
#include "utils/utils.h"

#include <sstream>

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
  HeaderField *senderMac = new HeaderFieldImpl<ArpPacket>(this, "Sender Hardware Address", "00:00:00:00:00:00", &ArpPacket::SetSenderMac);
  m_fields.emplace_back(senderMac);
  HeaderField *senderIp = new HeaderFieldImpl<ArpPacket>(this, "Sender Protocol Address", "0.0.0.0", &ArpPacket::SetSenderIp);
  m_fields.emplace_back(senderIp);
  HeaderField *targetMac = new HeaderFieldImpl<ArpPacket>(this, "Target Hardware Address", "00:00:00:00:00:00", &ArpPacket::SetTargetMac);
  m_fields.emplace_back(targetMac);
  HeaderField *targetIp = new HeaderFieldImpl<ArpPacket>(this, "Target Protocol Address", "0.0.0.0", &ArpPacket::SetTargetIp);
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
  m_header.hardwareType = BYTE_SWAP_16(m_header.hardwareType);
  m_header.protocolType = BYTE_SWAP_16(m_header.protocolType);
  m_header.opcode = BYTE_SWAP_16(m_header.opcode);

  char buf[8];
  sprintf(buf, "%04x", m_header.hardwareType);
  GetField("Hardware Type")->SetValue(buf);
  sprintf(buf, "%04x", m_header.protocolType);
  GetField("Protocol Type")->SetValue(buf);
  sprintf(buf, "%02x", m_header.hardwareLength);
  GetField("Hardware Length")->SetValue(buf);
  sprintf(buf, "%02x", m_header.protocolLength);
  GetField("Protocol Length")->SetValue(buf);
  sprintf(buf, "%04x", m_header.opcode);
  GetField("Opcode")->SetValue(buf);
  GetField("Sender Hardware Address")->SetValue(Utils::HardwareAddressToString(m_header.senderMac).c_str());
  GetField("Sender Protocol Address")->SetValue(Utils::IPv4ToString(m_header.senderIp).c_str());
  GetField("Target Hardware Address")->SetValue(Utils::HardwareAddressToString(m_header.targetMac).c_str());
  GetField("Target Protocol Address")->SetValue(Utils::IPv4ToString(m_header.targetIp).c_str());
}

void ArpPacket::SetHardwareType(const char *val)
{
  uint16_t data = std::stoi(val, 0, 16);
  m_header.hardwareType = data;
}

void ArpPacket::SetProtocolType(const char *val)
{
  uint16_t data = std::stoi(val, 0, 16);
  m_header.protocolType = data;
}

void ArpPacket::SetHardwareLength(const char *val)
{
  uint8_t data = std::stoi(val, 0, 16);
  m_header.hardwareLength = data;
}

void ArpPacket::SetProtocolLength(const char *val)
{
  uint8_t data = std::stoi(val, 0, 16);
  m_header.protocolLength = data;
}

void ArpPacket::SetOpcode(const char *val)
{
  uint16_t data = std::stoi(val, 0, 16);
  m_header.opcode = data;
}

void ArpPacket::SetSenderMac(const char *val)
{
  uint64_t data = Utils::HardwareToLong(val);
  for (int i = 0; i < 6; i++) {
    m_header.senderMac[6 - 1 - i] = (data >> (i * 8));
  }
}

void ArpPacket::SetSenderIp(const char *val)
{
  uint32_t data = Utils::IPv4ToLong(val);
  for (int i = 0; i < 4; i++) {
    m_header.senderIp[4 - 1 - i] = (data >> (i * 8));
  }
}

void ArpPacket::SetTargetMac(const char *val)
{
  uint64_t data = Utils::HardwareToLong(val);
  for (int i = 0; i < 6; i++) {
    m_header.targetMac[6 - 1 - i] = (data >> (i * 8));
  }
}

void ArpPacket::SetTargetIp(const char *val)
{
  uint32_t data = Utils::IPv4ToLong(val);
  for (int i = 0; i < 4; i++) {
    m_header.targetIp[4 - 1 - i] = (data >> (i * 8));
  }
}

bool ArpPacket::DoesReplyMatch(const uint8_t *buffer, uint32_t size)
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return false;
  }
  const ArpHeader *header = (const ArpHeader *)buffer;
  return Utils::BufferEquals<4>(m_header.targetIp, header->senderIp) && Utils::BufferEquals<4>(m_header.senderIp, header->targetIp);
}

void ArpPacket::DoWriteToBuf(uint8_t *buffer, uint32_t &offset)
{
  buffer += offset;
  Utils::WriteValue(buffer, BYTE_SWAP_16(m_header.hardwareType));
  buffer += sizeof(m_header.hardwareType);
  Utils::WriteValue(buffer, BYTE_SWAP_16(m_header.protocolType));
  buffer += sizeof(m_header.protocolType);
  Utils::WriteValue(buffer, m_header.hardwareLength);
  buffer += sizeof(m_header.hardwareLength);
  Utils::WriteValue(buffer, m_header.protocolLength);
  buffer += sizeof(m_header.protocolLength);
  Utils::WriteValue(buffer, BYTE_SWAP_16(m_header.opcode));
  buffer += sizeof(m_header.opcode);
  Utils::Write(buffer, m_header.senderMac, 6);
  buffer += 6;
  Utils::Write(buffer, m_header.senderIp, 4);
  buffer += 4;
  Utils::Write(buffer, m_header.targetMac, 6);
  buffer += 6;
  Utils::Write(buffer, m_header.targetIp, 4);
  buffer += 4;
  offset += HeaderSize();
}

std::string ArpPacket::ToString()
{
  std::stringstream ss;
  ss << GetName() << " (size: " << HeaderSize() << "): {\n";
  ss << "\thardwareType: " << std::hex << m_header.hardwareType << "\n";
  ss << "\tprotocolType: " << std::hex << m_header.protocolType << "\n";
  ss << "\thardwareLength: " << std::hex << m_header.hardwareLength << "\n";
  ss << "\tprotocolLength: " << std::hex << m_header.protocolLength << "\n";
  ss << "\topcode: " << std::hex << m_header.opcode << "\n";
  ss << "\tsenderMac: " << Utils::HardwareAddressToString(m_header.senderMac) << "\n";
  ss << "\tsenderIp: " << Utils::IPv4ToString(m_header.senderIp) << "\n";
  ss << "\ttargetMac: " << Utils::HardwareAddressToString(m_header.targetMac) << "\n";
  ss << "\ttargetIp: " << Utils::IPv4ToString(m_header.targetIp) << "\n";
  ss << "}";
  return ss.str();
}

}// namespace PacketHacker
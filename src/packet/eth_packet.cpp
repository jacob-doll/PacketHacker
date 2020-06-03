#include "eth_packet.h"
#include "utils/utils.h"

#include <sstream>

namespace PacketHacker {

EthernetPacket::EthernetPacket()
  : m_header(), Packet()
{
  HeaderField *dst = new HeaderFieldImpl<EthernetPacket>(this, "Dst", "00:00:00:00:00:00", &EthernetPacket::SetDst);
  m_fields.emplace_back(dst);
  HeaderField *src = new HeaderFieldImpl<EthernetPacket>(this, "Src", "00:00:00:00:00:00", &EthernetPacket::SetSrc);
  m_fields.emplace_back(src);
  HeaderField *type = new HeaderFieldImpl<EthernetPacket>(this, "Type", "0x0800", &EthernetPacket::SetType);
  m_fields.emplace_back(type);
  Init();
}

void EthernetPacket::SetDst(const char *val)
{
  uint64_t data = Utils::HardwareToLong(val);
  for (int i = 0; i < 6; i++) {
    m_header.dstMac[6 - 1 - i] = (data >> (i * 8));
  }
}

void EthernetPacket::SetSrc(const char *val)
{
  uint64_t data = Utils::HardwareToLong(val);
  for (int i = 0; i < 6; i++) {
    m_header.srcMac[6 - 1 - i] = (data >> (i * 8));
  }
}

void EthernetPacket::SetType(const char *val)
{
  uint16_t data = std::stoi(val, 0, 16);
  m_header.type = data;
}

void EthernetPacket::DoParse(uint8_t *buffer)
{
  Utils::ReadValue(buffer, m_header);
}

void EthernetPacket::DoWriteToBuf(uint8_t *buffer, uint32_t &offset)
{
  buffer += offset;
  Utils::Write(buffer, m_header.dstMac, 6);
  buffer += 6;
  Utils::Write(buffer, m_header.srcMac, 6);
  buffer += 6;
  Utils::WriteValue(buffer, BYTE_SWAP_16(m_header.type));
  offset += HeaderSize();
}

std::string EthernetPacket::ToString() 
{
  std::stringstream ss;
  ss << GetName() << " (size: " << HeaderSize() << "): {\n";
  ss << "\tdst: " << Utils::HardwareAddressToString(m_header.dstMac) << "\n";
  ss << "\tsrc: " << Utils::HardwareAddressToString(m_header.srcMac) << "\n";
  ss << "\ttype: " << std::hex << m_header.type << "\n";
  ss << "}";
  return ss.str();
}

}// namespace PacketHacker
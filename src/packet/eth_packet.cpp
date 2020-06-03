#include "eth_packet.h"
#include "utils/utils.h"
#include "utils/packet_utils.h"

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

EthernetPacket::EthernetPacket(const uint8_t *data, uint32_t size)
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);
  m_header.type = BYTE_SWAP_16(m_header.type);
  size = size - headerSize;
  if (size > 0) {
    SetInnerPacket(Utils::PacketFromType(m_header.type, (uint8_t *)(data + headerSize), size));
  }
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

bool EthernetPacket::DoesReplyMatch(const uint8_t *buffer, uint32_t size)
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return false;
  }

  const EthernetHeader *header = (const EthernetHeader *)buffer;
  size = size - headerSize;
  if (Utils::BufferEquals<6>(m_header.srcMac, header->dstMac)) {
    return GetInnerPacket() ? GetInnerPacket()->DoesReplyMatch((uint8_t *)(buffer + headerSize), size) : true;
  }

  return false;
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
  if (GetInnerPacket()) {
    ss << GetInnerPacket()->ToString() << "\n";
  }
  ss << "}";
  return ss.str();
}

}// namespace PacketHacker
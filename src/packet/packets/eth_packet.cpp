#include "eth_packet.h"
#include "packet/utils/buffer_utils.h"
#include "packet/utils/adapter_utils.h"
#include "packet/utils/packet_utils.h"

namespace PacketHacker {

EthernetPacket::EthernetPacket()
  : m_header(), Packet()
{
  HeaderField *dst = new HeaderFieldImpl<EthernetPacket>(this, "Dst", "00:00:00:00:00:00", &EthernetPacket::SetDst, true, FieldType::FIELD_HARDWARE);
  m_fields.emplace_back(dst);
  HeaderField *src = new HeaderFieldImpl<EthernetPacket>(this, "Src", "00:00:00:00:00:00", &EthernetPacket::SetSrc, true, FieldType::FIELD_HARDWARE);
  m_fields.emplace_back(src);
  HeaderField *type = new HeaderFieldImpl<EthernetPacket>(this, "EtherType", "0x0800", &EthernetPacket::SetType);
  m_fields.emplace_back(type);
  Init();
}

EthernetPacket::EthernetPacket(const uint8_t *data, uint32_t size)
  : EthernetPacket()
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);

  GetField("Dst")->SetValue(Utils::HardwareAddressToString(m_header.dstMac).c_str());
  GetField("Src")->SetValue(Utils::HardwareAddressToString(m_header.srcMac).c_str());
  char buf[8];
  sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.type));
  GetField("EtherType")->SetValue(buf);

  size = size - headerSize;
  if (size > 0) {
    SetInnerPacket(Utils::PacketFromType(BYTE_SWAP_16(m_header.type), (uint8_t *)(data + headerSize), size));
  }
}

void EthernetPacket::SetDst(const char *val)
{
  uint64_t data = Utils::HardwareToLong(val);
  for (int i = 0; i < 6; i++) {
    m_header.dstMac[6 - 1 - i] = (data >> (i * 8));
  }
  GetField("Dst")->SetValue(val);
}

void EthernetPacket::SetSrc(const char *val)
{
  uint64_t data = Utils::HardwareToLong(val);
  for (int i = 0; i < 6; i++) {
    m_header.srcMac[6 - 1 - i] = (data >> (i * 8));
  }
  GetField("Src")->SetValue(val);
}

void EthernetPacket::SetType(const char *val)
{
  uint16_t data = std::stoi(val, 0, 16);
  m_header.type = BYTE_SWAP_16(data);
  GetField("EtherType")->SetValue(val);
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

void EthernetPacket::DoWriteToBuf(uint8_t *buffer)
{
  Utils::WriteValue(buffer, m_header);
}

}// namespace PacketHacker
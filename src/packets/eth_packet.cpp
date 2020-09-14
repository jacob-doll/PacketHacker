#include "packets/eth_packet.h"
#include "utils/buffer_utils.h"
#include "utils/adapter_utils.h"
#include "utils/packet_utils.h"

namespace PacketHacker {

EthernetPacket::EthernetPacket()
  : m_header(), Packet()
{
  // HeaderField *dst = new HeaderFieldImpl<EthernetPacket>(this, "Dst", "00:00:00:00:00:00", &EthernetPacket::SetDst, true, FieldType::FIELD_HARDWARE);
  // HeaderField *src = new HeaderFieldImpl<EthernetPacket>(this, "Src", "00:00:00:00:00:00", &EthernetPacket::SetSrc, true, FieldType::FIELD_HARDWARE);
  // HeaderField *type = new HeaderFieldImpl<EthernetPacket>(this, "EtherType", "0x0800", &EthernetPacket::SetType);

  HeaderField *dst = HEADER_FIELD_HARDWARE(EthernetPacket, "Dst", HardwareAddress(), true, SetDst);
  HeaderField *src = HEADER_FIELD_HARDWARE(EthernetPacket, "Src", HardwareAddress(), true, SetSrc);
  HeaderField *type = HEADER_FIELD_INT16(EthernetPacket, "EtherType", 0x0800, true, SetType);

  m_fields = { dst, src, type };
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

  GetField("Dst")->SetValue(HardwareAddress(m_header.dstMac));
  GetField("Src")->SetValue(HardwareAddress(m_header.srcMac));
  // char buf[8];
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.type));
  GetField("EtherType")->SetValue(BYTE_SWAP_16(m_header.type));

  size = size - headerSize;
  if (size > 0) {
    SetInnerPacket(Utils::PacketFromType(BYTE_SWAP_16(m_header.type), (uint8_t *)(data + headerSize), size));
  }
}

void EthernetPacket::SetDst(const FieldData &val)
{
  HardwareAddress data = std::get<HardwareAddress>(val);

  // Utils::StringToHardwareAddress(val, m_header.dstMac, PHYSICAL_ADDR_LEN);
  Utils::Write(m_header.dstMac, data.GetData().data(), PHYSICAL_ADDR_LEN);
  GetField("Dst")->SetValue(val);
}

void EthernetPacket::SetSrc(const FieldData &val)
{
  HardwareAddress data = std::get<HardwareAddress>(val);
  // Utils::StringToHardwareAddress(val, m_header.srcMac, PHYSICAL_ADDR_LEN);
  Utils::Write(m_header.srcMac, data.GetData().data(), PHYSICAL_ADDR_LEN);
  GetField("Src")->SetValue(val);
}

void EthernetPacket::SetType(const FieldData &val)
{
  uint16_t data = std::get<uint16_t>(val);
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
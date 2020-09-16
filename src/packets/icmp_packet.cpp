#include "packets/icmp_packet.h"
#include "utils/buffer_utils.h"
#include "utils/adapter_utils.h"
#include "packets/data_packet.h"

namespace PacketHacker {

IcmpPacket::IcmpPacket()
  : m_header(), Packet()
{
  // HeaderField *type = new HeaderFieldImpl<IcmpPacket>(this, "Type", "0", &IcmpPacket::SetType);
  // HeaderField *code = new HeaderFieldImpl<IcmpPacket>(this, "Code", "0", &IcmpPacket::SetCode);
  // HeaderField *checksum = new HeaderFieldImpl<IcmpPacket>(this, "Checksum", "0x0000", &IcmpPacket::SetChecksum, false);
  // HeaderField *data = new HeaderFieldImpl<IcmpPacket>(this, "Icmp Data", "0", &IcmpPacket::SetData);

  // HeaderField *type = HEADER_FIELD_INT8(IcmpPacket, "Type", 0u, true, SetType);
  // HeaderField *code = HEADER_FIELD_INT8(IcmpPacket, "Code", 0u, true, SetCode);
  // HeaderField *checksum = HEADER_FIELD_INT16(IcmpPacket, "Checksum", 0u, false, SetChecksum);
  // HeaderField *data = HEADER_FIELD_INT32(IcmpPacket, "Icmp Data", 0u, true, SetData);

  // m_fields = { type, code, checksum, data };
  // Init();
}

IcmpPacket::IcmpPacket(const uint8_t *data, uint32_t size)
  : IcmpPacket()
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);

  // char buf[8];
  // sprintf(buf, "%d", m_header.type);
  // GetField("Type")->SetValue(m_header.type);
  // sprintf(buf, "%d", m_header.code);
  // GetField("Code")->SetValue(m_header.code);
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.checksum));
  // GetField("Checksum")->SetValue(BYTE_SWAP_16(m_header.checksum));
  // sprintf(buf, "%x", m_header.data);
  // GetField("Icmp Data")->SetValue(m_header.data);

  size = size - headerSize;
  if (size > 0) {
    SetInnerPacket(new DataPacket((uint8_t *)(data + headerSize), size));
  }
}

void IcmpPacket::SetType(const uint8_t type)
{
  m_header.type = type;
}

void IcmpPacket::SetCode(const uint8_t code)
{
  m_header.code = code;
}

void IcmpPacket::SetChecksum(const uint16_t checksum)
{
  m_header.checksum = BYTE_SWAP_16(checksum);
}

void IcmpPacket::SetData(const uint32_t data)
{
  m_header.data = BYTE_SWAP_32(data);
}

bool IcmpPacket::DoesReplyMatch(const uint8_t *buffer, uint32_t size) { return true; }

void IcmpPacket::DoWriteToBuf(uint8_t *buffer)
{
  m_header.checksum = 0x0000;
  Utils::WriteValue(buffer, m_header);

  uint16_t checksum = Utils::CalcChecksum((void *)buffer, Size());
  // char buf[6];
  // sprintf(buf, "0x%04x", checksum);
  // GetField("Checksum")->SetValue(checksum);

  Utils::WriteValue((uint8_t *)(buffer + 2), BYTE_SWAP_16(checksum));
}

}// namespace PacketHacker
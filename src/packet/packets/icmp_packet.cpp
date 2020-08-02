#include "icmp_packet.h"
#include "packet/utils/buffer_utils.h"
#include "packet/utils/adapter_utils.h"
#include "data_packet.h"

namespace PacketHacker {

IcmpPacket::IcmpPacket()
  : m_header(), Packet()
{
  HeaderField *type = new HeaderFieldImpl<IcmpPacket>(this, "Type", "0", &IcmpPacket::SetType);
  HeaderField *code = new HeaderFieldImpl<IcmpPacket>(this, "Code", "0", &IcmpPacket::SetCode);
  HeaderField *checksum = new HeaderFieldImpl<IcmpPacket>(this, "Checksum", "0x0000", &IcmpPacket::SetChecksum, false);
  HeaderField *data = new HeaderFieldImpl<IcmpPacket>(this, "Icmp Data", "0", &IcmpPacket::SetData);
  m_fields = { type, code, checksum, data };
  Init();
}

IcmpPacket::IcmpPacket(const uint8_t *data, uint32_t size)
  : IcmpPacket()
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);

  char buf[8];
  sprintf(buf, "%d", m_header.type);
  GetField("Type")->SetValue(buf);
  sprintf(buf, "%d", m_header.code);
  GetField("Code")->SetValue(buf);
  sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.checksum));
  GetField("Checksum")->SetValue(buf);
  sprintf(buf, "%x", m_header.data);
  GetField("Icmp Data")->SetValue(buf);

  size = size - headerSize;
  if (size > 0) {
    SetInnerPacket(new DataPacket((uint8_t *)(data + headerSize), size));
  }
}

void IcmpPacket::SetType(const char *val)
{
  uint8_t data = std::stoi(val, 0, 0);
  m_header.type = data;
  GetField("Type")->SetValue(val);
}

void IcmpPacket::SetCode(const char *val)
{
  uint8_t data = std::stoi(val, 0, 0);
  m_header.code = data;
  GetField("Code")->SetValue(val);
}
void IcmpPacket::SetChecksum(const char *val) {}
void IcmpPacket::SetData(const char *val)
{
  uint32_t data = std::stoi(val, 0, 0);
  m_header.data = BYTE_SWAP_32(data);
  GetField("Icmp Data")->SetValue(val);
}

bool IcmpPacket::DoesReplyMatch(const uint8_t *buffer, uint32_t size) { return true; }
uint32_t IcmpPacket::HeaderSize() const { return sizeof(IcmpHeader); }

void IcmpPacket::DoWriteToBuf(uint8_t *buffer)
{
  m_header.checksum = 0x0000;
  Utils::WriteValue(buffer, m_header);

  uint16_t checksum = Utils::CalcChecksum((void *)buffer, Size());
  char buf[6];
  sprintf(buf, "0x%04x", checksum);
  GetField("Checksum")->SetValue(buf);

  Utils::WriteValue((uint8_t *)(buffer + 2), BYTE_SWAP_16(checksum));
}


}// namespace PacketHacker
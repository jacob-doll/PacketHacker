#include "udp_packet.h"
#include "data_packet.h"
#include "ip_packet.h"
#include "packet/utils/buffer_utils.h"
#include "packet/utils/adapter_utils.h"
#include "packet/constants.h"

namespace PacketHacker {

UdpPacket::UdpPacket()
  : m_header(), Packet()
{
  // HeaderField *srcPort = new HeaderFieldImpl<UdpPacket>(this, "Src Port", 0u, FieldType::FIELD_INT16, &UdpPacket::SetSrcPort);
  // HeaderField *dstPort = new HeaderFieldImpl<UdpPacket>(this, "Dst Port", 0u, FieldType::FIELD_INT16, &UdpPacket::SetDstPort);
  // HeaderField *length = new HeaderFieldImpl<UdpPacket>(this, "Udp Length", 0u, FieldType::FIELD_INT16, &UdpPacket::SetLength, false);
  // HeaderField *checksum = new HeaderFieldImpl<UdpPacket>(this, "Udp Checksum", 0u, FieldType::FIELD_INT16, &UdpPacket::SetChecksum, false);

  HeaderField *srcPort = HEADER_FIELD_INT16(UdpPacket, "Src Port", 0u, true, SetSrcPort);
  HeaderField *dstPort = HEADER_FIELD_INT16(UdpPacket, "Src Port", 0u, true, SetSrcPort);
  HeaderField *length = HEADER_FIELD_INT16(UdpPacket, "Udp Length", 0u, true, SetSrcPort);
  HeaderField *checksum = HEADER_FIELD_INT16(UdpPacket, "Udp Checksum", 0u, true, SetSrcPort);

  m_fields = { srcPort, dstPort, length, checksum };
  Init();
}

UdpPacket::UdpPacket(const uint8_t *data, uint32_t size)
  : UdpPacket()
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);

  // char buf[8];
  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.srcPort));
  GetField("Src Port")->SetValue(BYTE_SWAP_16(m_header.srcPort));

  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.dstPort));
  GetField("Dst Port")->SetValue(BYTE_SWAP_16(m_header.dstPort));

  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.length));
  GetField("Udp Length")->SetValue(BYTE_SWAP_16(m_header.length));

  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.checksum));
  GetField("Udp Checksum")->SetValue(BYTE_SWAP_16(m_header.checksum));

  size = size - headerSize;
  if (size > 0) {
    SetInnerPacket(new DataPacket((uint8_t *)(data + headerSize), size));
  }
}

void UdpPacket::SetSrcPort(const FieldData &val)
{
  uint16_t data = std::get<uint16_t>(val);
  m_header.srcPort = BYTE_SWAP_16(data);
  GetField("Src Port")->SetValue(val);
}

void UdpPacket::SetDstPort(const FieldData &val)
{
  uint16_t data = std::get<uint16_t>(val);
  m_header.dstPort = BYTE_SWAP_16(data);
  GetField("Dst Port")->SetValue(val);
}

void UdpPacket::SetLength(const FieldData &val) {}

void UdpPacket::SetChecksum(const FieldData &val) {}

bool UdpPacket::DoesReplyMatch(const uint8_t *buffer, uint32_t size)
{
  uint16_t headerSize = HeaderSize();
  if (size < headerSize) {
    return false;
  }

  const UdpHeader *header = (const UdpHeader *)buffer;
  size = size - headerSize;

  if (m_header.dstPort == header->srcPort && m_header.srcPort == header->dstPort) {
    return GetInnerPacket() ? GetInnerPacket()->DoesReplyMatch((uint8_t *)(buffer + headerSize), size) : true;
  }

  return false;
}

uint32_t UdpPacket::HeaderSize() const { return sizeof(UdpHeader); }

void UdpPacket::DoWriteToBuf(uint8_t *buffer)
{
  uint16_t size = Size();
  m_header.checksum = 0x0000;
  m_header.length = BYTE_SWAP_16(size);
  GetField("Udp Length")->SetValue(size);

  Utils::WriteValue(buffer, m_header);


  std::vector<uint8_t> psuedo_header(size + 12);
  if (GetOuterPacket()->GetPacketType() == PacketType::IP) {
    IpPacket *ip = static_cast<IpPacket *>(GetOuterPacket());
    Utils::WriteValue(&psuedo_header[0], ip->GetSourceIp());
    Utils::WriteValue(&psuedo_header[4], ip->GetDestIp());
    Utils::WriteValue(&psuedo_header[8], (uint8_t)0);
    Utils::WriteValue(&psuedo_header[9], (uint8_t)Constants::IP::TYPE_UDP);
    Utils::WriteValue(&psuedo_header[10], (uint16_t)m_header.length);
  }


  Utils::Write(&psuedo_header[12], buffer, size);

  uint16_t checksum = Utils::CalcChecksum((void *)&psuedo_header[0], size + 12);
  // char buf[6];
  // sprintf(buf, "0x%04x", checksum);
  GetField("Udp Checksum")->SetValue(checksum);

  Utils::WriteValue((uint8_t *)(buffer + 6), BYTE_SWAP_16(checksum));
}

}// namespace PacketHacker
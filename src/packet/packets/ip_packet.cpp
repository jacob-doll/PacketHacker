#include "ip_packet.h"
#include "packet/utils/buffer_utils.h"
#include "packet/utils/adapter_utils.h"
#include "packet/utils/packet_utils.h"

namespace PacketHacker {

IpPacket::IpPacket()
  : m_header(), Packet()
{
  // HeaderField *version = new HeaderFieldImpl<IpPacket>(this, "Version", "4", &IpPacket::SetVersion, false);
  // HeaderField *headerLength = new HeaderFieldImpl<IpPacket>(this, "Header Length", "5", &IpPacket::SetHeaderLength);
  // HeaderField *diffServices = new HeaderFieldImpl<IpPacket>(this, "Differentiated Services", "0x00", &IpPacket::SetDiffServices);
  // HeaderField *totalLength = new HeaderFieldImpl<IpPacket>(this, "Total Length", "20", &IpPacket::SetTotalLength, false);
  // HeaderField *id = new HeaderFieldImpl<IpPacket>(this, "Identification", "0x0000", &IpPacket::SetId);
  // HeaderField *flags = new HeaderFieldImpl<IpPacket>(this, "Flags", "0x00", &IpPacket::SetFlags);
  // HeaderField *fragOffset = new HeaderFieldImpl<IpPacket>(this, "Fragment offset", "0x0000", &IpPacket::SetFragOffset);
  // HeaderField *ttl = new HeaderFieldImpl<IpPacket>(this, "Time to live", "128", &IpPacket::SetTtl);
  // HeaderField *protocol = new HeaderFieldImpl<IpPacket>(this, "Protocol", "0x00", &IpPacket::SetProtocol);
  // HeaderField *checksum = new HeaderFieldImpl<IpPacket>(this, "Header Checksum", "0x0000", &IpPacket::SetChecksum, false);
  // HeaderField *sourceIp = new HeaderFieldImpl<IpPacket>(this, "Source", "0.0.0.0", &IpPacket::SetSourceIp, true, FieldType::FIELD_IP);
  // HeaderField *destIp = new HeaderFieldImpl<IpPacket>(this, "Destination", "0.0.0.0", &IpPacket::SetDestIp, true, FieldType::FIELD_IP);
  HeaderField *version = HEADER_FIELD_INT8(IpPacket, "Version", 4u, false, SetVersion);
  HeaderField *headerLength = HEADER_FIELD_INT8(IpPacket, "Header Length", 4u, true, SetHeaderLength);
  HeaderField *diffServices = HEADER_FIELD_INT8(IpPacket, "Differentiated Services", 0u, true, SetDiffServices);
  HeaderField *totalLength = HEADER_FIELD_INT16(IpPacket, "Total Length", 20u, false, SetTotalLength);
  HeaderField *id = HEADER_FIELD_INT16(IpPacket, "Identification", 0u, true, SetId);
  HeaderField *flags = HEADER_FIELD_INT16(IpPacket, "Flags", 0u, true, SetFlags);
  HeaderField *fragOffset = HEADER_FIELD_INT16(IpPacket, "Fragment offset", 0u, true, SetFragOffset);
  HeaderField *ttl = HEADER_FIELD_INT8(IpPacket, "Time to live", 128u, true, SetTtl);
  HeaderField *protocol = HEADER_FIELD_INT8(IpPacket, "Protocol", 0u, true, SetProtocol);
  HeaderField *checksum = HEADER_FIELD_INT16(IpPacket, "Header Checksum", 0u, false, SetChecksum);
  HeaderField *sourceIp = HEADER_FIELD_IPv4(IpPacket, "Source", IPv4Address(), true, SetSourceIp);
  HeaderField *destIp = HEADER_FIELD_IPv4(IpPacket, "Destination", IPv4Address(), true, SetDestIp);
  m_fields = { version, headerLength, diffServices, totalLength, id, flags, fragOffset, ttl, protocol, checksum, sourceIp, destIp };
  Init();
}

IpPacket::IpPacket(const uint8_t *data, uint32_t size)
  : IpPacket()
{
  uint32_t headerSize = HeaderSize();
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);

  // char buf[8];
  // sprintf(buf, "%d", m_header.version);
  GetField("Version")->SetValue((uint8_t)m_header.version);
  // sprintf(buf, "%d", m_header.headerLength);
  GetField("Header Length")->SetValue((uint8_t)m_header.headerLength);
  // sprintf(buf, "0x%02x", m_header.dscpEcn);
  GetField("Differentiated Services")->SetValue(m_header.dscpEcn);
  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.totalLength));
  GetField("Total Length")->SetValue(BYTE_SWAP_16(m_header.totalLength));
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.id));
  GetField("Identification")->SetValue(BYTE_SWAP_16(m_header.id));
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.flagsOffset));
  GetField("Flags")->SetValue(BYTE_SWAP_16(m_header.flagsOffset));
  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.flagsOffset) & (0x1fff));
  GetField("Fragment offset")->SetValue(BYTE_SWAP_16(m_header.flagsOffset));
  // sprintf(buf, "%d", m_header.ttl);
  GetField("Time to live")->SetValue(m_header.ttl);
  // sprintf(buf, "0x%02x", m_header.protocol);
  GetField("Protocol")->SetValue(m_header.protocol);
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.checksum));
  GetField("Header Checksum")->SetValue(BYTE_SWAP_16(m_header.checksum));
  GetField("Source")->SetValue(IPv4Address(m_header.sourceIp));
  GetField("Destination")->SetValue(IPv4Address(m_header.destIp));

  size = size - headerSize;
  if (size > 0) {
    SetInnerPacket(Utils::PacketFromType(m_header.protocol, (uint8_t *)(data + headerSize), size));
  }
}

void IpPacket::SetVersion(const FieldData &val)
{
  uint8_t data = std::get<uint8_t>(val);
  m_header.version = data;
  GetField("Version")->SetValue(val);
}

void IpPacket::SetHeaderLength(const FieldData &val)
{
  uint8_t data = std::get<uint8_t>(val);
  m_header.headerLength = data;
  GetField("Header Length")->SetValue(val);
}

void IpPacket::SetDiffServices(const FieldData &val)
{
  uint8_t data = std::get<uint8_t>(val);
  m_header.dscpEcn = data;
  GetField("Differentiated Services")->SetValue(val);
}

void IpPacket::SetTotalLength(const FieldData &val)
{
  uint16_t data = std::get<uint16_t>(val);
  m_header.totalLength = BYTE_SWAP_16(data);
  GetField("Total Length")->SetValue(val);
}

void IpPacket::SetId(const FieldData &val)
{
  uint16_t data = std::get<uint16_t>(val);
  m_header.id = BYTE_SWAP_16(data);
  GetField("Identification")->SetValue(val);
}

void IpPacket::SetFlags(const FieldData &val)
{
  uint16_t data = std::get<uint16_t>(val);
  m_header.flags = data;
  GetField("Flags")->SetValue(val);
}

void IpPacket::SetFragOffset(const FieldData &val)
{
  uint16_t data = std::get<uint16_t>(val);
  m_header.fragOffset = data;
  GetField("Fragment offset")->SetValue(val);
}

void IpPacket::SetTtl(const FieldData &val)
{
  uint8_t data = std::get<uint8_t>(val);
  m_header.ttl = data;
  GetField("Time to live")->SetValue(val);
}

void IpPacket::SetProtocol(const FieldData &val)
{
  uint8_t data = std::get<uint8_t>(val);
  m_header.protocol = data;
  GetField("Protocol")->SetValue(val);
}

void IpPacket::SetChecksum(const FieldData &val)
{
  uint16_t data = std::get<uint16_t>(val);
  m_header.checksum = BYTE_SWAP_16(data);
  GetField("Header Checksum")->SetValue(val);
}

void IpPacket::SetSourceIp(const FieldData &val)
{
  IPv4Address data = std::get<IPv4Address>(val);
  m_header.sourceIp = BYTE_SWAP_32(data.GetData());
  GetField("Source")->SetValue(val);
}

void IpPacket::SetDestIp(const FieldData &val)
{
  IPv4Address data = std::get<IPv4Address>(val);
  m_header.destIp = BYTE_SWAP_32(data.GetData());
  GetField("Destination")->SetValue(val);
}

bool IpPacket::DoesReplyMatch(const uint8_t *buffer, uint32_t size)
{
  uint16_t headerSize = HeaderSize();
  if (size < headerSize) {
    return false;
  }

  const IpHeader *header = (const IpHeader *)buffer;
  size = size - headerSize;

  if (m_header.destIp == header->sourceIp && m_header.sourceIp == header->destIp && m_header.protocol == header->protocol) {
    return GetInnerPacket() ? GetInnerPacket()->DoesReplyMatch((uint8_t *)(buffer + headerSize), size) : true;
  }

  return false;
}

uint32_t IpPacket::HeaderSize() const { return sizeof(IpHeader); }

void IpPacket::DoWriteToBuf(uint8_t *buffer)
{
  uint16_t size = this->Size();
  m_header.totalLength = BYTE_SWAP_16(size);
  GetField("Total Length")->SetValue(size);

  m_header.checksum = 0x0000;
  uint16_t checksum = Utils::CalcChecksum((void *)&m_header, sizeof(m_header));
  m_header.checksum = BYTE_SWAP_16(checksum);
  // char buf[6];
  // sprintf(buf, "0x%04x", checksum);
  GetField("Header Checksum")->SetValue(checksum);

  Utils::WriteValue(buffer, m_header);
  buffer += 6;
  Utils::WriteValue(buffer, BYTE_SWAP_16(m_header.flagsOffset));
}

}// namespace PacketHacker
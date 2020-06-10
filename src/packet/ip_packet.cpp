#include "ip_packet.h"
#include "utils/utils.h"

namespace PacketHacker {

IpPacket::IpPacket()
  : m_header(), Packet()
{
  HeaderField *version = new HeaderFieldImpl<IpPacket>(this, "Version", "4", &IpPacket::SetVersion, false);
  HeaderField *headerLength = new HeaderFieldImpl<IpPacket>(this, "Header Length", "5", &IpPacket::SetHeaderLength);
  HeaderField *diffServices = new HeaderFieldImpl<IpPacket>(this, "Differentiated Services", "0x00", &IpPacket::SetDiffServices);
  HeaderField *totalLength = new HeaderFieldImpl<IpPacket>(this, "Total Length", "0", &IpPacket::SetTotalLength, false);
  HeaderField *id = new HeaderFieldImpl<IpPacket>(this, "Identification", "0x0000", &IpPacket::SetId);
  HeaderField *flags = new HeaderFieldImpl<IpPacket>(this, "Flags", "0x00", &IpPacket::SetFlags);
  HeaderField *fragOffset = new HeaderFieldImpl<IpPacket>(this, "Fragment offset", "0x0000", &IpPacket::SetFragOffset);
  HeaderField *ttl = new HeaderFieldImpl<IpPacket>(this, "Time to live", "20", &IpPacket::SetTtl);
  HeaderField *protocol = new HeaderFieldImpl<IpPacket>(this, "Protocol", "0x00", &IpPacket::SetProtocol);
  HeaderField *checksum = new HeaderFieldImpl<IpPacket>(this, "Checksum", "0x0000", &IpPacket::SetChecksum, false);
  HeaderField *sourceIp = new HeaderFieldImpl<IpPacket>(this, "Source", "0.0.0.0", &IpPacket::SetSourceIp);
  HeaderField *destIp = new HeaderFieldImpl<IpPacket>(this, "Destination", "0.0.0.0", &IpPacket::SetDestIp);
  m_fields = { version, headerLength, diffServices, totalLength, id, flags, fragOffset, ttl, protocol, checksum, sourceIp, destIp };
  Init();
}

IpPacket::IpPacket(const uint8_t *data, uint32_t size)
  : IpPacket()
{
  if (size < HeaderSize()) {
    return;
  }
  Utils::ReadValue(data, m_header);

  char buf[8];
  sprintf(buf, "%d", m_header.version);
  GetField("Version")->SetValue(buf);
  sprintf(buf, "%d", m_header.headerLength);
  GetField("Header Length")->SetValue(buf);
  sprintf(buf, "0x%02x", m_header.dscpEcn);
  GetField("Differentiated Services")->SetValue(buf);
  sprintf(buf, "%d", BYTE_SWAP_16(m_header.totalLength));
  GetField("Total Length")->SetValue(buf);
  sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.id));
  GetField("Identification")->SetValue(buf);
  sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.flagsOffset));
  GetField("Flags")->SetValue(buf);
  sprintf(buf, "%d", BYTE_SWAP_16(m_header.flagsOffset) & (0x1fff));
  GetField("Fragment offset")->SetValue(buf);
  sprintf(buf, "%d", m_header.ttl);
  GetField("Time to live")->SetValue(buf);
  sprintf(buf, "0x%02x", m_header.protocol);
  GetField("Protocol")->SetValue(buf);
  sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.checksum));
  GetField("Checksum")->SetValue(buf);
  GetField("Source")->SetValue(Utils::IPv4ToString(m_header.sourceIp).c_str());
  GetField("Destination")->SetValue(Utils::IPv4ToString(m_header.destIp).c_str());
}

void IpPacket::SetVersion(const char *val)
{
  uint8_t data = std::stoi(val, 0, 0);
  m_header.version = data;
  GetField("Version")->SetValue(val);
}

void IpPacket::SetHeaderLength(const char *val)
{
  uint8_t data = std::stoi(val, 0, 0);
  m_header.headerLength = data;
  GetField("Header Length")->SetValue(val);
}

void IpPacket::SetDiffServices(const char *val)
{
  uint8_t data = std::stoi(val, 0, 0);
  m_header.dscpEcn = data;
  GetField("Differentiated Services")->SetValue(val);
}

void IpPacket::SetTotalLength(const char *val)
{
  uint16_t data = std::stoi(val, 0, 0);
  m_header.totalLength = BYTE_SWAP_16(data);
  GetField("Total Length")->SetValue(val);
}

void IpPacket::SetId(const char *val)
{
  uint16_t data = std::stoi(val, 0, 0);
  m_header.id = BYTE_SWAP_16(data);
  GetField("Identification")->SetValue(val);
}

void IpPacket::SetFlags(const char *val)
{
  uint16_t data = std::stoi(val, 0, 0);
  m_header.flags = data;
  GetField("Flags")->SetValue(val);
}

void IpPacket::SetFragOffset(const char *val)
{
  uint16_t data = std::stoi(val, 0, 0);
  m_header.fragOffset = data;
  GetField("Fragment offset")->SetValue(val);
}

void IpPacket::SetTtl(const char *val)
{
  uint8_t data = std::stoi(val, 0, 0);
  m_header.ttl = data;
  GetField("Time to live")->SetValue(val);
}

void IpPacket::SetProtocol(const char *val)
{
  uint8_t data = std::stoi(val, 0, 0);
  m_header.protocol = data;
  GetField("Protocol")->SetValue(val);
}

void IpPacket::SetChecksum(const char *val)
{
  uint16_t data = std::stoi(val, 0, 0);
  m_header.checksum = BYTE_SWAP_16(data);
  GetField("Checksum")->SetValue(val);
}

void IpPacket::SetSourceIp(const char *val)
{
  uint32_t data = Utils::IPv4ToLong(val);
  m_header.sourceIp = BYTE_SWAP_32(data);
  GetField("Source")->SetValue(val);
}

void IpPacket::SetDestIp(const char *val)
{
  uint32_t data = Utils::IPv4ToLong(val);
  m_header.destIp = BYTE_SWAP_32(data);
  GetField("Destination")->SetValue(val);
}

bool IpPacket::DoesReplyMatch(const uint8_t *buffer, uint32_t size) { return false; }

uint32_t IpPacket::HeaderSize() const { return sizeof(IpHeader); }

void IpPacket::DoWriteToBuf(uint8_t *buffer, uint32_t &offset)
{
  buffer += offset;
  Utils::WriteValue(buffer, m_header);
  buffer += 6;
  Utils::WriteValue(buffer, BYTE_SWAP_16(m_header.flagsOffset));
  offset += HeaderSize();
}

}// namespace PacketHacker
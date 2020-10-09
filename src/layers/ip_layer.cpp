#include "layers/ip_layer.h"
#include "utils/buffer_utils.h"
#include "utils/adapter_utils.h"
#include "utils/layer_utils.h"

namespace PacketHacker {

IpLayer::IpLayer()
  : m_header(), Layer()
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
  // HeaderField *version = HEADER_FIELD_INT8(IpPacket, "Version", 4u, false, SetVersion);
  // HeaderField *headerLength = HEADER_FIELD_INT8(IpPacket, "Header Length", 4u, true, SetHeaderLength);
  // HeaderField *diffServices = HEADER_FIELD_INT8(IpPacket, "Differentiated Services", 0u, true, SetDiffServices);
  // HeaderField *totalLength = HEADER_FIELD_INT16(IpPacket, "Total Length", 20u, false, SetTotalLength);
  // HeaderField *id = HEADER_FIELD_INT16(IpPacket, "Identification", 0u, true, SetId);
  // HeaderField *flags = HEADER_FIELD_INT16(IpPacket, "Flags", 0u, true, SetFlags);
  // HeaderField *fragOffset = HEADER_FIELD_INT16(IpPacket, "Fragment offset", 0u, true, SetFragOffset);
  // HeaderField *ttl = HEADER_FIELD_INT8(IpPacket, "Time to live", 128u, true, SetTtl);
  // HeaderField *protocol = HEADER_FIELD_INT8(IpPacket, "Protocol", 0u, true, SetProtocol);
  // HeaderField *checksum = HEADER_FIELD_INT16(IpPacket, "Header Checksum", 0u, false, SetChecksum);
  // HeaderField *sourceIp = HEADER_FIELD_IPv4(IpPacket, "Source", IPv4Address(), true, SetSourceIp);
  // HeaderField *destIp = HEADER_FIELD_IPv4(IpPacket, "Destination", IPv4Address(), true, SetDestIp);
  // m_fields = { version, headerLength, diffServices, totalLength, id, flags, fragOffset, ttl, protocol, checksum, sourceIp, destIp };
  // Init();
}

IpLayer::IpLayer(const uint8_t *data, uint32_t size)
  : IpLayer()
{
  uint32_t headerSize = sizeof(IpHeader);
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);

  // char buf[8];
  // sprintf(buf, "%d", m_header.version);
  // GetField("Version")->SetValue((uint8_t)m_header.version);
  // sprintf(buf, "%d", m_header.headerLength);
  // GetField("Header Length")->SetValue((uint8_t)m_header.headerLength);
  // sprintf(buf, "0x%02x", m_header.dscpEcn);
  // GetField("Differentiated Services")->SetValue(m_header.dscpEcn);
  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.totalLength));
  // GetField("Total Length")->SetValue(BYTE_SWAP_16(m_header.totalLength));
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.id));
  // GetField("Identification")->SetValue(BYTE_SWAP_16(m_header.id));
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.flagsOffset));
  // GetField("Flags")->SetValue(BYTE_SWAP_16(m_header.flagsOffset));
  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.flagsOffset) & (0x1fff));
  // GetField("Fragment offset")->SetValue(BYTE_SWAP_16(m_header.flagsOffset));
  // sprintf(buf, "%d", m_header.ttl);
  // GetField("Time to live")->SetValue(m_header.ttl);
  // sprintf(buf, "0x%02x", m_header.protocol);
  // GetField("Protocol")->SetValue(m_header.protocol);
  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.checksum));
  // GetField("Header Checksum")->SetValue(BYTE_SWAP_16(m_header.checksum));
  // GetField("Source")->SetValue(IPv4Address(m_header.sourceIp));
  // GetField("Destination")->SetValue(IPv4Address(m_header.destIp));

  size = size - headerSize;
  if (size > 0) {
    innerLayer(Utils::LayerFromType(m_header.protocol, (uint8_t *)(data + headerSize), size));
  }
}

void IpLayer::version(const uint8_t version)
{
  m_header.version = version;
}

void IpLayer::headerLength(const uint8_t headerLength)
{
  m_header.headerLength = headerLength;
}

void IpLayer::diffServices(const uint8_t diffServices)
{
  m_header.diffServices = diffServices;
}

void IpLayer::totalLength(const uint16_t totalLength)
{
  m_header.totalLength = BYTE_SWAP_32(totalLength);
}

void IpLayer::id(const uint16_t id)
{
  m_header.id = BYTE_SWAP_32(id);
}

void IpLayer::flags(const uint16_t flags)
{
  m_header.flags = BYTE_SWAP_16(flags);
}

void IpLayer::fragOffset(const uint16_t fragOffset)
{
  m_header.fragOffset = BYTE_SWAP_16(fragOffset);
}

void IpLayer::ttl(const uint8_t ttl)
{
  m_header.ttl = ttl;
}

void IpLayer::protocol(const uint8_t protocol)
{
  m_header.protocol = protocol;
}

void IpLayer::checksum(const uint16_t checksum)
{
  m_header.checksum = BYTE_SWAP_16(checksum);
}

void IpLayer::sourceIp(const IPv4Address &sourceIp)
{
  m_header.sourceIp = sourceIp.data();
}

void IpLayer::destIp(const IPv4Address &destIp)
{
  m_header.destIp = destIp.data();
}

bool IpLayer::isReply(const uint8_t *buffer, uint32_t size)
{
  uint16_t headerSize = sizeof(IpHeader);
  if (size < headerSize) {
    return false;
  }

  const IpHeader *header = (const IpHeader *)buffer;
  size = size - headerSize;

  if (m_header.destIp == header->sourceIp && m_header.sourceIp == header->destIp && m_header.protocol == header->protocol) {
    return true;
  }

  return false;
}

void IpLayer::write(uint8_t *buffer)
{
  uint32_t size = headerSize();
  Layer *curr = innerLayer();
  while (curr) {
    size += curr->headerSize();
    curr = curr->innerLayer();
  }
  m_header.totalLength = BYTE_SWAP_16(size);
  // GetField("Total Length")->SetValue(size);

  m_header.checksum = 0x0000;
  uint16_t checksum = Utils::CalcChecksum((void *)&m_header, sizeof(m_header));
  m_header.checksum = BYTE_SWAP_16(checksum);
  // char buf[6];
  // sprintf(buf, "0x%04x", checksum);
  // GetField("Header Checksum")->SetValue(checksum);

  Utils::WriteValue(buffer, m_header);
  buffer += 6;
  Utils::WriteValue(buffer, BYTE_SWAP_16(m_header.flagsOffset));
}

}// namespace PacketHacker
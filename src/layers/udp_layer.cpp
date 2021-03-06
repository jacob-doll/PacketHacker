#include "layers/udp_layer.h"
#include "layers/ip_layer.h"
#include "utils/buffer_utils.h"
#include "utils/adapter_utils.h"
#include "constants.h"

namespace PacketHacker {

UdpLayer::UdpLayer()
  : m_header(), Layer()
{
  // HeaderField *srcPort = new HeaderFieldImpl<UdpPacket>(this, "Src Port", 0u, FieldType::FIELD_INT16, &UdpPacket::SetSrcPort);
  // HeaderField *dstPort = new HeaderFieldImpl<UdpPacket>(this, "Dst Port", 0u, FieldType::FIELD_INT16, &UdpPacket::SetDstPort);
  // HeaderField *length = new HeaderFieldImpl<UdpPacket>(this, "Udp Length", 0u, FieldType::FIELD_INT16, &UdpPacket::SetLength, false);
  // HeaderField *checksum = new HeaderFieldImpl<UdpPacket>(this, "Udp Checksum", 0u, FieldType::FIELD_INT16, &UdpPacket::SetChecksum, false);

  // HeaderField *srcPort = HEADER_FIELD_INT16(UdpPacket, "Src Port", 0u, true, SetSrcPort);
  // HeaderField *dstPort = HEADER_FIELD_INT16(UdpPacket, "Dst Port", 0u, true, SetDstPort);
  // HeaderField *length = HEADER_FIELD_INT16(UdpPacket, "Udp Length", 0u, true, SetLength);
  // HeaderField *checksum = HEADER_FIELD_INT16(UdpPacket, "Udp Checksum", 0u, true, SetChecksum);

  // m_fields = { srcPort, dstPort, length, checksum };
  // Init();
}

UdpLayer::UdpLayer(const uint8_t *data, uint32_t size)
  : UdpLayer()
{
  uint32_t headerSize = sizeof(UdpHeader);
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);

  // char buf[8];
  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.srcPort));
  // GetField("Src Port")->SetValue(BYTE_SWAP_16(m_header.srcPort));

  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.dstPort));
  // GetField("Dst Port")->SetValue(BYTE_SWAP_16(m_header.dstPort));

  // sprintf(buf, "%d", BYTE_SWAP_16(m_header.length));
  // GetField("Udp Length")->SetValue(BYTE_SWAP_16(m_header.length));

  // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.checksum));
  // GetField("Udp Checksum")->SetValue(BYTE_SWAP_16(m_header.checksum));
}

void UdpLayer::srcPort(const uint16_t srcPort)
{
  m_header.srcPort = BYTE_SWAP_16(srcPort);
}

void UdpLayer::dstPort(const uint16_t dstPort)
{
  m_header.dstPort = BYTE_SWAP_16(dstPort);
}

void UdpLayer::length(const uint16_t length)
{
  m_header.length = BYTE_SWAP_16(length);
}

void UdpLayer::checksum(const uint16_t checksum)
{
  m_header.checksum = BYTE_SWAP_16(checksum);
}

bool UdpLayer::isReply(const DataType *buffer, SizeType size)
{
  uint16_t headerSize = sizeof(UdpHeader);
  if (size < headerSize) {
    return false;
  }

  const UdpHeader *header = (const UdpHeader *)buffer;
  size = size - headerSize;

  if (m_header.dstPort == header->srcPort && m_header.srcPort == header->dstPort) {
    return true;
  }

  return false;
}

void UdpLayer::write(DataType *buffer)
{
  uint32_t size = headerSize();
  Layer *curr = innerLayer();
  while (curr) {
    size += curr->headerSize();
    curr = curr->innerLayer();
  }
  m_header.checksum = 0x0000;
  m_header.length = BYTE_SWAP_16(size);
  // GetField("Udp Length")->SetValue(size);

  Utils::WriteValue(buffer, m_header);


  std::vector<uint8_t> psuedo_header(size + 12);
  if (outerLayer()->type() == LayerType::IP) {
    IpLayer *ip = static_cast<IpLayer *>(outerLayer());
    Utils::WriteValue(&psuedo_header[0], ip->sourceIp());
    Utils::WriteValue(&psuedo_header[4], ip->destIp());
    Utils::WriteValue(&psuedo_header[8], (uint8_t)0);
    Utils::WriteValue(&psuedo_header[9], (uint8_t)Constants::IP::TYPE_UDP);
    Utils::WriteValue(&psuedo_header[10], (uint16_t)m_header.length);
  }


  Utils::Write(&psuedo_header[12], buffer, size);

  uint16_t checksum = Utils::CalcChecksum((void *)&psuedo_header[0], size + 12);
  // char buf[6];
  // sprintf(buf, "0x%04x", checksum);
  // GetField("Udp Checksum")->SetValue(checksum);

  Utils::WriteValue((uint8_t *)(buffer + 6), BYTE_SWAP_16(checksum));
}

}// namespace PacketHacker
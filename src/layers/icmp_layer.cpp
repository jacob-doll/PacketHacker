#include "layers/icmp_layer.h"
#include "utils/buffer_utils.h"
#include "utils/adapter_utils.h"

namespace PacketHacker {

IcmpLayer::IcmpLayer()
  : m_header(),
    m_originateTimestamp(0),
    m_receiveTimestamp(0),
    m_transmitTimestamp(0),
    m_addressMask(0),
    Layer()
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

IcmpLayer::IcmpLayer(const uint8_t *data, uint32_t size)
  : IcmpLayer()
{
  uint32_t headerSize = sizeof(IcmpHeader);
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);
  data += sizeof(IcmpHeader);

  size = size - headerSize;
  if (icmpType() == TIMESTAMP || icmpType() == TIMESTAMP_REPLY) {
    if (size < 3 * sizeof(uint32_t)) {
      return;
    }
    Utils::ReadValue(data, m_originateTimestamp);
    Utils::ReadValue(data + sizeof(uint32_t), m_receiveTimestamp);
    Utils::ReadValue(data + 2 * sizeof(uint32_t), m_transmitTimestamp);
  } else if (icmpType() == ADDRESS_MASK_REQUEST || icmpType() == ADDRESS_MASK_REPLY) {
    if (size < sizeof(uint32_t)) {
      return;
    }
    Utils::ReadValue(data, m_addressMask);
  }
}

void IcmpLayer::icmpType(const uint8_t icmpType)
{
  m_header.icmpType = icmpType;
}

void IcmpLayer::code(const uint8_t code)
{
  m_header.code = code;
}

void IcmpLayer::checksum(const uint16_t checksum)
{
  m_header.checksum = BYTE_SWAP_16(checksum);
}

void IcmpLayer::id(const uint16_t id)
{
  m_header.data.id = BYTE_SWAP_16(id);
}

void IcmpLayer::sequence(const uint16_t sequence)
{
  m_header.data.sequence = BYTE_SWAP_16(sequence);
}

void IcmpLayer::nextHopMtu(const uint16_t nextHopMtu)
{
  m_header.data.nextHopMtu = BYTE_SWAP_16(nextHopMtu);
}

void IcmpLayer::gateway(const IPv4Address &gateway)
{
  m_header.data.gateway = gateway.data();
}

void IcmpLayer::originateTimestamp(const uint32_t originateTimestamp)
{
  m_originateTimestamp = BYTE_SWAP_32(originateTimestamp);
}

void IcmpLayer::receiveTimestamp(const uint32_t receiveTimestamp)
{
  m_receiveTimestamp = BYTE_SWAP_32(receiveTimestamp);
}

void IcmpLayer::transmitTimestamp(const uint32_t transmitTimestamp)
{
  m_transmitTimestamp = BYTE_SWAP_32(transmitTimestamp);
}

void IcmpLayer::addressMask(const IPv4Address &addressMask)
{
  m_addressMask = addressMask.data();
}

const SizeType IcmpLayer::headerSize() const
{
  if (m_header.icmpType == TIMESTAMP || m_header.icmpType == TIMESTAMP_REPLY) {
    return sizeof(IcmpHeader) + 3 * sizeof(uint32_t);
  } else if (m_header.icmpType == ADDRESS_MASK_REQUEST || m_header.icmpType == ADDRESS_MASK_REPLY) {
    return sizeof(IcmpHeader) + sizeof(uint32_t);
  }
  return sizeof(IcmpHeader);
}

bool IcmpLayer::isReply(const DataType *buffer, SizeType size) { return true; }

void IcmpLayer::write(DataType *buffer)
{
  m_header.checksum = 0x0000;
  Utils::WriteValue(buffer, m_header);

  if (icmpType() == TIMESTAMP || icmpType() == TIMESTAMP_REPLY) {
    Utils::WriteValue(buffer + sizeof(m_header), m_originateTimestamp);
    Utils::WriteValue(buffer + sizeof(m_header) + sizeof(uint32_t), m_receiveTimestamp);
    Utils::WriteValue(buffer + sizeof(m_header) + 2 * sizeof(uint32_t), m_transmitTimestamp);
  } else if (icmpType() == ADDRESS_MASK_REQUEST || icmpType() == ADDRESS_MASK_REPLY) {
    Utils::WriteValue(buffer + sizeof(m_header), m_addressMask);
  }

  uint32_t size = headerSize();
  Layer *curr = innerLayer();
  while (curr) {
    size += curr->headerSize();
    curr = curr->innerLayer();
  }

  uint16_t checksum = Utils::CalcChecksum((void *)buffer, size);

  Utils::WriteValue((uint8_t *)(buffer + 2), BYTE_SWAP_16(checksum));
}

}// namespace PacketHacker
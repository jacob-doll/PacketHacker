#include "packets/icmp_packet.h"
#include "utils/buffer_utils.h"
#include "utils/adapter_utils.h"
#include "packets/data_packet.h"

namespace PacketHacker {

IcmpPacket::IcmpPacket()
  : m_header(),
    m_originateTimestamp(0),
    m_receiveTimestamp(0),
    m_transmitTimestamp(0),
    m_addressMask(0),
    Packet()
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
  uint32_t headerSize = sizeof(IcmpHeader);
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);
  data += sizeof(IcmpHeader);

  size = size - headerSize;
  if (type() == TIMESTAMP || type() == TIMESTAMP_REPLY) {
    if (size < 3 * sizeof(uint32_t)) {
      return;
    }
    Utils::ReadValue(data, m_originateTimestamp);
    Utils::ReadValue(data + sizeof(uint32_t), m_receiveTimestamp);
    Utils::ReadValue(data + 2 * sizeof(uint32_t), m_transmitTimestamp);
  } else if (type() == ADDRESS_MASK_REQUEST || type() == ADDRESS_MASK_REPLY) {
    if (size < sizeof(uint32_t)) {
      return;
    }
    Utils::ReadValue(data, m_addressMask);
  }

  if (size > 0) {
    innerPacket(new DataPacket((uint8_t *)(data + headerSize), size));
  }
}

void IcmpPacket::type(const uint8_t type)
{
  m_header.type = type;
}

void IcmpPacket::code(const uint8_t code)
{
  m_header.code = code;
}

void IcmpPacket::checksum(const uint16_t checksum)
{
  m_header.checksum = BYTE_SWAP_16(checksum);
}

void IcmpPacket::id(const uint16_t id)
{
  m_header.data.id = BYTE_SWAP_16(id);
}

void IcmpPacket::sequence(const uint16_t sequence)
{
  m_header.data.sequence = BYTE_SWAP_16(sequence);
}

void IcmpPacket::nextHopMtu(const uint16_t nextHopMtu)
{
  m_header.data.nextHopMtu = BYTE_SWAP_16(nextHopMtu);
}

void IcmpPacket::gateway(const IPv4Address &gateway)
{
  m_header.data.gateway = gateway.data();
}

void IcmpPacket::originateTimestamp(const uint32_t originateTimestamp)
{
  m_originateTimestamp = BYTE_SWAP_32(originateTimestamp);
}

void IcmpPacket::receiveTimestamp(const uint32_t receiveTimestamp)
{
  m_receiveTimestamp = BYTE_SWAP_32(receiveTimestamp);
}

void IcmpPacket::transmitTimestamp(const uint32_t transmitTimestamp)
{
  m_transmitTimestamp = BYTE_SWAP_32(transmitTimestamp);
}

void IcmpPacket::addressMask(const IPv4Address &addressMask)
{
  m_addressMask = addressMask.data();
}

const uint32_t IcmpPacket::headerSize() const
{
  if (m_header.type == TIMESTAMP || m_header.type == TIMESTAMP_REPLY) {
    return sizeof(IcmpHeader) + 3 * sizeof(uint32_t);
  } else if (m_header.type == ADDRESS_MASK_REQUEST || m_header.type == ADDRESS_MASK_REPLY) {
    return sizeof(IcmpHeader) + sizeof(uint32_t);
  }
  return sizeof(IcmpHeader);
}

bool IcmpPacket::doesReplyMatch(const uint8_t *buffer, uint32_t size) { return true; }

void IcmpPacket::doWriteToBuf(uint8_t *buffer)
{
  m_header.checksum = 0x0000;
  Utils::WriteValue(buffer, m_header);

  if (type() == TIMESTAMP || type() == TIMESTAMP_REPLY) {
    Utils::WriteValue(buffer + sizeof(m_header), m_originateTimestamp);
    Utils::WriteValue(buffer + sizeof(m_header) + sizeof(uint32_t), m_receiveTimestamp);
    Utils::WriteValue(buffer + sizeof(m_header) + 2 * sizeof(uint32_t), m_transmitTimestamp);
  } else if (type() == ADDRESS_MASK_REQUEST || type() == ADDRESS_MASK_REPLY) {
    Utils::WriteValue(buffer + sizeof(m_header), m_addressMask);
  }

  uint16_t checksum = Utils::CalcChecksum((void *)buffer, size());

  Utils::WriteValue((uint8_t *)(buffer + 2), BYTE_SWAP_16(checksum));
}

}// namespace PacketHacker
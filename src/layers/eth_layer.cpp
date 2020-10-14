#include "layers/eth_layer.h"
#include "utils/buffer_utils.h"
#include "utils/adapter_utils.h"
#include "utils/layer_utils.h"

namespace PacketHacker {

EthernetLayer::EthernetLayer()
  : m_header(), Layer()
{
  // HeaderField *dst = new HeaderFieldImpl<EthernetPacket>(this, "Dst", "00:00:00:00:00:00", &EthernetPacket::SetDst, true, FieldType::FIELD_HARDWARE);
  // HeaderField *src = new HeaderFieldImpl<EthernetPacket>(this, "Src", "00:00:00:00:00:00", &EthernetPacket::SetSrc, true, FieldType::FIELD_HARDWARE);
  // HeaderField *type = new HeaderFieldImpl<EthernetPacket>(this, "EtherType", "0x0800", &EthernetPacket::SetType);

  // HeaderField *dst = HEADER_FIELD_HARDWARE(EthernetPacket, "Dst", HardwareAddress(), true, SetDst);
  // HeaderField *src = HEADER_FIELD_HARDWARE(EthernetPacket, "Src", HardwareAddress(), true, SetSrc);
  // HeaderField *type = HEADER_FIELD_INT16(EthernetPacket, "EtherType", 0x0800, true, SetType);

  // m_fields = { dst, src, type };
  // Init();
}

EthernetLayer::EthernetLayer(const uint8_t *data, uint32_t size)
  : EthernetLayer()
{
  uint32_t headerSize = sizeof(EthernetHeader);
  if (size < headerSize) {
    return;
  }
  Utils::ReadValue(data, m_header);

  // GetField("Dst")->SetValue(HardwareAddress(m_header.dstMac));
  // GetField("Src")->SetValue(HardwareAddress(m_header.srcMac));
  // // char buf[8];
  // // sprintf(buf, "0x%04x", BYTE_SWAP_16(m_header.type));
  // GetField("EtherType")->SetValue(BYTE_SWAP_16(m_header.type));

  size = size - headerSize;
  if (size > 0) {
    innerLayer(Utils::LayerFromType(BYTE_SWAP_16(m_header.protoType), (uint8_t *)(data + headerSize), size));
  }
}

void EthernetLayer::dstMac(const HardwareAddress &dst)
{
  Utils::Write(m_header.dstMac, dst.ptr(), PHYSICAL_ADDR_LEN);
}

void EthernetLayer::srcMac(const HardwareAddress &src)
{
  Utils::Write(m_header.srcMac, src.ptr(), PHYSICAL_ADDR_LEN);
}

void EthernetLayer::protoType(const uint16_t protoType)
{
  m_header.protoType = BYTE_SWAP_16(protoType);
}

bool EthernetLayer::isReply(const DataType *buffer, SizeType size)
{
  uint32_t headerSize = sizeof(EthernetHeader);
  if (size < headerSize) {
    return false;
  }

  const EthernetHeader *header = (const EthernetHeader *)buffer;
  size = size - headerSize;
  if (Utils::BufferEquals<6>(m_header.srcMac, header->dstMac)) {
    return true;
  }

  return false;
}

void EthernetLayer::write(DataType *buffer)
{
  Utils::WriteValue(buffer, m_header);
}

}// namespace PacketHacker
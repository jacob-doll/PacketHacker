#include "packet_eth.h"

#include <string.h>

#include "utils/buffer_utils.h"

namespace PacketHacker
{

EthernetPacket::EthernetPacket()
{
}

size_t EthernetPacket::HeaderSize() const
{
    return sizeof(EthernetHeader);
}

void EthernetPacket::SetDstMac(uint8_t *dstMac)
{
    memcpy(m_header.dstMac, dstMac, PHYS_ADDR_SIZE);
}

void EthernetPacket::SetSrcMac(uint8_t *srcMac)
{
    memcpy(m_header.srcMac, srcMac, PHYS_ADDR_SIZE);
}

void EthernetPacket::SetType(uint16_t type)
{
    m_header.type = type;
}

void EthernetPacket::DoWriteToBuf(uint8_t *buffer, uint32_t &offset)
{
    Utils::CopyToBuffer(buffer, m_header.dstMac, offset, PHYS_ADDR_SIZE);
    Utils::CopyToBuffer(buffer, m_header.srcMac, offset, PHYS_ADDR_SIZE);
    Utils::WriteToBuffer(buffer, offset, m_header.type);
}

} // namespace PacketHacker

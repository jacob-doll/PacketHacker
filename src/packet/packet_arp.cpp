#include "packet_arp.h"
#include <string.h>
#include "utils/buffer_utils.h"

namespace PacketHacker
{

ArpPacket::ArpPacket()
{
}

uint32_t ArpPacket::HeaderSize() const
{
    return sizeof(ArpHeader);
}

void ArpPacket::SetHardwareType(uint16_t hardwareType)
{
    m_header.hardwareType = hardwareType;
}

void ArpPacket::SetProtocolType(uint16_t protocolType)
{
    m_header.protocolType = protocolType;
}

void ArpPacket::SetHardwareSize(uint8_t hardwareSize)
{
    m_header.hardwareSize = hardwareSize;
}

void ArpPacket::SetProtocolSize(uint8_t protocolSize)
{
    m_header.protocolSize = protocolSize;
}

void ArpPacket::SetOpcode(uint16_t opcode)
{
    m_header.opcode = opcode;
}

void ArpPacket::SetSenderMac(uint8_t *senderMac)
{
    memcpy(m_header.senderMac, senderMac, PHYS_ADDR_SIZE);
}

void ArpPacket::SetSenderIp(uint8_t *senderIp)
{
    memcpy(m_header.senderIp, senderIp, IPV4_ADDR_SIZE);
}

void ArpPacket::SetTargetMac(uint8_t *targetMac)
{
    memcpy(m_header.targetMac, targetMac, PHYS_ADDR_SIZE);
}

void ArpPacket::SetTargetIp(uint8_t *targetIp)
{
    memcpy(m_header.targetIp, targetIp, IPV4_ADDR_SIZE);
}

void ArpPacket::DoWriteToBuf(uint8_t *buffer, uint32_t &offset)
{
    Utils::WriteToBuffer(buffer, offset, m_header.hardwareType);
    Utils::WriteToBuffer(buffer, offset, m_header.protocolType);
    Utils::WriteToBuffer(buffer, offset, m_header.hardwareSize);
    Utils::WriteToBuffer(buffer, offset, m_header.protocolSize);
    Utils::WriteToBuffer(buffer, offset, m_header.opcode);
    Utils::CopyToBuffer(buffer, m_header.senderMac, offset, PHYS_ADDR_SIZE);
    Utils::CopyToBuffer(buffer, m_header.senderIp, offset, IPV4_ADDR_SIZE);
    Utils::CopyToBuffer(buffer, m_header.targetMac, offset, PHYS_ADDR_SIZE);
    Utils::CopyToBuffer(buffer, m_header.targetIp, offset, IPV4_ADDR_SIZE);
}

} // namespace PacketHacker
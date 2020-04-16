#include "packet.h"

namespace PacketHacker
{

Packet::Packet()
    : m_innerPacket(), m_outerPacket()
{
}

Packet::~Packet()
{
    delete m_innerPacket;
}

size_t Packet::Size() const
{
    uint32_t size = HeaderSize() + PayloadSize();
    if (m_innerPacket)
    {
        size += m_innerPacket->Size();
    }
    return size;
}

void Packet::SetInnerPacket(Packet *inner)
{
    delete m_innerPacket;
    m_innerPacket = inner;
    if (m_innerPacket)
    {
        m_innerPacket->SetOuterPacket(this);
    }
}

void Packet::SetOuterPacket(Packet *outer)
{
    m_outerPacket = outer;
}

void Packet::WriteToBuf(uint8_t *buffer, uint32_t size, uint32_t offset)
{
    uint32_t packetSize = this->Size();
    if (size < packetSize)
        return;
    uint32_t currentOffset = offset;
    DoWriteToBuf(buffer, currentOffset);
    if (m_innerPacket)
        m_innerPacket->WriteToBuf(buffer, packetSize, currentOffset);
}

} // namespace PacketHacker
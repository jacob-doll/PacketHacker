#pragma once
#include <stdint.h>

namespace PacketHacker
{

class Packet
{
public:
    Packet();
    virtual ~Packet();

    virtual uint32_t HeaderSize() const = 0;
    virtual uint32_t PayloadSize() const { return 0; }
    uint32_t Size() const;

    Packet *GetOuterPacket() const { return m_outerPacket; }
    Packet *GetInnerPacket() const { return m_innerPacket; }
    void SetInnerPacket(Packet *inner);

    void WriteToBuf(uint8_t *buffer, uint32_t size, uint32_t offset = 0);
    virtual void DoWriteToBuf(uint8_t *buffer, uint32_t &offset) = 0;

private:
    void SetOuterPacket(Packet *outer);
    Packet *m_outerPacket;
    Packet *m_innerPacket;
};

} // namespace PacketHacker

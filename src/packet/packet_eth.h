#pragma once

#include "packet.h"
#include "addresses.h"

namespace PacketHacker
{

struct EthernetHeader
{
    uint8_t dstMac[PHYS_ADDR_SIZE];
    uint8_t srcMac[PHYS_ADDR_SIZE];
    uint16_t type;
};

class EthernetPacket : public Packet
{
public:
    EthernetPacket();

    uint32_t HeaderSize() const override;

    void SetDstMac(uint8_t *dstMac);
    void SetSrcMac(uint8_t *srcMac);
    void SetType(uint16_t type);

    void DoWriteToBuf(uint8_t *buffer, uint32_t &offset) override;

private:
    EthernetHeader m_header;
};

} // namespace PacketHacker
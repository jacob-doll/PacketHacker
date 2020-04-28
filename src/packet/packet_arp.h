#pragma once

#include "packet.h"
#include "addresses.h"

namespace PacketHacker
{

struct ArpHeader
{
    uint16_t hardwareType;
    uint16_t protocolType;
    uint8_t hardwareSize;
    uint8_t protocolSize;
    uint16_t opcode;
    uint8_t senderMac[PHYS_ADDR_SIZE];
    uint8_t senderIp[IPV4_ADDR_SIZE];
    uint8_t targetMac[PHYS_ADDR_SIZE];
    uint8_t targetIp[IPV4_ADDR_SIZE];
};

class ArpPacket : public Packet
{
public:
    ArpPacket();

    uint32_t HeaderSize() const override;

    void SetHardwareType(uint16_t hardwareType);
    void SetProtocolType(uint16_t protocolType);
    void SetHardwareSize(uint8_t hardwareSize);
    void SetProtocolSize(uint8_t protocolSize);
    void SetOpcode(uint16_t opcode);
    void SetSenderMac(uint8_t *senderMac);
    void SetSenderIp(uint8_t *senderIp);
    void SetTargetMac(uint8_t *targetMac);
    void SetTargetIp(uint8_t *targetIp);

    void DoWriteToBuf(uint8_t *buffer, uint32_t &offset) override;

    std::string GetName() const override { return "Arp"; }
    std::vector<std::string> GetFields() const override { return m_fields; }

private:
    const std::vector<std::string> m_fields = {"Hardware Type",
                                               "Protocol Type",
                                               "Hardware Address Length",
                                               "Protocol Address Size",
                                               "Operation Code",
                                               "Sender MAC",
                                               "Sender IP",
                                               "Target MAC",
                                               "Target IP"};
    ArpHeader m_header;
};

} // namespace PacketHacker
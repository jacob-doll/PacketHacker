#pragma once

#include "packet.h"
#include "addresses.h"

namespace PacketHacker
{

class DataPacket : public Packet
{
public:
    DataPacket();
    ~DataPacket();

    uint32_t HeaderSize() const override;

    void DoWriteToBuf(uint8_t *buffer, uint32_t &offset) override;

    void SetData(uint8_t *m_data, uint32_t size);
    void FillEmpty(uint32_t size);

    std::string GetName() const override { return "Data"; }
    std::vector<std::string> GetFields() const override { return std::vector<std::string>(); }

private:
    uint8_t *m_data;
    uint32_t m_size;
};

} //namespace PacketHacker
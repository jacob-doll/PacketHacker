#include "packet_data.h"

namespace PacketHacker
{

DataPacket::DataPacket() {}

DataPacket::~DataPacket() {}

uint32_t DataPacket::HeaderSize() const
{
    return m_size;
}

void DataPacket::DoWriteToBuf(uint8_t *buffer, uint32_t &offset) {}

void DataPacket::SetData(uint8_t *m_data, uint32_t size) {}

void DataPacket::FillEmpty(uint32_t size) {}

} // namespace PacketHacker

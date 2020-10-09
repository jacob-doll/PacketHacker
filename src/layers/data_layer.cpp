#include "layers/data_layer.h"
#include "utils/buffer_utils.h"

namespace PacketHacker {

DataLayer::DataLayer()
  : m_data(), m_size(0), Layer()
{
  // HeaderField *data = new HeaderFieldImpl<DataPacket>(this, "Packet Data", "", &DataPacket::SetData);
  // m_fields.emplace_back(data);
}

DataLayer::DataLayer(const uint8_t *data, uint32_t size)
  : m_data(data, data + size), m_size(size)
{
  // HeaderField *data_field = new HeaderFieldImpl<DataPacket>(this, "Packet Data", "", &DataPacket::SetData);
  // m_fields.emplace_back(data_field);

  // std::string str((const char *)data, size);
  // data_field->SetValue(str.c_str());
}

void DataLayer::write(uint8_t *buffer)
{
  // Utils::Write(buffer, m_data.data(), m_size);
}

}// namespace PacketHacker
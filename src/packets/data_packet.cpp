#include "packets/data_packet.h"
#include "utils/buffer_utils.h"

namespace PacketHacker {

DataPacket::DataPacket()
  : m_data(), m_size(0), Packet()
{
  // HeaderField *data = new HeaderFieldImpl<DataPacket>(this, "Packet Data", "", &DataPacket::SetData);
  // m_fields.emplace_back(data);
}

DataPacket::DataPacket(const uint8_t *data, uint32_t size)
  : m_data(data, data + size), m_size(size)
{
  // HeaderField *data_field = new HeaderFieldImpl<DataPacket>(this, "Packet Data", "", &DataPacket::SetData);
  // m_fields.emplace_back(data_field);

  // std::string str((const char *)data, size);
  // data_field->SetValue(str.c_str());
}

void DataPacket::SetData(const FieldData &val)
{
  // m_size = strlen(val);
  // m_data.clear();
  // m_data.resize(m_size);

  // for (int i = 0; i < m_size; i++) {
  //   m_data[i] = (uint8_t)val[i];
  // }

  // GetField("Packet Data")->SetValue(val);
}

void DataPacket::DoWriteToBuf(uint8_t *buffer)
{
  // Utils::Write(buffer, m_data.data(), m_size);
}


}// namespace PacketHacker
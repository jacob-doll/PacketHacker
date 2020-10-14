#include "payload.h"
#include "utils/buffer_utils.h"

namespace PacketHacker {

// Raw Payload
RawPayload::RawPayload(const DataType *data, SizeType size)
  : m_payload(size)
{
  Utils::Write(m_payload.data(), data, size);
}

const SizeType RawPayload::payloadSize() const
{
  return m_payload.size();
}

void RawPayload::write(DataType *buffer)
{
  Utils::Write(buffer, m_payload.data(), m_payload.size());
}


// String Payload
StringPayload::StringPayload(const std::string payload)
  : m_payload(std::move(payload))
{
}

const SizeType StringPayload::payloadSize() const
{
  return m_payload.length();
}

void StringPayload::write(DataType *buffer)
{
  Utils::Write(buffer, (uint8_t *)m_payload.data(), m_payload.length());
}


}// namespace PacketHacker
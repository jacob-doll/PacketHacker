#include "packet.h"

#include <sstream>

namespace PacketHacker {

Packet::Packet()
  : m_innerPacket(), m_outerPacket()
{
}

Packet::~Packet()
{
  for (HeaderField *field : m_fields) {
    delete field;
  }
  delete m_innerPacket;
}

uint32_t Packet::Size() const
{
  uint32_t size = HeaderSize();
  if (m_innerPacket) {
    size += m_innerPacket->Size();
  }
  return size;
}

void Packet::Init()
{
  for (HeaderField *field : m_fields) {
    field->HandleData(field->GetDefaultVal().c_str());
  }
}

Packet *Packet::GetPacket(std::string name)
{
  Packet *packet = this;
  while (packet) {
    if (packet->GetName() == name) {
      return packet;
    }
    packet = packet->GetInnerPacket();
  }
  return nullptr;
}

void Packet::SetInnerPacket(Packet *inner)
{
  delete m_innerPacket;
  m_innerPacket = inner;
  if (m_innerPacket) {
    m_innerPacket->SetOuterPacket(this);
  }
}

void Packet::RemoveInnerPacket()
{
  delete m_innerPacket;
  m_innerPacket = nullptr;
}

void Packet::SetOuterPacket(Packet *outer)
{
  m_outerPacket = outer;
}

HeaderField *Packet::GetField(std::string name) const
{
  for (HeaderField *field : m_fields) {
    if (field->GetName() == name) {
      return field;
    }
  }
  return nullptr;
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

std::string Packet::ToString() const
{
  std::stringstream ss;
  ss << GetName() << ": " << HeaderSize() << " {\n";
  for (HeaderField *field : GetFields()) {
    ss << "\t" << field->GetName() << ": " << field->GetCurrentVal() << "\n";
  }
  if (m_innerPacket) {
    ss << m_innerPacket->ToString();
  }
  ss << "}";
  return ss.str();
}


}// namespace PacketHacker
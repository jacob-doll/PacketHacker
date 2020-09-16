#include "packet.h"

#include <sstream>

namespace PacketHacker {

Packet::Packet()
  : m_innerPacket(), m_outerPacket()
{
}

Packet::~Packet()
{
  delete m_innerPacket;
}

const uint32_t Packet::Size() const
{
  uint32_t size = HeaderSize();
  if (m_innerPacket) {
    size += m_innerPacket->Size();
  }
  return size;
}

Packet *Packet::GetPacket(const std::string &name)
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

void Packet::WriteToBuf(uint8_t *buffer, const uint32_t size)
{
  uint32_t packetSize = this->Size();
  if (size < packetSize)
    return;
  uint32_t offset = HeaderSize();
  if (m_innerPacket)
    m_innerPacket->WriteToBuf((buffer + offset), packetSize);
  DoWriteToBuf(buffer);
}

std::ostream &operator<<(std::ostream &output, Packet *packet)
{
  Packet *curr = packet;
  while (curr != nullptr) {
    output << curr->GetName();
    if (curr->GetInnerPacket()) output << "/";
    curr = curr->GetInnerPacket();
  }
  return output;
}

}// namespace PacketHacker
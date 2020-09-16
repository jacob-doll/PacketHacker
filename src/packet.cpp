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

const uint32_t Packet::size() const
{
  uint32_t size = headerSize();
  if (m_innerPacket) {
    size += m_innerPacket->size();
  }
  return size;
}

Packet *Packet::getPacket(const std::string &name)
{
  Packet *packet = this;
  while (packet) {
    if (packet->name() == name) {
      return packet;
    }
    packet = packet->innerPacket();
  }
  return nullptr;
}

void Packet::innerPacket(Packet *inner)
{
  delete m_innerPacket;
  m_innerPacket = inner;
  if (m_innerPacket) {
    m_innerPacket->outerPacket(this);
  }
}

void Packet::removeInnerPacket()
{
  delete m_innerPacket;
  m_innerPacket = nullptr;
}

void Packet::outerPacket(Packet *outer)
{
  m_outerPacket = outer;
}

void Packet::writeToBuf(uint8_t *buffer, const uint32_t size)
{
  uint32_t packetSize = this->size();
  if (size < packetSize)
    return;
  uint32_t offset = headerSize();
  if (m_innerPacket)
    m_innerPacket->writeToBuf((buffer + offset), packetSize);
  doWriteToBuf(buffer);
}

std::ostream &operator<<(std::ostream &output, Packet *packet)
{
  Packet *curr = packet;
  while (curr != nullptr) {
    output << curr->name();
    if (curr->innerPacket()) output << "/";
    curr = curr->innerPacket();
  }
  return output;
}

}// namespace PacketHacker
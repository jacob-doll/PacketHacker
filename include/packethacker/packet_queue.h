#pragma once

#include "packet.h"

#include <queue>

namespace PacketHacker {

class PacketQueue
{
public:
  PacketQueue(const uint16_t capacity);

  bool IsEmpty() const;
  std::size_t Size() const;
  Packet *GetPacket();
  void InsertPacket(Packet *packet);
  const uint16_t Capacity() const { return m_capacity; }

private:
  std::queue<Packet *> m_packetQueue;
  const uint16_t m_capacity;
};

}// namespace PacketHacker
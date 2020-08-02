#pragma once

#include "packet.h"

#include <queue>

namespace PacketHacker {

class PacketQueue
{
public:
  PacketQueue();

private:
  std::queue<Packet *> m_sendQueue;
};

}// namespace PacketHacker
#include "packet_queue.h"
namespace PacketHacker {

// PacketQueue::PacketQueue(const uint16_t capacity)
//   : m_capacity(capacity)
// {
// }

// bool PacketQueue::empty() const
// {
//   return m_packetQueue.empty();
// }

// std::size_t PacketQueue::size() const
// {
//   return m_packetQueue.size();
// }

// Packet *PacketQueue::getPacket()
// {
//   if (m_packetQueue.empty()) return nullptr;
//   Packet *packet = m_packetQueue.front();
//   m_packetQueue.pop();
//   return packet;
// }

// void PacketQueue::insertPacket(Packet *packet)
// {
//   if (m_packetQueue.size() <= m_capacity) {
//     m_packetQueue.push(packet);
//   }
// }

}// namespace PacketHacker
#include "packet_utils.h"

namespace PacketHacker {
namespace Utils {

  Packet *PacketFromId(int packetId)
  {
    switch (packetId) {
    case PacketType::ARP:
      return new ArpPacket();
    case PacketType::ETHERNET:
      return new EthernetPacket();
    case PacketType::IP:
      return new IpPacket();
    case PacketType::ICMP:
      return new IcmpPacket();
    case PacketType::DATA:
      return new DataPacket();
    case PacketType::UDP:
      return new UdpPacket();
    default:
      return nullptr;
    }
  }

  Packet *PacketFromType(uint32_t type, const uint8_t *buffer, uint32_t size)
  {
    switch (type) {
    case TYPE_ARP:
      return new ArpPacket(buffer, size);
    case TYPE_IPv4:
      return new IpPacket(buffer, size);
    case TYPE_ICMP:
      return new IcmpPacket(buffer, size);
    case TYPE_UDP:
      return new UdpPacket(buffer, size);
    default:
      break;
    }
    return nullptr;
  }

}// namespace Utils
}// namespace PacketHacker
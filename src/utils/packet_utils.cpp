#include "utils/packet_utils.h"

namespace PacketHacker {
namespace Utils {

  Packet *PacketFromId(const uint32_t packetId)
  {
    switch (packetId) {
    case Packet::ARP:
      return new ArpPacket();
    case Packet::ETHERNET:
      return new EthernetPacket();
    case Packet::IP:
      return new IpPacket();
    case Packet::ICMP:
      return new IcmpPacket();
    case Packet::DATA:
      return new DataPacket();
    case Packet::UDP:
      return new UdpPacket();
    default:
      return nullptr;
    }
  }

  Packet *PacketFromType(const uint32_t type, const uint8_t *buffer, const uint32_t size)
  {
    switch (type) {
    case Constants::Ethernet::TYPE_ARP:
      return new ArpPacket(buffer, size);
    case Constants::Ethernet::TYPE_IPv4:
      return new IpPacket(buffer, size);
    case Constants::IP::TYPE_ICMP:
      return new IcmpPacket(buffer, size);
    case Constants::IP::TYPE_UDP:
      return new UdpPacket(buffer, size);
    default:
      break;
    }
    return nullptr;
  }

}// namespace Utils
}// namespace PacketHacker
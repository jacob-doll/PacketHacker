#include "packet_utils.h"

namespace PacketHacker {
namespace Utils {

  Packet *PacketFromType(uint32_t type, const uint8_t *buffer, uint32_t size)
  {
    switch (type) {
    case TYPE_ARP:
      return new ArpPacket(buffer, size);
    case TYPE_IPv4:
      return new IpPacket(buffer, size);
    default:
      break;
    }
    return nullptr;
  }

}// namespace Utils
}// namespace PacketHacker
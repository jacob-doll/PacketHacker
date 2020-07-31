#pragma once

#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/constants.h"

namespace PacketHacker {
namespace Utils {

  Packet *PacketFromId(const uint32_t packetId);
  Packet *PacketFromType(const uint32_t type, const uint8_t *buffer, const uint32_t size);

}// namespace Utils
}// namespace PacketHacker
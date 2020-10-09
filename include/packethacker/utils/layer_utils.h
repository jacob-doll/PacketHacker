#pragma once

#include "packet.h"
#include "constants.h"

namespace PacketHacker {
namespace Utils {

  Layer *LayerFromId(const uint32_t packetId);
  Layer *LayerFromType(const uint32_t type, const uint8_t *buffer, const uint32_t size);

}// namespace Utils
}// namespace PacketHacker
#pragma once

#include <string>

namespace PacketHacker {
namespace Utils {

  uint16_t CalcChecksum(void *vdata, uint32_t size);

}// namespace Utils
}// namespace PacketHacker
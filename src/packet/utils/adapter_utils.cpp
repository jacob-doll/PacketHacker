#include "adapter_utils.h"

#ifdef _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#endif

namespace PacketHacker {
namespace Utils {

  uint16_t CalcChecksum(void *vdata, uint32_t size)
  {
    uint8_t *data = (uint8_t *)vdata;

    // Initialise the accumulator.
    uint64_t acc = 0xffff;

    // Handle any partial block at the start of the data.
    unsigned int offset = ((uintptr_t)data) & 3;
    if (offset) {
      size_t count = 4 - offset;
      if (count > size) count = size;
      uint32_t word = 0;
      memcpy(offset + (char *)&word, data, count);
      acc += ntohl(word);
      data += count;
      size -= count;
    }

    // Handle any complete 32-bit blocks.
    uint8_t *data_end = data + (size & ~3);
    while (data != data_end) {
      uint32_t word;
      memcpy(&word, data, 4);
      acc += ntohl(word);
      data += 4;
    }
    size &= 3;

    // Handle any partial block at the end of the data.
    if (size) {
      uint32_t word = 0;
      memcpy(&word, data, size);
      acc += ntohl(word);
    }

    // Handle deferred carries.
    acc = (acc & 0xffffffff) + (acc >> 32);
    while (acc >> 16) {
      acc = (acc & 0xffff) + (acc >> 16);
    }

    // If the data began at an odd byte address
    // then reverse the byte order to compensate.
    if (offset & 1) {
      acc = ((acc & 0xff00) >> 8) | ((acc & 0x00ff) << 8);
    }

    // Return the checksum in network byte order.
    return ~acc;
  }

}// namespace Utils
}// namespace PacketHacker
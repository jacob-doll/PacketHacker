#include "buffer_utils.h"
#include <cstring>

namespace PacketHacker
{
namespace Utils
{

void WriteToBuffer(uint8_t *buffer, uint32_t &offset, const uint8_t value)
{
    buffer[offset] = value;
    offset = offset + 1;
}

void WriteToBuffer(uint8_t *buffer, uint32_t &offset, const uint16_t value)
{
    buffer += offset;
    uint16_t swap = BYTE_SWAP_16(value);
    std::memcpy(buffer, &swap, sizeof(value));
    offset += sizeof(value);
}

void WriteToBuffer(uint8_t *buffer, uint32_t &offset, const uint32_t value)
{
    buffer += offset;
    uint16_t swap = BYTE_SWAP_32(value);
    std::memcpy(buffer, &swap, sizeof(value));
    offset += sizeof(value);
}

void CopyToBuffer(uint8_t *dst, uint8_t *src, uint32_t &offset, uint32_t size)
{
    dst += offset;
    std::memcpy(dst, src, size);
    offset += size;
}

} // namespace Utils
} // namespace PacketHacker
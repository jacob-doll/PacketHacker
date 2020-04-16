#include "buffer_utils.h"

namespace PacketHacker
{
namespace Utils
{

void WriteToBuffer(uint8_t *buffer, uint32_t &offset, uint8_t value)
{
    buffer[offset] = value;
    offset = offset + 1;
}

void WriteToBuffer(uint8_t *buffer, uint32_t &offset, uint16_t value)
{
    for (int i = 0; i < 2; i++)
    {
        buffer[offset] = (value >> (i * 8));
        offset = offset + 1;
    }
}

void WriteToBuffer(uint8_t *buffer, uint32_t &offset, uint32_t value)
{
    for (int i = 0; i < 4; i++)
    {
        buffer[offset] = (value >> (i * 8));
        offset = offset + 1;
    }
}

void CopyToBuffer(uint8_t *dst, uint8_t *src, uint32_t &offset, uint32_t size)
{
    for (int i = 0; i < size; i++)
    {
        dst[offset] = src[i];
        offset = offset + 1;
    }
}

} // namespace Utils
} // namespace PacketHacker
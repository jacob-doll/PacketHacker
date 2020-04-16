#pragma once

#include <stdint.h>

namespace PacketHacker
{
namespace Utils
{

void WriteToBuffer(uint8_t *buffer, uint32_t &offset, uint8_t value);
void WriteToBuffer(uint8_t *buffer, uint32_t &offset, uint16_t value);
void WriteToBuffer(uint8_t *buffer, uint32_t &offset, uint32_t value);

void CopyToBuffer(uint8_t *dst, uint8_t *src, uint32_t &offset, uint32_t size);

} // namespace Utils
} // namespace PacketHacker
#pragma once

#include <stdint.h>
#include <intrin.h>

#if defined(_MSC_VER)
#define BYTE_SWAP_16(data) _byteswap_ushort(data)
#define BYTE_SWAP_32(data) _byteswap_ulong(data)
#define BYTE_SWAP_64(data) _byteswap_uint64(data)
#elif defined(TINS_HAVE_GCC_BUILTIN_SWAP)
#define BYTE_SWAP_16(data) __builtin_bswap16(data)
#define BYTE_SWAP_32(data) __builtin_bswap32(data)
#define BYTE_SWAP_64(data) __builtin_bswap64(data)
#endif

namespace PacketHacker
{
namespace Utils
{

void WriteToBuffer(uint8_t *buffer, uint32_t &offset, const uint8_t value);
void WriteToBuffer(uint8_t *buffer, uint32_t &offset, const uint16_t value);
void WriteToBuffer(uint8_t *buffer, uint32_t &offset, const uint32_t value);

void CopyToBuffer(uint8_t *dst, uint8_t *src, uint32_t &offset, uint32_t size);

} // namespace Utils
} // namespace PacketHacker
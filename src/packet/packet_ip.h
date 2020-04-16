#pragma once

#include "packet.h"
#include "addresses.h"

namespace PacketHacker
{

struct IpHeader
{
    uint8_t version : 4, ihl : 4;
};

} // namespace PacketHacker
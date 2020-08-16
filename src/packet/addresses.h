#pragma once

#include "utils/adapter_utils.h"

#define PHYSICAL_ADDR_LEN 6

namespace PacketHacker {

class HardwareAddress
{
public:
  HardwareAddress(const uint8_t *address = 0);
  HardwareAddress(const char *address);


private:
  uint8_t m_data[PHYSICAL_ADDR_LEN];
};


}// namespace PacketHacker
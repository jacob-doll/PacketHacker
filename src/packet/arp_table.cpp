#include "arp_table.h"

namespace PacketHacker {

ArpTable::ArpTable(const uint32_t ifIndex)
  : m_ifIndex(ifIndex)
{}

}// namespace PacketHacker
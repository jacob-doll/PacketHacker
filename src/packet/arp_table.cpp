#include "arp_table.h"

namespace PacketHacker {

ArpTable::ArpTable(uint32_t ifIndex)
  : m_ifIndex(ifIndex)
{}

}// namespace PacketHacker
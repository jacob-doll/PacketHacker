#include "packet/routing_table.h"

namespace PacketHacker {

RoutingTable::RoutingTable(const uint32_t ifIndex)
  : m_ifIndex(ifIndex)
{}

}// namespace PacketHacker
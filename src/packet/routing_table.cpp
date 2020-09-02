#include "packet/routing_table.h"

namespace PacketHacker {

RoutingTable::RoutingTable(const uint32_t ifIndex)
  : m_ifIndex(ifIndex)
{}

RouteEntry *RoutingTable::GetEntry(const IPv4Address &networkDest)
{
  auto first = m_entries.begin();
  while (first != m_entries.end()) {
    if (first->networkDest == networkDest) return &(*first);
    first++;
  }
  return nullptr;
}

}// namespace PacketHacker
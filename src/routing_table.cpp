#include "routing_table.h"

namespace PacketHacker {

RoutingTable::RoutingTable()
{
  refreshTable();
}

RouteEntry *RoutingTable::getEntryFromNetDest(const IPv4Address &networkDest)
{
  auto first = m_entries.begin();
  while (first != m_entries.end()) {
    if (first->networkDest == networkDest) return &(*first);
    first++;
  }
  return nullptr;
}

RouteEntry *RoutingTable::getEntryFromIpDest(const IPv4Address &ipDest)
{
  auto last = m_entries.rbegin();
  while (last != m_entries.rend()) {
    if ((ipDest & last->netmask) == last->networkDest) return &(*last);
    last++;
  }
  return nullptr;
}

}// namespace PacketHacker
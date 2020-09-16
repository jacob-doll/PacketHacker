#pragma once

#include "ip_address.h"

#include <vector>

namespace PacketHacker {

struct RouteEntry
{
  IPv4Address networkDest;
  IPv4Address netmask;
  IPv4Address nextHop;
  uint32_t metric;
  uint32_t ifIndex;
};

class RoutingTable
{
public:
  void refreshTable();
  void addEntry(const IPv4Address &networkDest,
    const IPv4Address &netmask,
    const IPv4Address &nextHop,
    const uint32_t metric,
    const uint32_t ifIndex);
  void deleteEntry(const IPv4Address &networkDest);
  RouteEntry *getEntryFromNetDest(const IPv4Address &networkDest);
  RouteEntry *getEntryFromIpDest(const IPv4Address &ipDest);

  std::vector<RouteEntry> &entries() { return m_entries; }

  static RoutingTable &instance()
  {
    static RoutingTable instance;
    return instance;
  }

private:
  RoutingTable();
  std::vector<RouteEntry> m_entries;
};

}// namespace PacketHacker
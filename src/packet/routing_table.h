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
};

class RoutingTable
{
public:
  RoutingTable(const uint32_t ifIndex);

  void RefreshTable();

  void AddEntry(const IPv4Address &networkDest,
    const IPv4Address &netmask,
    const IPv4Address &nextHop,
    const uint32_t metric);
  void DeleteEntry(const IPv4Address &networkDest);
  RouteEntry *GetEntry(const IPv4Address &networkDest);

  std::vector<RouteEntry> &GetEntries() { return m_entries; }

private:
  const uint32_t m_ifIndex;
  std::vector<RouteEntry> m_entries;
};

}// namespace PacketHacker
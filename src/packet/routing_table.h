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
  void RefreshTable();
  void AddEntry(const IPv4Address &networkDest,
    const IPv4Address &netmask,
    const IPv4Address &nextHop,
    const uint32_t metric,
    const uint32_t ifIndex);
  void DeleteEntry(const IPv4Address &networkDest);
  RouteEntry *GetEntryFromNetDest(const IPv4Address &networkDest);
  RouteEntry *GetEntryFromIpDest(const IPv4Address &ipDest);

  std::vector<RouteEntry> &GetEntries() { return m_entries; }

  static RoutingTable &GetInstance()
  {
    static RoutingTable instance;
    return instance;
  }

private:
  RoutingTable();
  std::vector<RouteEntry> m_entries;
};

}// namespace PacketHacker
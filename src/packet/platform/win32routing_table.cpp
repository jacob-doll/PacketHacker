#include "packet/routing_table.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#include "packet/utils/buffer_utils.h"

namespace PacketHacker {

void RoutingTable::RefreshTable()
{
  ULONG size = 0;
  PMIB_IPFORWARDTABLE IpForwardTable = nullptr;

  GetIpForwardTable(nullptr, &size, FALSE);
  IpForwardTable = (MIB_IPFORWARDTABLE *)malloc(size);
  m_entries.clear();

  if (GetIpForwardTable(IpForwardTable, &size, FALSE) == NO_ERROR) {
    for (int i = 0; i < IpForwardTable->dwNumEntries; i++) {
      MIB_IPFORWARDROW IpForwardRow = IpForwardTable->table[i];
      if (IpForwardRow.dwForwardIfIndex != m_ifIndex) continue;
      m_entries.emplace_back(RouteEntry{
        IPv4Address(IpForwardRow.dwForwardDest),
        IPv4Address(IpForwardRow.dwForwardMask),
        IPv4Address(IpForwardRow.dwForwardNextHop),
        IpForwardRow.dwForwardMetric1 });
    }
  }

  if (IpForwardTable) {
    free(IpForwardTable);
  }
}

void RoutingTable::AddEntry(const IPv4Address &networkDest,
  const IPv4Address &netmask,
  const IPv4Address &nextHop,
  const uint32_t metric)
{
  MIB_IPFORWARDROW IpForwardRow{};
  IpForwardRow.dwForwardDest = BYTE_SWAP_32(networkDest.GetData());
  IpForwardRow.dwForwardMask = BYTE_SWAP_32(netmask.GetData());
  IpForwardRow.dwForwardNextHop = BYTE_SWAP_32(nextHop.GetData());
  IpForwardRow.dwForwardIfIndex = m_ifIndex;
  IpForwardRow.dwForwardType = MIB_IPROUTE_TYPE_INDIRECT;
  IpForwardRow.dwForwardProto = MIB_IPPROTO_NETMGMT;
  IpForwardRow.dwForwardMetric1 = metric;
  DWORD ret = CreateIpForwardEntry(&IpForwardRow);
  if (ret == NO_ERROR) {
    RefreshTable();
  }
}

void RoutingTable::DeleteEntry(const IPv4Address &networkDest)
{
  RouteEntry entry = GetEntry(networkDest);
  MIB_IPFORWARDROW IpForwardRow{};
  IpForwardRow.dwForwardIfIndex = m_ifIndex;
  IpForwardRow.dwForwardDest = BYTE_SWAP_32(networkDest.GetData());
  IpForwardRow.dwForwardMask = BYTE_SWAP_32(entry.netmask.GetData());
  IpForwardRow.dwForwardNextHop = BYTE_SWAP_32(entry.nextHop.GetData());
  IpForwardRow.dwForwardProto = MIB_IPPROTO_NETMGMT;
  DWORD ret = DeleteIpForwardEntry(&IpForwardRow);
  if (ret == NO_ERROR) {
    RefreshTable();
  }
}

RouteEntry &RoutingTable::GetEntry(const IPv4Address &networkDest)
{
  auto first = m_entries.begin();
  while (first != m_entries.end()) {
    if (first->networkDest == networkDest) return *first;
    first++;
  }
  return *m_entries.end();
}

};// namespace PacketHacker
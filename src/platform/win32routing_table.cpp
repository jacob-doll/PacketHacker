#include "routing_table.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#include "utils/buffer_utils.h"

namespace PacketHacker {

void RoutingTable::refreshTable()
{
  ULONG size = 0;
  PMIB_IPFORWARDTABLE IpForwardTable = nullptr;

  GetIpForwardTable(nullptr, &size, FALSE);
  IpForwardTable = (MIB_IPFORWARDTABLE *)malloc(size);
  m_entries.clear();

  if (GetIpForwardTable(IpForwardTable, &size, FALSE) == NO_ERROR) {
    for (int i = 0; i < IpForwardTable->dwNumEntries; i++) {
      MIB_IPFORWARDROW IpForwardRow = IpForwardTable->table[i];
      m_entries.emplace_back(RouteEntry{
        IPv4Address(IpForwardRow.dwForwardDest),
        IPv4Address(IpForwardRow.dwForwardMask),
        IPv4Address(IpForwardRow.dwForwardNextHop),
        IpForwardRow.dwForwardMetric1,
        IpForwardRow.dwForwardIfIndex });
    }
  }

  if (IpForwardTable) {
    free(IpForwardTable);
  }
}

void RoutingTable::addEntry(const IPv4Address &networkDest,
  const IPv4Address &netmask,
  const IPv4Address &nextHop,
  const uint32_t metric,
  const uint32_t ifIndex)
{
  MIB_IPFORWARDROW IpForwardRow{};
  IpForwardRow.dwForwardDest = BYTE_SWAP_32(networkDest.data());
  IpForwardRow.dwForwardMask = BYTE_SWAP_32(netmask.data());
  IpForwardRow.dwForwardNextHop = BYTE_SWAP_32(nextHop.data());
  IpForwardRow.dwForwardIfIndex = BYTE_SWAP_32(ifIndex);
  IpForwardRow.dwForwardType = MIB_IPROUTE_TYPE_INDIRECT;
  IpForwardRow.dwForwardProto = MIB_IPPROTO_NETMGMT;
  IpForwardRow.dwForwardMetric1 = metric;
  DWORD ret = CreateIpForwardEntry(&IpForwardRow);
  if (ret == NO_ERROR) {
    refreshTable();
  }
}

void RoutingTable::deleteEntry(const IPv4Address &networkDest)
{
  RouteEntry *entry = getEntryFromNetDest(networkDest);
  if (!entry) return;
  MIB_IPFORWARDROW IpForwardRow{};
  IpForwardRow.dwForwardIfIndex = BYTE_SWAP_32(entry->ifIndex);
  IpForwardRow.dwForwardDest = BYTE_SWAP_32(networkDest.data());
  IpForwardRow.dwForwardMask = BYTE_SWAP_32(entry->netmask.data());
  IpForwardRow.dwForwardNextHop = BYTE_SWAP_32(entry->nextHop.data());
  IpForwardRow.dwForwardProto = MIB_IPPROTO_NETMGMT;
  DWORD ret = DeleteIpForwardEntry(&IpForwardRow);
  if (ret == NO_ERROR) {
    refreshTable();
  }
}

};// namespace PacketHacker
#include "packet/arp_table.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <WS2tcpip.h>

#include "packet/utils/buffer_utils.h"

namespace PacketHacker {

void ArpTable::RefreshTable()
{
  ULONG size = 0;
  PMIB_IPNETTABLE IpNetTable = nullptr;

  GetIpNetTable(nullptr, &size, FALSE);
  IpNetTable = (MIB_IPNETTABLE *)malloc(size);

  if (GetIpNetTable(IpNetTable, &size, FALSE) == NO_ERROR) {
    m_entries.clear();
    for (int i = 0; i < IpNetTable->dwNumEntries; i++) {
      MIB_IPNETROW IpNetRow = IpNetTable->table[i];
      if (IpNetRow.dwIndex != m_ifIndex) continue;
      m_entries.emplace_back(ArpEntry{ IPv4Address(IpNetRow.dwAddr), HardwareAddress(IpNetRow.bPhysAddr) });
    }
  }

  if (IpNetTable) {
    free(IpNetTable);
  }
}

void ArpTable::FlushTable()
{
  FlushIpNetTable(m_ifIndex);
}

void ArpTable::AddEntry(IPv4Address ipAddress, HardwareAddress hwAddress)
{
  MIB_IPNETROW IpNetRow{};
  IpNetRow.dwIndex = m_ifIndex;
  IpNetRow.dwPhysAddrLen = PHYSICAL_ADDR_LEN;
  Utils::Write(IpNetRow.bPhysAddr, hwAddress.GetData(), PHYSICAL_ADDR_LEN);
  IpNetRow.dwAddr = ipAddress.GetData();
  IpNetRow.dwType = MIB_IPNET_TYPE_DYNAMIC;

  if (SetIpNetEntry(&IpNetRow) == NO_ERROR) {
    m_entries.emplace_back(ArpEntry{ ipAddress, hwAddress });
  }
}

HardwareAddress &ArpTable::GetEntry(IPv4Address ipAddress)
{
  auto first = m_entries.begin();
  while (first != m_entries.end()) {
    if (first->ipAddress.GetData() == ipAddress.GetData()) return first->hwAddress;
    first++;
  }
  return m_entries.end()->hwAddress;
}

}// namespace PacketHacker
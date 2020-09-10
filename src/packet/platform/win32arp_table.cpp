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
  m_entries.clear();

  if (GetIpNetTable(IpNetTable, &size, FALSE) == NO_ERROR) {
    for (int i = 0; i < IpNetTable->dwNumEntries; i++) {
      MIB_IPNETROW IpNetRow = IpNetTable->table[i];
      m_entries.emplace_back(ArpEntry{ HardwareAddress(IpNetRow.bPhysAddr), IPv4Address(IpNetRow.dwAddr), IpNetRow.dwIndex });
    }
  }

  if (IpNetTable) {
    free(IpNetTable);
  }
}

void ArpTable::FlushTable(uint32_t ifIndex)
{
  FlushIpNetTable(ifIndex);
}

void ArpTable::AddEntry(const IPv4Address &ipAddress, const HardwareAddress &hwAddress, uint32_t ifIndex)
{
  MIB_IPNETROW IpNetRow{};
  IpNetRow.dwIndex = ifIndex;
  IpNetRow.dwPhysAddrLen = PHYSICAL_ADDR_LEN;
  Utils::Write(IpNetRow.bPhysAddr, hwAddress.GetData().data(), PHYSICAL_ADDR_LEN);
  IpNetRow.dwAddr = BYTE_SWAP_32(ipAddress.GetData());
  IpNetRow.dwType = MIB_IPNET_TYPE_STATIC;
  DWORD ret = CreateIpNetEntry(&IpNetRow);
  if (ret == NO_ERROR) {
    RefreshTable();
  }
}

void ArpTable::DeleteEntry(const IPv4Address &ipAddress, uint32_t ifIndex)
{
  MIB_IPNETROW IpNetRow{};
  IpNetRow.dwIndex = ifIndex;
  IpNetRow.dwAddr = BYTE_SWAP_32(ipAddress.GetData());
  DWORD ret = DeleteIpNetEntry(&IpNetRow);
  if (ret == NO_ERROR) {
    RefreshTable();
  }
}


}// namespace PacketHacker
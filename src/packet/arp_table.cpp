#include "arp_table.h"

namespace PacketHacker {

ArpTable::ArpTable()
{
  RefreshTable();
}

ArpEntry *ArpTable::GetEntry(const IPv4Address &ipAddress)
{
  auto first = m_entries.begin();
  while (first != m_entries.end()) {
    if (first->ipAddress == ipAddress) return &(*first);
    first++;
  }
  return nullptr;
}

}// namespace PacketHacker
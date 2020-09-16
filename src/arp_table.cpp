#include "arp_table.h"

namespace PacketHacker {

ArpTable::ArpTable()
{
  refreshTable();
}

ArpEntry *ArpTable::getEntry(const IPv4Address &ipAddress)
{
  auto first = m_entries.begin();
  while (first != m_entries.end()) {
    if (first->ipAddress == ipAddress) return &(*first);
    first++;
  }
  return nullptr;
}

}// namespace PacketHacker
#include "arp_table.h"

namespace PacketHacker {

ArpTable::ArpTable(const uint32_t ifIndex)
  : m_ifIndex(ifIndex)
{}

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
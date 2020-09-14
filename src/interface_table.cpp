#include "interface_table.h"
#include "packets.h"

#include <pcap.h>

namespace PacketHacker {

InterfaceTable::InterfaceTable()
{
  RefreshTable();
}

Interface *InterfaceTable::GetInterface(const uint32_t ifIndex)
{
  auto first = m_interfaces.begin();
  while (first != m_interfaces.end()) {
    if (first->index == ifIndex) return &(*first);
    first++;
  }
  return nullptr;
}

}// namespace PacketHacker
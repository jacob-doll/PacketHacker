#include "interface_table.h"

namespace PacketHacker {

InterfaceTable::InterfaceTable()
{
  refreshTable();
}

Interface *InterfaceTable::getInterface(const uint32_t ifIndex)
{
  auto first = m_interfaces.begin();
  while (first != m_interfaces.end()) {
    if (first->index == ifIndex) return &(*first);
    first++;
  }
  return nullptr;
}

}// namespace PacketHacker
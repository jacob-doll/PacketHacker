#pragma once

#include "hardware_address.h"
#include "ip_address.h"

#include <string>
#include <vector>

namespace PacketHacker {

struct ArpEntry
{
  HardwareAddress hwAddress;
  IPv4Address ipAddress;
  uint32_t ifIndex;
};

class ArpTable
{
public:
  void refreshTable();
  void flushTable(uint32_t ifIndex);

  void addEntry(const IPv4Address &ipAddress, const HardwareAddress &hwAddress, uint32_t ifIndex);
  void deleteEntry(const IPv4Address &ipAddress, uint32_t ifIndex);
  ArpEntry *getEntry(const IPv4Address &ipAddress);

  std::vector<ArpEntry> &entries() { return m_entries; }

  static ArpTable &instance()
  {
    static ArpTable instance;
    return instance;
  }

private:
  ArpTable();
  std::vector<ArpEntry> m_entries;
};

}// namespace PacketHacker
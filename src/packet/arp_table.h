#pragma once

#include "hardware_address.h"
#include "ip_address.h"

#include <string>
#include <vector>

namespace PacketHacker {

struct ArpEntry
{
  IPv4Address ipAddress;
  HardwareAddress hwAddress;
};

class ArpTable
{
public:
  ArpTable(uint32_t ifIndex);

  void RefreshTable();
  void FlushTable();

  void AddEntry(IPv4Address ipAddress, HardwareAddress hwAddress);
  HardwareAddress &GetEntry(IPv4Address ipAddress);

  std::vector<ArpEntry> &GetEntries() { return m_entries; }

private:
  uint32_t m_ifIndex;
  std::vector<ArpEntry> m_entries;
};

}// namespace PacketHacker
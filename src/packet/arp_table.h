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
};

class ArpTable
{
public:
  ArpTable(const uint32_t ifIndex);

  void RefreshTable();
  void FlushTable();

  void AddEntry(const IPv4Address &ipAddress, const HardwareAddress &hwAddress);
  void DeleteEntry(const IPv4Address &ipAddress);
  HardwareAddress &GetEntry(const IPv4Address &ipAddress);

  std::vector<ArpEntry> &GetEntries() { return m_entries; }

private:
  const uint32_t m_ifIndex;
  std::vector<ArpEntry> m_entries;
};

}// namespace PacketHacker
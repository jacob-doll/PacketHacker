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
  void RefreshTable();
  void FlushTable(uint32_t ifIndex);

  void AddEntry(const IPv4Address &ipAddress, const HardwareAddress &hwAddress, uint32_t ifIndex);
  void DeleteEntry(const IPv4Address &ipAddress, uint32_t ifIndex);
  ArpEntry *GetEntry(const IPv4Address &ipAddress);

  std::vector<ArpEntry> &GetEntries() { return m_entries; }

  static ArpTable &GetInstance()
  {
    static ArpTable instance;
    return instance;
  }

private:
  ArpTable();
  std::vector<ArpEntry> m_entries;
};

}// namespace PacketHacker
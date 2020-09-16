#pragma once

#include <string>
#include <vector>
#include <stdint.h>

#include "hardware_address.h"
#include "ip_address.h"

namespace PacketHacker {

struct Interface
{
  uint32_t index;
  HardwareAddress address;
  IPv4Address unicastAddress;
  IPv4Address anycastAddress;
  IPv4Address multicastAddress;
  IPv4Address dnsServerAddress;
  IPv4Address gatewayAddress;
  std::string name;
  std::wstring dnsSuffix;
  std::wstring description;
  std::wstring friendlyName;
};

class InterfaceTable
{
public:
  void refreshTable();
  Interface *bestInterface(IPv4Address &address);

  Interface *getInterface(const uint32_t ifIndex);

  std::vector<Interface> &interfaces() { return m_interfaces; }

  static InterfaceTable &instance()
  {
    static InterfaceTable instance;
    return instance;
  }

private:
  InterfaceTable();
  std::vector<Interface> m_interfaces;
};

}// namespace PacketHacker
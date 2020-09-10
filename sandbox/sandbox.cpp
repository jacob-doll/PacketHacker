#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/interface.h"
#include "packet/routing_table.h"

#include <iostream>

int main()
{
  using namespace PacketHacker;
  Interface interface = Interface::BestInterface(IPv4Address(8, 8, 8, 8));
  std::cout << interface.GetUnicastAddress() << "\n";

  std::cout << "ROUTING TABLE\n";
  for (RouteEntry entry : RoutingTable::GetInstance().GetEntries()) {
    std::cout << entry.networkDest << "\t";
    std::cout << entry.netmask << "\t";
    std::cout << entry.nextHop << "\t";
    std::cout << entry.metric << "\t";
    std::cout << entry.ifIndex << "\n";
  }
  std::cout << "\n";
  std::cout << "ARP TABLE\n";
  for (ArpEntry entry : ArpTable::GetInstance().GetEntries()) {
    std::cout << entry.ipAddress << "\t";
    std::cout << entry.hwAddress << "\t";
    std::cout << entry.ifIndex << "\n";
  }

  return 0;
}

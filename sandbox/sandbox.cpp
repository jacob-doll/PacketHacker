#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/interface.h"

int main()
{
  using namespace PacketHacker;
  Interface interface = Interface::BestInterface(IPv4Address("8.8.8.8"));

  printf("%ws\n", interface.GetFriendlyName().c_str());
  printf("%s\n", interface.GetUnicastAddress().ToString().c_str());
  // for (int i = 0; i < interface.GetArpTable().GetEntries().size(); i++) {
  //   ArpEntry entry = interface.GetArpTable().GetEntries()[i];
  //   printf("\t %s %s\n", entry.hwAddress.ToString().c_str(), entry.ipAddress.ToString().c_str());
  // }
  // // interface.GetArpTable().AddEntry(IPv4Address("1.2.3.4"), HardwareAddress("11:22:33:44:55:66"));
  // // interface.GetArpTable().DeleteEntry(IPv4Address("1.2.3.4"));
  // // interface.GetArpTable().RefreshTable();

  // printf("After\n");
  // for (int i = 0; i < interface.GetArpTable().GetEntries().size(); i++) {
  //   ArpEntry entry = interface.GetArpTable().GetEntries()[i];
  //   printf("\t %s %s\n", entry.hwAddress.ToString().c_str(), entry.ipAddress.ToString().c_str());
  // }

  for (int i = 0; i < interface.GetRoutingTable().GetEntries().size(); i++) {
    RouteEntry entry = interface.GetRoutingTable().GetEntries().at(i);
    printf("\t%s %s %s %d\n", entry.networkDest.ToString().c_str(), entry.netmask.ToString().c_str(), entry.nextHop.ToString().c_str(), entry.metric);
  }

  // interface.GetRoutingTable().AddEntry(IPv4Address("12.0.0.0"), IPv4Address("255.0.0.0"), IPv4Address("50.30.232.1"), 400);
  // interface.GetRoutingTable().DeleteEntry(IPv4Address("12.0.0.0"));
  IPv4Address address("12.0.0.0");
  printf("%s\n", address.ToString().c_str());
  printf("After\n");

  for (int i = 0; i < interface.GetRoutingTable().GetEntries().size(); i++) {
    RouteEntry entry = interface.GetRoutingTable().GetEntries().at(i);
    printf("\t%s %s %s %d\n", entry.networkDest.ToString().c_str(), entry.netmask.ToString().c_str(), entry.nextHop.ToString().c_str(), entry.metric);
  }

  return 0;
}

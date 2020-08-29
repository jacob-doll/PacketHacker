#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/interface.h"

int main()
{
  using namespace PacketHacker;
  // int i = 0;
  // int inum = 0;
  // for (Interface interface : Interface::GetAvailableInterfaces()) {
  //   printf("%d. %ws (%ws)\n", ++i, interface.GetFriendlyName().c_str(), interface.GetDescription().c_str());
  // }

  // printf("Enter the interface number (1-%d):", i);
  // scanf("%d", &inum);

  // if (inum < 1 || inum > i) {
  //   printf("\nInterface number out of range.\n");
  //   return -1;
  // }

  // Interface interface = Interface::GetAvailableInterfaces()[inum - 1];
  Interface interface = Interface::BestInterface(IPv4Address("8.8.8.8"));

  printf("%ws\n", interface.GetFriendlyName().c_str());
  printf("%s\n", interface.GetUnicastAddress().ToString().c_str());
  for (int i = 0; i < interface.GetArpTable().GetEntries().size(); i++) {
    ArpEntry entry = interface.GetArpTable().GetEntries()[i];
    printf("\t %s %s\n", entry.hwAddress.ToString().c_str(), entry.ipAddress.ToString().c_str());
  }
  interface.GetArpTable().AddEntry(IPv4Address("1.2.3.4"), HardwareAddress("11:22:33:44:55:66"));
  // interface.GetArpTable().DeleteEntry(IPv4Address("1.2.3.4"));
  // interface.GetArpTable().RefreshTable();

  printf("After\n");
  for (int i = 0; i < interface.GetArpTable().GetEntries().size(); i++) {
    ArpEntry entry = interface.GetArpTable().GetEntries()[i];
    printf("\t %s %s\n", entry.hwAddress.ToString().c_str(), entry.ipAddress.ToString().c_str());
  }

  return 0;
}

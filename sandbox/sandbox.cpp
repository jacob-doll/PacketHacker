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

  // Interface interface = Interface::GetAvailableInterfaces()[inum];
  Interface interface = Interface::BestInterface(IPv4Address("8.8.8.8"));

  printf("%s\n", interface.GetUnicastAddress().ToString().c_str());
  for (ArpEntry entry : interface.GetArpTable()) {
    printf("\t %s %s %s\n", entry.hwAddress.ToString().c_str(), entry.ipAddress.ToString().c_str(), entry.type.c_str());
  }

  return 0;
}

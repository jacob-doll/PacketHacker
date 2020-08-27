#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/adapter.h"

int main()
{
  using namespace PacketHacker;
  int i = 0;
  int inum = 0;
  for (AdapterInfo info : Adapter::GetAvailableAdapters()) {
    printf("%d. %ws (%ws)\n", ++i, info.friendlyName.c_str(), info.description.c_str());
  }

  printf("Enter the interface number (1-%d):", i);
  scanf("%d", &inum);

  if (inum < 1 || inum > i) {
    printf("\nInterface number out of range.\n");
    return -1;
  }

  AdapterInfo info = Adapter::GetAvailableAdapters()[inum];
  Adapter adapter(info);

  printf("%s\n", info.unicastAddress.ToString().c_str());
  for (ArpEntry entry : adapter.GetArpTable()) {
    printf("\t %s %s %s\n", entry.hwAddress.ToString().c_str(), entry.ipAddress.ToString().c_str(), entry.type.c_str());
  }

  return 0;
}

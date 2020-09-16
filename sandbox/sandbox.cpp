#include "packet.h"
#include "packets.h"
#include "interface_table.h"
#include "routing_table.h"
#include "arp_table.h"
#include "packet_stream.h"

#include <iostream>

int main()
{
  using namespace PacketHacker;
  Interface *currInterface = InterfaceTable::GetInstance().BestInterface(IPv4Address(8, 8, 8, 8));

  std::cout << currInterface->unicastAddress << "\n";

  PacketStream stream(currInterface);

  char errbuf[500];
  if (!stream.OpenPacketStream(errbuf)) {
    std::cout << errbuf << "\n";
    return 1;
  }

  stream.ClosePacketStream();

  return 0;
}

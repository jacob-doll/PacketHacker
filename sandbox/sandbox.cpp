#include "packet/packet.h"
#include "packet/packets.h"
#include "packet/interface_table.h"
#include "packet/routing_table.h"
#include "packet/arp_table.h"
#include "packet/packet_stream.h"

#include <iostream>

int main()
{
  using namespace PacketHacker;
  Interface *currInterface = InterfaceTable::GetInstance().BestInterface(IPv4Address(8, 8, 8, 8));

  PacketStream stream(currInterface);

  char errbuf[500];
  if (!stream.OpenPacketStream(errbuf)) {
    std::cout << errbuf << "\n";
    return 1;
  }

  for (int i = 0; i < 5; i++) {
    uint32_t size;
    const uint8_t *data = stream.GetNextPacket(&size, errbuf);
    if (data) {
      std::cout << size << "\n";
    }
  }

  stream.ClosePacketStream();

  return 0;
}

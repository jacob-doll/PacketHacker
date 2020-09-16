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
  Interface *currInterface = InterfaceTable::instance().bestInterface(IPv4Address(8, 8, 8, 8));

  std::cout << currInterface->unicastAddress << "\n";

  PacketStream stream(currInterface);

  char errbuf[500];
  if (!stream.openPacketStream(errbuf)) {
    std::cout << errbuf << "\n";
    return 1;
  }

  while (stream.streamOpen()) {
    uint32_t size;
    const uint8_t *data = stream.getNextPacket(&size, errbuf);
    EthernetPacket eth(data, size);
    std::cout << "Eth{" << eth.dstMac() << ", " << eth.srcMac() << "}\n";
  }

  stream.closePacketStream();

  return 0;
}

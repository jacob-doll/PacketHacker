#include "packet.h"
#include "packets.h"
#include "interface_table.h"
#include "routing_table.h"
#include "arp_table.h"
#include "packet_stream.h"
#include "constants.h"

#include <iostream>

int main()
{
  using namespace PacketHacker;
  Interface *currInterface = InterfaceTable::instance().bestInterface(IPv4Address(8, 8, 8, 8));

  std::cout << currInterface->unicastAddress << "\n";

  PacketStream stream(currInterface);

  if (!stream.openPacketStream()) {
    std::cout << "Error opening packet stream!\n";
    return 1;
  }

  EthernetPacket *eth = new EthernetPacket();
  IpPacket *ip = new IpPacket();
  IcmpPacket *icmp = new IcmpPacket();
  eth->type(Constants::TYPE_IPv4);
  eth->dstMac({ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff });
  eth->srcMac(currInterface->address);
  ip->version(4);
  ip->headerLength(5);
  ip->diffServices(0);
  ip->id(0x0001);
  ip->flags(0);
  ip->fragOffset(0);
  ip->ttl(128);
  ip->protocol(Constants::TYPE_ICMP);
  ip->sourceIp(currInterface->unicastAddress);
  ip->destIp(currInterface->gatewayAddress);
  icmp->type(IcmpPacket::ADDRESS_MASK_REQUEST);
  icmp->code(0);
  icmp->id(1);
  icmp->sequence(1);
  eth->innerPacket(ip);
  ip->innerPacket(icmp);

  stream.sendPacket(eth);

  stream.closePacketStream();

  return 0;
}

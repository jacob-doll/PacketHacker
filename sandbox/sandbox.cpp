#include <iostream>

#include "packet.h"
#include "interface_table.h"
#include "routing_table.h"
#include "arp_table.h"
#include "packet_stream.h"
#include "constants.h"

#include "layers/eth_layer.h"
#include "layers/ip_layer.h"
#include "layers/icmp_layer.h"

int main(void)
{
  using namespace PacketHacker;
  Interface *currInterface = InterfaceTable::instance().bestInterface(IPv4Address(8, 8, 8, 8));

  std::cout << currInterface->unicastAddress << "\n";

  PacketStream stream(currInterface);

  if (!stream.openPacketStream()) {
    std::cout << "Error opening packet stream!\n";
    return 1;
  }

  Packet packet;

  EthernetLayer *eth = new EthernetLayer;
  IpLayer *ip = new IpLayer;
  IcmpLayer *icmp = new IcmpLayer;
  eth->protoType(Constants::TYPE_IPv4);
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
  icmp->icmpType(IcmpLayer::ECHO_REQUEST);
  icmp->code(0);
  icmp->id(1);
  icmp->sequence(1);

  packet.insertLayer(eth);
  packet.insertLayer(ip);
  packet.insertLayer(icmp);

  IcmpLayer *test = packet.getLayer<IcmpLayer>();
  if (test) {
    std::cout << test->id() << "\n";
  }

  // const uint32_t size = packet.size();
  // std::vector<uint8_t> data(size);
  // packet.writeToBuf(data.data(), size);

  // for (int i = 0; i < size; i++) {
  //   std::cout << std::hex << (int)data.at(i) << " ";
  // }
  // std::cout << "\n";

  // stream.sendPacket(packet);

  // while (stream.streamOpen()) {
  //   uint32_t size;
  //   const uint8_t *data = stream.getNextPacket(&size);
  //   if (packet.isReply(data, size)) {
  //     std::cout << "Received reply\n";
  //     break;
  //   }
  // }

  stream.closePacketStream();

  return 0;
}

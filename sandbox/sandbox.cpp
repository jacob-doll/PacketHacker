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

  packet.payload(new StringPayload("Hello World!"));

  IcmpLayer *test = packet.getLayer<IcmpLayer>();
  if (test) {
    std::cout << test->id() << "\n";
  }

  const SizeType size = packet.size();
  std::vector<DataType> data(size);
  packet.writeToBuf(data.data(), size);

  for (int i = 0; i < size; i++) {
    std::cout << std::hex << (int)data.at(i) << " ";
  }
  std::cout << "\n";

  stream.sendPacket(packet);

  while (stream.streamOpen()) {
    SizeType recv_size;
    const DataType *recv_data = stream.getNextPacket(&recv_size);
    if (packet.isReply(recv_data, recv_size)) {
      std::cout << "Received reply\n";
      Packet received(recv_data, size);
      std::vector<DataType> recv_vector(recv_size);
      received.writeToBuf(recv_vector.data(), size);
      for (int i = 0; i < recv_size; i++) {
        std::cout << std::hex << (int)recv_vector.at(i) << " ";
      }
      std::cout << "\n";
      break;
    }
  }

  stream.closePacketStream();

  return 0;
}

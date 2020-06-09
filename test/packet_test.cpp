#include "packet/packet.h"
#include "packet/eth_packet.h"
#include "packet/arp_packet.h"
#include "packet/utils/utils.h"
#include "packet/adapter.h"

void ParseTest()
{
  using namespace PacketHacker;

  uint8_t data[42] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x08, 0x00, 0x20, 0x1A, 0x1B, 0xDE, 0xC6, 0x0B, 0x6D, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x0B, 0x6D, 0x0A };
  EthernetPacket *eth = new EthernetPacket(data, 42);
  printf("%s", eth->ToString().c_str());
  delete eth;
}

void MatchReplyTest()
{
  using namespace PacketHacker;
  uint8_t sent[42] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x15, 0x5d, 0x40, 0xaf, 0x88, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0x00, 0x15, 0x5d, 0x40, 0xaf, 0x88, 0xac, 0x12, 0x0f, 0xd1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xac, 0x12, 0x0f, 0xdf };
  uint8_t received[42] = { 0x00, 0x15, 0x5d, 0x40, 0xaf, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xac, 0x12, 0x0f, 0xdf, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xac, 0x12, 0x0f, 0xd1 };
  EthernetPacket *eth1 = new EthernetPacket(sent, 42);
  EthernetPacket *eth2 = new EthernetPacket(received, 42);
  printf("%s\n", eth1->ToString().c_str());
  printf("%s\n", eth2->ToString().c_str());
  if (eth1->DoesReplyMatch(received, 42)) {
    printf("Reply matched\n");
  }
  delete eth1;
  delete eth2;
}

void AdapterTest()
{
  using namespace PacketHacker;
  // for (AdapterInfo info : Utils::GetAdapters()) {
  //   printf("%ws\n", info.friendlyName.c_str());
  // }
  AdapterInfo info = Utils::GetAdapters()[5];
  printf("%ws\n", info.friendlyName.c_str());

  std::string senderMac = Utils::HardwareAddressToString(info.address);
  std::string senderIp = info.unicastAddress;
  std::string targetIp = info.dnsServerAddress;

  EthernetPacket *eth = new EthernetPacket();
  ArpPacket *arp = new ArpPacket();
  eth->SetInnerPacket(arp);
  eth->SetDst("ff:ff:ff:ff:ff:ff");
  eth->SetSrc(senderMac.c_str());
  eth->SetType("0x0806");
  arp->SetHardwareType("0x0001");
  arp->SetProtocolType("0x0800");
  arp->SetHardwareLength("0x06");
  arp->SetProtocolLength("0x04");
  arp->SetOpcode("0x0001");
  arp->SetSenderMac(senderMac.c_str());
  arp->SetSenderIp(senderIp.c_str());
  arp->SetTargetMac("00:00:00:00:00:00");
  arp->SetTargetIp(targetIp.c_str());

  printf("Sending packet:\n%s\n", eth->ToString().c_str());

  const uint8_t *data;
  uint32_t size;
  char errbuf[256];
  Adapter adapter(info.name);
  if (!adapter.OpenPacketStream(errbuf)) {
    printf("%s\n", errbuf);
  }

  adapter.SendPacket(eth, errbuf);

  while ((data = adapter.GetNextPacket(&size, errbuf)) != nullptr) {
    if (eth->DoesReplyMatch(data, size)) {
      Packet *packet = new EthernetPacket(data, size);
      printf("Receive Reply!\n");
      printf("%s\n", packet->ToString().c_str());
      delete packet;
      break;
    }
  }

  adapter.ClosePacketStream();

  delete data;
  delete eth;
}

int main()
{
  AdapterTest();

  return 0;
}

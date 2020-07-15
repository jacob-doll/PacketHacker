#include "packet/packet.h"
#include "packet/packets.h"
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
  for (AdapterInfo info : Utils::GetAdapters()) {
    printf("%ws\n", info.friendlyName.c_str());
  }
  AdapterInfo info = Utils::GetAdapters()[3];
  printf("%ws\n", info.friendlyName.c_str());

  std::string senderMac = Utils::HardwareAddressToString(info.address);
  std::string senderIp = info.unicastAddress;
  std::string targetIp = info.dnsServerAddress;

  // const uint8_t data[42] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xd5, 0x5e, 0x61, 0xb5, 0x7d, 0x08, 0x06, 0x00, 0x01, 0x08, 0x00, 0x06, 0x04, 0x00, 0x01, 0xe0, 0xd5, 0x5e, 0x61, 0xb5, 0x7d, 0xc0, 0xa8, 0x01, 0x0a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x01 };

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

  uint8_t *out = new uint8_t[eth->Size()];
  eth->WriteToBuf(out, eth->Size());

  for (int i = 0; i < eth->Size(); i++) {
    printf("%02x ", out[i]);
  }

  delete out;

  const uint8_t *data;
  uint32_t size;
  char errbuf[256];
  Adapter adapter(info.name);
  if (!adapter.OpenPacketStream(errbuf)) {
    printf("%s\n", errbuf);
  }

  adapter.SendPacket(eth, errbuf);

  while ((data = adapter.GetNextPacket(&size, errbuf)) != nullptr) {
    Packet *packet = new EthernetPacket(data, size);
    printf("%s\n", packet->ToString().c_str());
    delete packet;
    if (eth->DoesReplyMatch(data, size)) {
      printf("Receive Reply!\n");
      break;
    }
  }

  adapter.ClosePacketStream();

  delete eth;
}

void IpTest()
{
  using namespace PacketHacker;
  AdapterInfo info = Utils::GetAdapters()[3];
  printf("%ws\n", info.friendlyName.c_str());

  std::string senderMac = Utils::HardwareAddressToString(info.address);
  std::string senderIp = info.unicastAddress;
  std::string targetIp = info.dnsServerAddress;

  const uint8_t data[20] = { 0x45, 0x00, 0x00, 0x4b, 0xe4, 0x69, 0x40, 0x00, 0x80, 0x06, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x0a, 0x23, 0xba, 0xe0, 0x2f };

  IpPacket *ip = new IpPacket();
  ip->SetVersion("4");
  ip->SetHeaderLength("5");
  ip->SetDiffServices("0x00");
  ip->SetTotalLength("75");
  ip->SetId("0xe469");
  ip->SetFlags("0x02");
  ip->SetFragOffset("0x0000");
  ip->SetTtl("0x80");
  ip->SetProtocol("0x06");
  ip->SetChecksum("0x0000");
  ip->SetSourceIp("192.168.1.10");
  ip->SetDestIp("35.186.224.47");
  printf("%s\n", ip->ToString().c_str());

  uint8_t out[20];

  ip->WriteToBuf(out, 20);

  for (int i = 0; i < 20; i++) {
    if (data[i] != out[i]) {
      printf("%02x ", out[i]);
    }
  }
  delete ip;
}

void ChecksumTest()
{
  using namespace PacketHacker;

  const uint8_t data[20] = { 0x45, 0x00, 0x00, 0x73, 0x00, 0x00, 0x40, 0x00, 0x40, 0x11, 0x00, 0x00, 0xc0, 0xa8, 0x00, 0x01, 0xc0, 0xa8, 0x00, 0xc7 };
  IpPacket ip(data, 20);
  printf("%s\n", ip.ToString().c_str());

  uint8_t out[20];
  ip.WriteToBuf(out, 20);

  for (int i = 0; i < 20; i++) {
    printf("%02x ", out[i]);
  }

  printf("\n");
  printf("%s\n", ip.ToString().c_str());
  // uint16_t check = Utils::CalcChecksum((void *)data, 20);
  // printf("%04x", check);
}

void Icmp()
{
  using namespace PacketHacker;
  AdapterInfo info = Utils::GetAdapters()[4];
  printf("%ws\n", info.friendlyName.c_str());

  std::string senderMac = Utils::HardwareAddressToString(info.address);
  std::string senderIp = info.unicastAddress;
  std::string targetIp = info.dnsServerAddress;

  // const uint8_t in[42] = { 0x10, 0xda, 0x43, 0x96, 0x84, 0xcf, 0xe0, 0xd5, 0x5e, 0x61, 0xb5, 0x7d, 0x08, 0x00, 0x45, 0x00, 0x00, 0x3c, 0xfe, 0xe0, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x0a, 0xc0, 0xa8, 0x01, 0x01, 0x08, 0x00, 0x4d, 0x5a, 0x00, 0x01, 0x00, 0x01 };
  EthernetPacket *eth = new EthernetPacket();
  IpPacket *ip = new IpPacket();
  IcmpPacket *icmp = new IcmpPacket();
  DataPacket *data_ = new DataPacket();
  eth->SetDst("10:da:43:96:84:cf");
  eth->SetSrc(senderMac.c_str());
  eth->SetType("0x0800");
  ip->SetProtocol("1");
  ip->SetSourceIp(senderIp.c_str());
  ip->SetDestIp(targetIp.c_str());
  icmp->SetType("8");
  icmp->SetCode("0");
  icmp->SetData("0x00010001");
  data_->SetData("abcdefg");
  eth->SetInnerPacket(ip);
  ip->SetInnerPacket(icmp);
  icmp->SetInnerPacket(data_);


  printf("%s\n", eth->ToString().c_str());

  uint8_t out[42];
  eth->WriteToBuf(out, 42);

  for (int i = 0; i < 42; i++) {
    printf("%02x ", out[i]);
  }
  printf("\n");
  printf("%s\n", eth->ToString().c_str());

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
      printf("Receive Reply!\n");
      Packet *recieved = new EthernetPacket(data, size);
      printf("%s\n", recieved->ToString().c_str());
      delete recieved;
      break;
    }
  }

  adapter.ClosePacketStream();
  delete eth;
}

int main()
{
  Icmp();
  return 0;
}

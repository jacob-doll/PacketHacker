#include "packet/packet.h"
#include "packet/eth_packet.h"
#include "packet/arp_packet.h"
#include "packet/utils/utils.h"

int main()
{
  using namespace PacketHacker;
  EthernetPacket *eth = new EthernetPacket();
  ArpPacket *arp = new ArpPacket();
  // eth->SetInnerPacket(arp);
  // eth->SetDst("ff:ff:ff:ff:ff:ff");
  // eth->SetSrc("00:11:22:33:44:55");
  // eth->SetType("0x0806");
  // arp->SetHardwareType("0x0001");
  // arp->SetProtocolType("0x0800");
  // arp->SetHardwareLength("0x06");
  // arp->SetProtocolLength("0x04");
  // arp->SetOpcode("0x0001");
  // arp->SetSenderMac("00:11:22:33:44:55");
  // arp->SetSenderIp("192.168.1.12");
  // arp->SetTargetMac("00:00:00:00:00:00");
  // arp->SetTargetIp("192.168.1.1");
  // uint8_t data[42]{};
  // eth->WriteToBuf(data, 42);
  // for (int i = 0; i < 42; i++) {
  //   printf("%.2x ", data[i]);
  // }
  // printf("\n");
  // AdapterInfo info = Utils::GetAdapters()[4];
  // printf("%ws\n", info.friendlyName.c_str());
  // Utils::SendPacket(info, data, 42);

  uint8_t data[14] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x06 };
  eth->Parse(data, 14);
  uint8_t out_data[14]{};
  eth->WriteToBuf(out_data, 14);
  for (int i = 0; i < 14; i++) {
    printf("%.2x ", data[i]);
  }

  delete eth;
  return 0;
}
#pragma once

namespace PacketHacker {
namespace Constants {

  enum Ethernet {
    TYPE_IPv4 = 0x0800,
    TYPE_ARP = 0x0806,
  };

  enum IP {
    TYPE_ICMP = 1,
    TYPE_IGMP = 2,
    TYPE_TCP = 6,
    TYPE_UDP = 17,
    TYPE_ENCAP = 41,
    TYPE_OSPF = 89,
    TYPE_SCTP = 132,
  };

}// namespace Constants
}// namespace PacketHacker
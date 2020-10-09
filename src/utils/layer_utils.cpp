#include "layer.h"
#include "constants.h"

#include "layers/arp_layer.h"
#include "layers/data_layer.h"
#include "layers/eth_layer.h"
#include "layers/icmp_layer.h"
#include "layers/ip_layer.h"
#include "layers/udp_layer.h"

namespace PacketHacker {
namespace Utils {

  Layer *LayerFromId(const uint32_t packetId)
  {
    switch (packetId) {
    case Layer::LayerType::ARP:
      return new ArpLayer();
    case Layer::LayerType::ETHERNET:
      return new EthernetLayer();
    case Layer::LayerType::IP:
      return new IpLayer();
    case Layer::LayerType::ICMP:
      return new IcmpLayer();
    case Layer::LayerType::DATA:
      return new DataLayer();
    case Layer::LayerType::UDP:
      return new UdpLayer();
    default:
      return nullptr;
    }
  }

  Layer *LayerFromType(const uint32_t type, const uint8_t *buffer, const uint32_t size)
  {
    switch (type) {
    case Constants::Ethernet::TYPE_ARP:
      return new ArpLayer(buffer, size);
    case Constants::Ethernet::TYPE_IPv4:
      return new IpLayer(buffer, size);
    case Constants::IP::TYPE_ICMP:
      return new IcmpLayer(buffer, size);
    case Constants::IP::TYPE_UDP:
      return new UdpLayer(buffer, size);
    default:
      break;
    }
    return nullptr;
  }

}// namespace Utils

}
#pragma once

#include <string>
#include <stdint.h>

#include "packet.h"
#include "arp_table.h"

struct pcap;

namespace PacketHacker {

struct InterfaceInfo
{
  uint32_t index;
  HardwareAddress address;
  IPv4Address unicastAddress;
  IPv4Address anycastAddress;
  IPv4Address multicastAddress;
  IPv4Address dnsServerAddress;
  IPv4Address gatewayAddress;
  std::string name;
  std::wstring dnsSuffix;
  std::wstring description;
  std::wstring friendlyName;
};

class Interface
{
public:
  Interface();
  Interface(const InterfaceInfo &info);

  // Getters for adapter info
  const uint32_t GetIndex() { return m_info.index; }
  const HardwareAddress &GetAddress() { return m_info.address; }
  const IPv4Address &GetUnicastAddress() { return m_info.unicastAddress; }
  const IPv4Address &GetAnycastAddress() { return m_info.anycastAddress; }
  const IPv4Address &GetMulticastAddress() { return m_info.multicastAddress; }
  const IPv4Address &GetDnsServerAddress() { return m_info.dnsServerAddress; }
  const IPv4Address &GetGatewayAddress() { return m_info.gatewayAddress; }
  const std::string &GetName() { return m_info.name; }
  const std::wstring &GetDnsSuffix() { return m_info.dnsSuffix; }
  const std::wstring &GetDescription() { return m_info.description; }
  const std::wstring &GetFriendlyName() { return m_info.friendlyName; }

  // Packet Streams
  bool OpenPacketStream(char *errbuf);
  void ClosePacketStream();
  bool SendPacket(Packet *packet, char *errbuf);
  const uint8_t *GetNextPacket(uint32_t *size, char *errbuf);
  bool IsStreamOpen() const { return m_streamOpen; }

  // Arp table for given adapter
  ArpTable GetArpTable() { return m_arpTable; }

  static const Interface DefaultInterface() { return s_availableInterfaces[0]; }
  static const Interface BestInterface(IPv4Address &address);
  static const std::vector<Interface> &GetAvailableInterfaces() { return s_availableInterfaces; }

private:
  const InterfaceInfo m_info;
  ArpTable m_arpTable;

  pcap *m_handle;
  bool m_streamOpen;

private:
  static const std::vector<Interface> s_availableInterfaces;
};

}// namespace PacketHacker
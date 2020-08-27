#pragma once

#include <string>
#include <stdint.h>

#include "packet/packet.h"

struct pcap;

namespace PacketHacker {

struct AdapterInfo
{
  uint32_t index;
  std::string name;
  IPv4Address unicastAddress;
  IPv4Address anycastAddress;
  IPv4Address multicastAddress;
  IPv4Address dnsServerAddress;
  std::wstring dnsSuffix;
  std::wstring description;
  std::wstring friendlyName;
  HardwareAddress address;
  IPv4Address gatewayAddress;
};

struct ArpEntry
{
  HardwareAddress hwAddress;
  IPv4Address ipAddress;
  std::string type;
};

class Adapter
{
public:
  Adapter(const AdapterInfo &info);

  // Packet Streams
  bool OpenPacketStream(char *errbuf);
  void ClosePacketStream();
  bool SendPacket(Packet *packet, char *errbuf);
  const uint8_t *GetNextPacket(uint32_t *size, char *errbuf);
  bool IsStreamOpen() const { return m_streamOpen; }

  // Arp table for given adapter
  const std::vector<ArpEntry> GetArpTable();

  const static std::vector<AdapterInfo> &GetAvailableAdapters() { return s_availableAdapters; }

private:
  const std::string m_name;
  const uint32_t m_index;

  pcap *m_handle;
  bool m_streamOpen;

private:
  const static std::vector<AdapterInfo> s_availableAdapters;
};

}// namespace PacketHacker
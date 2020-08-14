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
  std::string unicastAddress;
  std::string anycastAddress;
  std::string multicastAddress;
  std::string dnsServerAddress;
  std::wstring dnsSuffix;
  std::wstring description;
  std::wstring friendlyName;
  std::string address;
  std::string gatewayAddress;
};

class Adapter
{
public:
  Adapter(std::string name);

  bool OpenPacketStream(char *errbuf);
  void ClosePacketStream();
  bool SendPacket(Packet *packet, char *errbuf);
  const uint8_t *GetNextPacket(uint32_t *size, char *errbuf);

  bool IsStreamOpen() const { return m_streamOpen; }

  const static std::vector<AdapterInfo> &GetAvailableAdapters() { return s_availableAdapters; }

private:
  const std::string m_name;
  pcap *m_handle;
  bool m_streamOpen;

private:
  const static std::vector<AdapterInfo> s_availableAdapters;
};

}// namespace PacketHacker
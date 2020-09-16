#pragma once

#include "packet.h"

#include "ip_address.h"
#include "utils/buffer_utils.h"

namespace PacketHacker {

class IpPacket : public Packet
{
public:
  IpPacket();
  IpPacket(const uint8_t *data, uint32_t size);

  uint8_t version() { return m_header.version; }
  uint8_t headerLength() { return m_header.headerLength; }
  uint8_t diffServices() { return m_header.diffServices; }
  uint16_t totalLength() { return BYTE_SWAP_16(m_header.totalLength); }
  uint16_t id() { return BYTE_SWAP_16(m_header.id); }
  uint16_t flags() { return BYTE_SWAP_16(m_header.flags); }
  uint16_t fragOffset() { return BYTE_SWAP_16(m_header.fragOffset); }
  uint8_t ttl() { return m_header.ttl; }
  uint8_t protocol() { return m_header.protocol; }
  uint16_t checksum() { return BYTE_SWAP_16(m_header.checksum); }
  IPv4Address sourceIp() { return IPv4Address(m_header.sourceIp); }
  IPv4Address destIp() { return IPv4Address(m_header.destIp); }

  void version(const uint8_t version);
  void headerLength(const uint8_t headerLength);
  void diffServices(const uint8_t diffServices);
  void totalLength(const uint16_t totalLength);
  void id(const uint16_t id);
  void flags(const uint16_t flags);
  void fragOffset(const uint16_t fragOffset);
  void ttl(const uint8_t ttl);
  void protocol(const uint8_t protocol);
  void checksum(const uint16_t checksum);
  void sourceIp(const IPv4Address &sourceIp);
  void destIp(const IPv4Address &destIp);

  virtual const PacketType packetType() const override { return PacketType::IP; }
  virtual const std::string name() const override { return "IPv4"; }
  virtual const uint32_t headerSize() const override { return sizeof(IpHeader); }
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  virtual void doWriteToBuf(uint8_t *buffer) override;

private:
#pragma pack(push, 1)
  struct IpHeader
  {
    union {
      uint8_t hdrVerLen;
      struct
      {
        uint8_t headerLength : 4, version : 4;
      };
    };
    union {
      uint8_t dscpEcn;
      struct
      {
        uint8_t ecn : 2, diffServices : 6;
      };
    };
    uint16_t totalLength;
    uint16_t id;
    union {
      uint16_t flagsOffset;
      struct
      {
        uint16_t fragOffset : 13, flags : 3;
      };
    };
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t sourceIp;
    uint32_t destIp;
  };
#pragma pack(pop)

  IpHeader m_header;
};

}// namespace PacketHacker

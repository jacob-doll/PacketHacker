#pragma once

#include "packet.h"

#include "ip_address.h"

namespace PacketHacker {

class IpPacket : public Packet
{
public:
  IpPacket();
  IpPacket(const uint8_t *data, uint32_t size);

  void SetVersion(const uint8_t version);
  void SetHeaderLength(const uint8_t headerLength);
  void SetDiffServices(const uint8_t diffServices);
  void SetTotalLength(const uint16_t totalLength);
  void SetId(const uint16_t id);
  void SetFlags(const uint16_t flags);
  void SetFragOffset(const uint16_t fragOffset);
  void SetTtl(const uint8_t ttl);
  void SetProtocol(const uint8_t protocol);
  void SetChecksum(const uint16_t checksum);
  void SetSourceIp(const IPv4Address &sourceIp);
  void SetDestIp(const IPv4Address &destIp);

  const uint32_t GetSourceIp() const { return m_header.sourceIp; }
  const uint32_t GetDestIp() const { return m_header.destIp; }

  virtual const PacketType GetPacketType() const override { return PacketType::IP; }
  virtual const std::string GetName() const override { return "IPv4"; }
  virtual const uint32_t HeaderSize() const override { return sizeof(IpHeader); }
  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  virtual void DoWriteToBuf(uint8_t *buffer) override;

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

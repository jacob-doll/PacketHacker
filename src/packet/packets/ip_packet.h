#pragma once

#include "packet/packet.h"

namespace PacketHacker {

class IpPacket : public Packet
{
public:
  IpPacket();
  IpPacket(const uint8_t *data, uint32_t size);

  void SetVersion(const FieldData &val);
  void SetHeaderLength(const FieldData &val);
  void SetDiffServices(const FieldData &val);
  void SetTotalLength(const FieldData &val);
  void SetId(const FieldData &val);
  void SetFlags(const FieldData &val);
  void SetFragOffset(const FieldData &val);
  void SetTtl(const FieldData &val);
  void SetProtocol(const FieldData &val);
  void SetChecksum(const FieldData &val);
  void SetSourceIp(const FieldData &val);
  void SetDestIp(const FieldData &val);

  const uint32_t GetSourceIp() const { return m_header.sourceIp; }
  const uint32_t GetDestIp() const { return m_header.destIp; }

  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override;
  virtual uint32_t HeaderSize() const override;
  virtual std::string GetName() const override { return "IPv4"; }
  virtual PacketType GetPacketType() const override { return PacketType::IP; }

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
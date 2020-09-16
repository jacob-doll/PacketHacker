#pragma once
#include <stdint.h>
#include <string>


namespace PacketHacker {

enum PacketType {
  ARP = 1000,
  ETHERNET,
  IP,
  ICMP,
  DATA,
  UDP
};

class Packet
{
public:
  Packet();
  virtual ~Packet();

  Packet *GetOuterPacket() const { return m_outerPacket; }
  Packet *GetInnerPacket() const { return m_innerPacket; }
  Packet *GetPacket(const std::string &name);
  void SetInnerPacket(Packet *inner);
  void RemoveInnerPacket();
  const uint32_t Size() const;

  void WriteToBuf(uint8_t *buffer, const uint32_t size);

  virtual const PacketType GetPacketType() const = 0;
  virtual const std::string GetName() const = 0;
  virtual const uint32_t HeaderSize() const = 0;
  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) = 0;

protected:
  virtual void DoWriteToBuf(uint8_t *buffer) = 0;

private:
  void SetOuterPacket(Packet *outer);

private:
  Packet *m_outerPacket;
  Packet *m_innerPacket;
};

std::ostream &operator<<(std::ostream &output, Packet *packet);

}// namespace PacketHacker

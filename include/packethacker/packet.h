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

  Packet *outerPacket() const { return m_outerPacket; }
  Packet *innerPacket() const { return m_innerPacket; }
  Packet *getPacket(const std::string &name);
  void innerPacket(Packet *inner);
  void removeInnerPacket();
  const uint32_t size() const;

  void writeToBuf(uint8_t *buffer, const uint32_t size);

  virtual const PacketType packetType() const = 0;
  virtual const std::string name() const = 0;
  virtual const uint32_t headerSize() const = 0;
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) = 0;

protected:
  virtual void doWriteToBuf(uint8_t *buffer) = 0;

private:
  void outerPacket(Packet *outer);

private:
  Packet *m_outerPacket;
  Packet *m_innerPacket;
};

std::ostream &operator<<(std::ostream &output, Packet *packet);

}// namespace PacketHacker

#pragma once

#include "packet/packet.h"
#include <vector>

namespace PacketHacker {

class DataPacket : public Packet
{
public:
  DataPacket();
  DataPacket(const uint8_t *data, uint32_t size);

  void SetData(const char *val);

  virtual bool DoesReplyMatch(const uint8_t *buffer, uint32_t size) override { return true; }
  virtual uint32_t HeaderSize() const override { return m_size; }
  virtual std::string GetName() const override { return "Data"; }
  virtual PacketType GetPacketType() const override { return PacketType::DATA; }

protected:
  virtual void DoWriteToBuf(uint8_t *buffer) override;

private:
  std::vector<uint8_t> m_data;
  uint32_t m_size;
};

}// namespace PacketHacker
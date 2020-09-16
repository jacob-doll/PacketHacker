#pragma once

#include "packet.h"
#include <vector>

namespace PacketHacker {

class DataPacket : public Packet
{
public:
  DataPacket();
  DataPacket(const uint8_t *data, uint32_t size);

  void data(const std::vector<uint8_t> &data);

  virtual const PacketType packetType() const override { return PacketType::DATA; }
  virtual const std::string name() const override { return "Data"; }
  virtual const uint32_t headerSize() const override { return 0; }
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) override { return true; }

protected:
  virtual void doWriteToBuf(uint8_t *buffer) override;

private:
  std::vector<uint8_t> m_data;
  uint32_t m_size;
};

}// namespace PacketHacker
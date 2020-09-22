#pragma once

#include "packet.h"
#include <vector>

namespace PacketHacker {

/**
 * \brief Class representation of raw data.
 * 
 * This is the default packet type if no other is specified.
 */
class DataPacket : public Packet
{
public:
  /**
   * \brief Default constructor.
   */
  DataPacket();

  /**
   * \brief Inititializes data from a buffer.
   * 
   * This will instanstiate a vector with the size of the buffer
   * and its contents.
   * 
   * @param data buffer to read from
   * @param size size of buffer
   */
  DataPacket(const uint8_t *data, uint32_t size);

  /**
   * \brief Returns the data in a byte vector.
   * @return vector of data 
   */
  std::vector<uint8_t> &data() { return m_data; }


  /**
   * \brief Returns the packet's type.
   * @return PacketType::DATA
   */
  virtual const PacketType packetType() const override { return PacketType::DATA; }

  /**
   * \brief Returns the packet's name.
   * @return Data
   */
  virtual const std::string name() const override { return "Data"; }

  /**
   * \brief Returns the header size of the packet.
   * Returns 0 as there is no header.
   * @return 0
   */
  virtual const uint32_t headerSize() const override { return 0; }

  /**
   * \brief Returns if a packet is the reply to this packet.
   * 
   * Will always return true as there is no default reply.
   * @param buffer data of the reply
   * @param size size of the reply buffer
   * @return true
   */
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) override { return true; }

protected:
  /**
   * Writes the contents of this packet to the buffer.
   * @param buffer data buffer to write to
   */
  virtual void doWriteToBuf(uint8_t *buffer) override;

private:
  std::vector<uint8_t> m_data;
  uint32_t m_size;
};

}// namespace PacketHacker
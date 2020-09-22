#pragma once

#include "packet.h"

#include "utils/buffer_utils.h"

namespace PacketHacker {

/**
 * \brief Class representation of an ICMP packet.
 */
class IcmpPacket : public Packet
{
public:
  /**
   * \brief Default constructor.
   * Fills packet with zereos.
   */
  IcmpPacket();

  /**
   * \brief Initializes ICMP packet from a data buffer.
   * 
   * If size is less than the sizeof(IcmpHeader) then an error will be thrown.
   * The packet will only read the first sizeof(IcmpHeader) bytes from the
   * buffer, the rest is ignored.
   * @param data buffer to read from
   * @param size size of buffer
   */
  IcmpPacket(const uint8_t *data, uint32_t size);

  /**
   * \brief Returns the ICMP type.
   * @return ICMP type
   */
  uint8_t type() { return m_header.type; }

  /**
   * \brief Returns the ICMP code.
   * @return ICMP code
   */
  uint8_t code() { return m_header.code; }

  /**
   * \brief Returns the checksum.
   * @return checksum
   */
  uint16_t checksum() { return BYTE_SWAP_16(m_header.checksum); }

  /**
   * \brief Returns the ICMP data.
   * @return ICMP data
   */
  uint32_t data() { return BYTE_SWAP_32(m_header.data); }

  /**
   * \brief Sets the ICMP type.
   * @param type ICMP type
   */
  void type(const uint8_t type);

  /**
   * \brief Sets the ICMP code.
   * @param code ICMP code
   */
  void code(const uint8_t code);

  /**
   * \brief Sets the checksum.
   * \warning checksum should not be set manually.
   * @param checksum checksum
   */
  void checksum(const uint16_t checksum);

  /**
   * \brief Sets the ICMP data.
   * @param data ICMP data
   */
  void data(const uint32_t data);

  /**
   * \brief Returns the packet's type.
   * @return PacketType::ICMP
   */
  virtual const PacketType packetType() const override { return PacketType::ICMP; }

  /**
   * \brief Returns the packet's name.
   * @return ICMP
   */
  virtual const std::string name() const override { return "ICMP"; }

  /**
   * \brief Returns the header size of the packet.
   * @return sizeof(IcmpHeader)
   */
  virtual const uint32_t headerSize() const override { return sizeof(IcmpHeader); }

  /**
   * \brief Returns if a packet is the reply to this packet.
   * 
   * Size must be at least sizeof(IcmpHeader)
   * @param buffer data of the reply
   * @param size size of the reply buffer
   * @return true if the buffer contains a reply, false otherwise
   */
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) override;

protected:
  /**
   * Writes the contents of this packet to the buffer.
   * @param buffer data buffer to write to
   */
  virtual void doWriteToBuf(uint8_t *buffer) override;

private:
#pragma pack(push, 1)
  struct IcmpHeader
  {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t data;
  };
#pragma pack(pop)

  IcmpHeader m_header;
};

}// namespace PacketHacker
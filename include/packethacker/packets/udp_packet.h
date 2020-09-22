#pragma once

#include "packet.h"

#include "utils/buffer_utils.h"

namespace PacketHacker {

/**
 * \brief Class representation of an UDP packet.
 */
class UdpPacket : public Packet
{
public:
  /**
   * \brief Default constructor.
   * 
   * Fills entire packet with zeroes.
   */
  UdpPacket();

  /**
   * \brief Initializes Ethernet packet from a data buffer.
   * 
   * If size is less than the sizeof(UdpHeader) then an error will be thrown.
   * The packet will only read the first sizeof(UdpHeader) bytes from the
   * buffer, the rest is ignored.
   * @param data buffer to read from
   * @param size size of buffer
   */
  UdpPacket(const uint8_t *data, uint32_t size);

  /**
   * \brief Returns the source port.
   * @return source port
   */
  uint16_t srcPort() { return BYTE_SWAP_16(m_header.srcPort); }

  /**
   * \brief Returns the destination port.
   * @return destination port
   */
  uint16_t dstPort() { return BYTE_SWAP_16(m_header.dstPort); }

  /**
   * \brief Returns the payload length.
   * @return payload length
   */
  uint16_t length() { return BYTE_SWAP_16(m_header.length); }

  /**
   * \brief Returns the checksum.
   * \warning This may not reflect the actual current checksum of the packet.
   * @return protocol
   */
  uint16_t checksum() { return BYTE_SWAP_16(m_header.checksum); }


  /**
   * \brief Sets the source port.
   * @param srcPort source port
   */
  void srcPort(const uint16_t srcPort);

  /**
   * \brief Sets the destination port.
   * @param dstPort destination port
   */
  void dstPort(const uint16_t dstPort);

  /**
   * \brief Sets the length of the payload.
   * \warning Do not set this manually.
   * @param length destination port
   */
  void length(const uint16_t length);

  /**
   * \brief Sets the checksum.
   * \warning Do not set this manually.
   * @param checksum checksum
   */
  void checksum(const uint16_t checksum);

  /**
   * \brief Returns the packet's type.
   * @return PacketType::UDP
   */
  virtual const PacketType packetType() const override { return PacketType::UDP; }

  /**
   * \brief Returns the packet's name.
   * @return UDP
   */
  virtual const std::string name() const override { return "UDP"; }

  /**
   * \brief Returns the header size of the packet.
   * @return sizeof(UdpHeader)
   */
  virtual const uint32_t headerSize() const override { return sizeof(UdpHeader); }

  /**
   * \brief Returns if a packet is the reply to this packet.
   * 
   * True if reply's destination port equals the source port and vice
   * versa for the reply's source port and the destination port. 
   * 
   * Size must be at least sizeof(UdpHeader)
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
  struct UdpHeader
  {
    uint16_t srcPort;
    uint16_t dstPort;
    uint16_t length;
    uint16_t checksum;
  };
#pragma pack(pop)

  UdpHeader m_header;
};

}// namespace PacketHacker
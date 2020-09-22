#pragma once

#include "hardware_address.h"
#include "utils/buffer_utils.h"

#include "packet.h"

namespace PacketHacker {

/**
 * \brief Class representation of an EthernetII packet.
 */
class EthernetPacket : public Packet
{
public:
  /**
   * \brief Default constructor.
   * Fills packet with zereos.
   */
  EthernetPacket();

  /**
   * \brief Initializes Ethernet packet from a data buffer.
   * 
   * If size is less than the sizeof(EthernetHeader) then an error will be thrown.
   * The packet will only read the first sizeof(EthernetHeader) bytes from the
   * buffer, the rest is ignored.
   * @param data buffer to read from
   * @param size size of buffer
   */
  EthernetPacket(const uint8_t *data, uint32_t size);

  /**
   * \brief Returns the destination hardware address.
   * @return destination hardware address
   */
  HardwareAddress dstMac() { return HardwareAddress(m_header.dstMac); }

  /**
   * \brief Returns the source hardware address.
   * @return source hardware address
   */
  HardwareAddress srcMac() { return HardwareAddress(m_header.srcMac); }

  /**
   * \brief Returns the next protocol type.
   * @return protocol type
   */
  uint16_t type() { return BYTE_SWAP_16(m_header.type); }

  /**
   * \brief Sets the destination hardware address.
   * @param dst destination hardware address
   */
  void dstMac(const HardwareAddress &dst);

  /**
   * \brief Sets the source hardware address.
   * @param src source hardware address
   */
  void srcMac(const HardwareAddress &src);

  /**
   * \brief Sets the protocol type.
   * @param type protocol type
   */
  void type(const uint16_t type);

  /**
   * \brief Returns the packet's type.
   * @return PacketType::ETHERNET
   */
  virtual const PacketType packetType() const override { return PacketType::ETHERNET; }

  /**
   * \brief Returns the packet's name.
   * @return Ethernet
   */
  virtual const std::string name() const override { return "Ethernet"; }

  /**
   * \brief Returns the header size of the packet.
   * @return sizeof(EthernetHeader)
   */
  virtual const uint32_t headerSize() const override { return sizeof(EthernetHeader); }

  /**
   * \brief Returns if a packet is the reply to this packet.
   * 
   * True if reply's dst mac equals the src mac and vice
   * versa for the reply's src mac and the dst mac. Type must
   * also be the same.
   * 
   * Size must be at least sizeof(EthernetHeader)
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
  struct EthernetHeader
  {
    uint8_t dstMac[6];
    uint8_t srcMac[6];
    uint16_t type;
  };

  EthernetHeader m_header;
};

}// namespace PacketHacker
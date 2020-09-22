#pragma once
#include <stdint.h>
#include <string>


namespace PacketHacker {

/**
 * \brief Packet container class representing all data for a given packet.
 * 
 * All packet types are sub-classed from this class.
 * 
 * The supported packets are as follows:
 *   - ArpPacket
 *   - EthernetPacket
 *   - IcmpPacket
 *   - IpPacket
 *   - UdpPacket
 * 
 * As of right now the base packet for reading from an interface are 
 * EthernetPacket (s). Support for more packet types are coming soon.
 * 
 * Each packet type has their own initializers, yet each packet can be
 * instantiated from a buffer.
 * 
 *   - e.g. Packet *p = new EthernetPacket({...}, size);
 */
class Packet
{
public:
  /**
   * \brief Available packet types.
   */
  enum PacketType {
    ARP,
    ETHERNET,
    IP,
    ICMP,
    DATA,
    UDP
  };

  /**
   * \brief Default constructor.
   */
  Packet();

  /**
   * \brief Virtual destructor that defaults to nothing.
   * 
   * If a sub-class needs to destroy data then it
   * overrides this function.
   */
  virtual ~Packet();

  /**
   * \brief Pointer to the parent packet.
   * 
   * If top-level packet then outerPacket returns nullptr.
   * @return Pointer of outer packet
   */
  Packet *outerPacket() const { return m_outerPacket; }

  /**
   * \brief Returns a pointer to the next child packet.
   * 
   * If bottom-level packet then innerPacket returns nullptr.
   * @return Pointer of inner packet
   */
  Packet *innerPacket() const { return m_innerPacket; }

  /**
   * \brief Returns packet in this structure with the given name.
   * 
   * Attempts to find a packet with the given name in
   * the packet structure. If no packet can be found 
   * then nullptr is returned.
   * @param name string name of packet to look for.
   * @return Packet of the given name
   */
  Packet *getPacket(const std::string &name); /** TODO: Use packet class to find instead of packet name. */

  /**
   * \brief Sets the inner packet.
   * @param inner Pointer to packet object
   */
  void innerPacket(Packet *inner);

  /**
   * \brief Removes child packet and all other children, deleting them.
   */
  void removeInnerPacket();

  /**
   * \brief Returns the total size of the packet in bytes.
   * @return Size of packet
   */
  const uint32_t size() const;

  /**
   * \brief Writes the entire packet and children to a supplied buffer.
   * 
   * Will overwrite and data in the buffer of the given size.
   * @param buffer buffer to write into.
   * @param size size in bytes to write.
   */
  void writeToBuf(uint8_t *buffer, const uint32_t size);

  /**
   * \brief Returns the packet's type.
   * @return PacketType of this packet
   */
  virtual const PacketType packetType() const = 0;

  /**
   * \brief Returns the packet's name.
   * @return string name of the packet
   */
  virtual const std::string name() const = 0;

  /**
   * \brief Returns the header size of the packet.
   * @return size of header in bytes
   */
  virtual const uint32_t headerSize() const = 0;

  /**
   * \brief Returns if a packet is the reply to this packet.
   * 
   * If the buffer has enough capacity and represents
   * the reply to this packet then true is returned.
   * @param buffer data of the reply
   * @param size size of the reply buffer
   * @return true if the buffer contains a reply, false otherwise
   */
  virtual bool doesReplyMatch(const uint8_t *buffer, uint32_t size) = 0;

  /**
   * \brief Prints the packet to a stream.
   * @param output stream to print to
   * @param packet packet to print
   * @return stream that was written to
   */
  friend std::ostream &operator<<(std::ostream &output, Packet *packet);

protected:
  /**
   * Writes just this packet to the buffer.
   * @param buffer data buffer to write to
   */
  virtual void doWriteToBuf(uint8_t *buffer) = 0;

private:
  /**
   * Sets the parent packet of this packet.
   * @param outer parent packet
   */
  void outerPacket(Packet *outer);

private:
  Packet *m_outerPacket;
  Packet *m_innerPacket;
};

}// namespace PacketHacker

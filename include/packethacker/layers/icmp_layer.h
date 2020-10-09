#pragma once

#include "layer.h"

#include "utils/buffer_utils.h"
#include "ip_address.h"

namespace PacketHacker {

/**
 * @brief Class representation of an ICMP layer.
 */
class IcmpLayer : public Layer
{
public:
  /**
   * Declares the layer type of this layer.
   */
  DECL_LAYER(LayerType::ICMP);

  /**
   * @brief Support ICMP types.
   */
  enum Type {
    ECHO_REPLY = 0,
    DESTINATION_UNREACHABLE = 3,
    SOURCE_QUENCH = 4,
    REDIRECT_MESSAGE = 5,
    ECHO_REQUEST = 8,
    TIME_EXCEEDED = 11,
    TIMESTAMP = 13,
    TIMESTAMP_REPLY = 14,
    ADDRESS_MASK_REQUEST = 17,
    ADDRESS_MASK_REPLY = 18,
  };

  /**
   * @brief Default constructor.
   * Initializes header with zero values.
   */
  IcmpLayer();

  /**
   * @brief Initializes ICMP layer from a data buffer.
   * 
   * If size is less than the sizeof(IcmpHeader) then an error will be thrown.
   * The layer will only read the first sizeof(IcmpHeader) bytes from the
   * buffer, the rest is ignored.
   * @param data buffer to read from
   * @param size size of buffer
   */
  IcmpLayer(const uint8_t *data, uint32_t size);

  /**
   * @brief Returns the ICMP type.
   * @return ICMP type
   */
  uint8_t icmpType() { return m_header.icmpType; }

  /**
   * @brief Returns the ICMP code.
   * @return ICMP code
   */
  uint8_t code() { return m_header.code; }

  /**
   * @brief Returns the checksum.
   * @return checksum
   */
  uint16_t checksum() { return BYTE_SWAP_16(m_header.checksum); }

  /**
   * @brief Returns the identifier.
   * The id field is only set for ICMP echo, timestamp, and
   * address mask messages.
   * @return identifier
   */
  uint16_t id() { return BYTE_SWAP_16(m_header.data.id); }

  /**
   * @brief Returns the sequence number.
   * The sequence number field is only set for ICMP echo, timestamp,
   * and address mask messages.
   * @return sequence number
   */
  uint16_t sequence() { return BYTE_SWAP_16(m_header.data.sequence); }

  /**
   * @brief Returns the next hop mtu for destination unreachable messages.
   * @return next hop mtu
   */
  uint16_t nextHopMtu() { return BYTE_SWAP_16(m_header.data.nextHopMtu); }

  /**
   * @brief Returns the gateway ip address for redirect messages.
   * @return gateway ip
   */
  IPv4Address gateway() { return IPv4Address(m_header.data.gateway); }

  /**
   * @brief Returns the originate timestamp for timestamp messages.
   * @return originate timestamp
   */
  uint32_t originateTimestamp() { return BYTE_SWAP_32(m_originateTimestamp); }

  /**
   * @brief Returns the receive timestamp for timestamp messages.
   * This field is unused for timestamp requests.
   * @return receive timestamp
   */
  uint32_t receiveTimestamp() { return BYTE_SWAP_32(m_receiveTimestamp); }

  /**
   * @brief Returns the trasmit timestamp for timestamp messages.
   * This field is unused for timestamp requests.
   * @return trasmit timestamp
   */
  uint32_t transmitTimestamp() { return BYTE_SWAP_32(m_transmitTimestamp); }

  /**
   * @brief Returns the address mask for address mask messages.
   * @return address mask
   */
  uint32_t addressMask() { return BYTE_SWAP_32(m_addressMask); }

  /**
   * @brief Sets the ICMP type.
   * @param icmpType ICMP type
   */
  void icmpType(const uint8_t icmpType);

  /**
   * @brief Sets the ICMP code.
   * @param code ICMP code
   */
  void code(const uint8_t code);

  /**
   * @brief Sets the checksum.
   * @warning checksum should not be set manually.
   * @param checksum checksum
   */
  void checksum(const uint16_t checksum);

  /**
   * @brief Sets the identifier.
   * The id field is only set for ICMP echo, timestamp, and
   * address mask messages.
   * @param id the identifier
   */
  void id(const uint16_t id);

  /**
   * @brief Sets the sequence number.
   * The sequence number field is only set for ICMP echo, timestamp,
   * and address mask messages.
   * @param sequence the sequence number
   */
  void sequence(const uint16_t sequence);

  /**
   * @brief Sets the next hop mtu for destination unreachable messages.
   * @param nextHopMtu next hop mtu
   */
  void nextHopMtu(const uint16_t nextHopMtu);

  /**
   * @brief Sets the gateway ip address for redirect messages.
   * @param gateway gateway ip
   */
  void gateway(const IPv4Address &gateway);

  /**
   * @brief Sets the originate timestamp for timestamp messages.
   * @param originateTimestamp originate timestamp
   */
  void originateTimestamp(const uint32_t originateTimestamp);

  /**
   * @brief Sets the receive timestamp for timestamp messages.
   * This field is unused for timestamp requests.
   * @param receiveTimestamp receive timestamp
   */
  void receiveTimestamp(const uint32_t receiveTimestamp);

  /**
   * @brief Sets the trasmit timestamp for timestamp messages.
   * This field is unused for timestamp requests.
   * @param transmitTimestamp trasmit timestamp
   */
  void transmitTimestamp(const uint32_t transmitTimestamp);

  /**
   * @brief Sets the address mask for address mask messages.
   * @param addressMask address mask
   */
  void addressMask(const IPv4Address &addressMask);


  /**
   * @brief Returns the layer's type.
   * @return LayerType::ICMP
   * @sa Layer::type()
   */
  virtual const LayerType type() const override { return typeFlag; }

  /**
   * @brief Returns the header size of the layer.
   * @return sizeof(IcmpHeader)
   * @sa Layer::headerSize()
   */
  virtual const uint32_t headerSize() const override;//{ return sizeof(IcmpHeader); }

  /**
   * @brief Returns if a buffer contains the reply to this layer.
   * 
   * Size must be at least sizeof(IcmpHeader)
   * @param buffer data of the reply
   * @param size size of the reply buffer
   * @return true if the buffer contains a reply, false otherwise
   * @sa Layer::isReply()
   */
  virtual bool isReply(const uint8_t *buffer, uint32_t size) override;

  /**
   * @brief Writes the contents of this layer to the buffer.
   * @param buffer data buffer to write to
   * @sa Layer::write()
   */
  virtual void write(uint8_t *buffer) override;

private:
#pragma pack(push, 1)
  struct IcmpHeader
  {
    uint8_t icmpType;
    uint8_t code;
    uint16_t checksum;
    union {
      struct
      {
        uint16_t id;
        uint16_t sequence;
      };
      struct
      {
        uint16_t unused;
        uint16_t nextHopMtu;
      };
      uint32_t gateway;

    } data;
  };
#pragma pack(pop)

  IcmpHeader m_header;
  uint32_t m_originateTimestamp;
  uint32_t m_receiveTimestamp;
  uint32_t m_transmitTimestamp;
  uint32_t m_addressMask;
};

}// namespace PacketHacker
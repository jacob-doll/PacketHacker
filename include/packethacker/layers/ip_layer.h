#pragma once

#include "layer.h"

#include "ip_address.h"
#include "utils/buffer_utils.h"

namespace PacketHacker {

/**
 * @brief Class representation of an IPv4 layer.
 */
class IpLayer : public Layer
{
public:
  /**
   * Declares the layer type of this layer.
   */
  DECL_LAYER(LayerType::IP);

  /**
   * @brief Default constructor.
   * 
   * Initializes header with zero values.
   */
  IpLayer();

  /**
   * @brief Initializes IP layer from a data buffer.
   * 
   * If size is less than the sizeof(IpHeader) then an error will be thrown.
   * The layer will only read the first sizeof(IpHeader) bytes from the
   * buffer, the rest is ignored.
   * @param data buffer to read from
   * @param size size of buffer
   */
  IpLayer(const uint8_t *data, uint32_t size);

  /**
   * @brief Returns the version, default is 4.
   * @return version
   */
  uint8_t version() { return m_header.version; }

  /**
   * @brief Returns the IP header length.
   * @return header length
   */
  uint8_t headerLength() { return m_header.headerLength; }

  /**
   * @brief Returns the IP differentiated services.
   * @return header length
   */
  uint8_t diffServices() { return m_header.diffServices; }

  /**
   * @brief Returns the total length of the IP layer and its payload.
   * @warning This may not reflect the actual current length of the layer.
   * @return header length
   */
  uint16_t totalLength() { return BYTE_SWAP_16(m_header.totalLength); }

  /**
   * @brief Returns the id number of IP layer.
   * @return id
   */
  uint16_t id() { return BYTE_SWAP_16(m_header.id); }

  /**
   * @brief Returns the layer flags.
   * @return flags
   */
  uint16_t flags() { return BYTE_SWAP_16(m_header.flags); }

  /**
   * @brief Returns the fragment offset.
   * @return fragment offset
   */
  uint16_t fragOffset() { return BYTE_SWAP_16(m_header.fragOffset); }

  /**
   * @brief Returns the time to live.
   * @return time to live
   */
  uint8_t ttl() { return m_header.ttl; }

  /**
   * @brief Returns the next protocol.
   * @return protocol
   */
  uint8_t protocol() { return m_header.protocol; }

  /**
   * @brief Returns the checksum.
   * @warning This may not reflect the actual current checksum of the layer.
   * @return protocol
   */
  uint16_t checksum() { return BYTE_SWAP_16(m_header.checksum); }

  /**
   * @brief Returns the source IP address.
   * @return source IP address
   */
  IPv4Address sourceIp() { return IPv4Address(m_header.sourceIp); }

  /**
   * @brief Returns the destination IP address.
   * @return destination IP address
   */
  IPv4Address destIp() { return IPv4Address(m_header.destIp); }

  /**
   * @brief Sets the version.
   * @warning Do not set this to something that is not IPv4.
   * @param version IP version
   */
  void version(const uint8_t version);

  /**
   * @brief Sets the header length.
   * @param headerLength header length
   */
  void headerLength(const uint8_t headerLength);

  /**
   * @brief Sets the differentiated services.
   * @param diffServices differentiated services
   */
  void diffServices(const uint8_t diffServices);

  /**
   * @brief Sets the total length of the layer.
   * @warning Do not set this manually.
   * @param totalLength total length
   */
  void totalLength(const uint16_t totalLength);

  /**
   * @brief Sets the ID of the layer.
   * @param id ID
   */
  void id(const uint16_t id);

  /**
   * @brief Sets the flags the layer.
   * @param flags flags
   */
  void flags(const uint16_t flags);

  /**
   * @brief Sets the fragment offset of the layer.
   * @param fragOffset fragment offset
   */
  void fragOffset(const uint16_t fragOffset);

  /**
   * @brief Sets the time to live.
   * @param ttl time to live
   */
  void ttl(const uint8_t ttl);

  /**
   * @brief Sets the next layer protocol.
   * @param protocol next layer protocol
   */
  void protocol(const uint8_t protocol);

  /**
   * @brief Sets the checksum.
   * @warning Do not set this manually.
   * @param checksum checksum
   */
  void checksum(const uint16_t checksum);

  /**
   * @brief Sets the source IP address.
   * @param sourceIp source IP address
   */
  void sourceIp(const IPv4Address &sourceIp);

  /**
   * @brief Sets the destination IP address.
   * @param destIp destination IP address
   */
  void destIp(const IPv4Address &destIp);

  /**
   * @brief Returns the layer's type.
   * @return LayerType::IP
   * @sa Layer::type()
   */
  virtual const LayerType type() const override { return typeFlag; }

  /**
   * @brief Returns the header size of the layer.
   * @return sizeof(IpHeader)
   * @sa Layer::headerSize()
   */
  virtual const uint32_t headerSize() const override { return sizeof(IpHeader); }

  /**
   * @brief Returns if a buffer contains the reply to this layer.
   * 
   * True if reply's destination IP equals the source IP and vice
   * versa for the reply's source IP and the destination IP. Protocol
   * must also be the same.
   * 
   * Size must be at least sizeof(IpHeader)
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
  struct IpHeader
  {
    union {
      uint8_t hdrVerLen;
      struct
      {
        uint8_t headerLength : 4, version : 4;
      };
    };
    union {
      uint8_t dscpEcn;
      struct
      {
        uint8_t ecn : 2, diffServices : 6;
      };
    };
    uint16_t totalLength;
    uint16_t id;
    union {
      uint16_t flagsOffset;
      struct
      {
        uint16_t fragOffset : 13, flags : 3;
      };
    };
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t sourceIp;
    uint32_t destIp;
  };
#pragma pack(pop)

  IpHeader m_header;
};

}// namespace PacketHacker

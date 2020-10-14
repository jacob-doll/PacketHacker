#pragma once

#include "layer.h"

#include "utils/buffer_utils.h"

namespace PacketHacker {

/**
 * @brief Class representation of an UDP layer.
 */
class UdpLayer : public Layer
{
public:
  /**
   * Declares the layer type of this layer.
   */
  DECL_LAYER(LayerType::UDP);

  /**
   * @brief Default constructor.
   * 
   * Fills entire layer with zeroes.
   */
  UdpLayer();

  /**
   * @brief Initializes UDP layer from a data buffer.
   * 
   * If size is less than the sizeof(UdpHeader) then an error will be thrown.
   * The layer will only read the first sizeof(UdpHeader) bytes from the
   * buffer, the rest is ignored.
   * @param data buffer to read from
   * @param size size of buffer
   */
  UdpLayer(const uint8_t *data, uint32_t size);

  /**
   * @brief Returns the source port.
   * @return source port
   */
  uint16_t srcPort() { return BYTE_SWAP_16(m_header.srcPort); }

  /**
   * @brief Returns the destination port.
   * @return destination port
   */
  uint16_t dstPort() { return BYTE_SWAP_16(m_header.dstPort); }

  /**
   * @brief Returns the payload length.
   * @return payload length
   */
  uint16_t length() { return BYTE_SWAP_16(m_header.length); }

  /**
   * @brief Returns the checksum.
   * @warning This may not reflect the actual current checksum of the layer.
   * @return protocol
   */
  uint16_t checksum() { return BYTE_SWAP_16(m_header.checksum); }


  /**
   * @brief Sets the source port.
   * @param srcPort source port
   */
  void srcPort(const uint16_t srcPort);

  /**
   * @brief Sets the destination port.
   * @param dstPort destination port
   */
  void dstPort(const uint16_t dstPort);

  /**
   * @brief Sets the length of the payload.
   * @warning Do not set this manually.
   * @param length destination port
   */
  void length(const uint16_t length);

  /**
   * @brief Sets the checksum.
   * @warning Do not set this manually.
   * @param checksum checksum
   */
  void checksum(const uint16_t checksum);

  /**
   * @brief Returns the layer's type.
   * @return LayerType::UDP
   * @sa Layer::type()
   */
  virtual const LayerType type() const override { return typeFlag; }

  /**
   * @brief Returns the header size of the layer.
   * @return sizeof(UdpHeader)
   * @sa Layer::headerSize()
   */
  virtual const SizeType headerSize() const override { return sizeof(UdpHeader); }

  /**
   * @brief Returns if a buffer contains the reply to this layer.
   * 
   * True if reply's destination port equals the source port and vice
   * versa for the reply's source port and the destination port. 
   * 
   * Size must be at least sizeof(UdpHeader)
   * @param buffer data of the reply
   * @param size size of the reply buffer
   * @return true if the buffer contains a reply, false otherwise
   * @sa Layer::isReply()
   */
  virtual bool isReply(const DataType *buffer, SizeType size) override;

  /**
   * @brief Writes the contents of this layer to the buffer.
   * @param buffer data buffer to write to
   * @sa Layer::write()
   */
  virtual void write(DataType *buffer) override;

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
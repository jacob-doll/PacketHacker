#pragma once

#include "hardware_address.h"
#include "utils/buffer_utils.h"

#include "layer.h"

namespace PacketHacker {

/**
 * @brief Class representation of an EthernetII layer.
 */
class EthernetLayer : public Layer
{
public:
  /**
   * Declares the layer type of this layer.
   */
  DECL_LAYER(LayerType::ETHERNET);

  /**
   * @brief Default constructor.
   * Initializes header with zero values.
   */
  EthernetLayer();

  /**
   * @brief Initializes Ethernet layer from a data buffer.
   * 
   * If size is less than the sizeof(EthernetHeader) then an error will be thrown.
   * The layer will only read the first sizeof(EthernetHeader) bytes from the
   * buffer, the rest is ignored.
   * @param data buffer to read from
   * @param size size of buffer
   */
  EthernetLayer(const uint8_t *data, uint32_t size);

  /**
   * @brief Returns the destination hardware address.
   * @return destination hardware address
   */
  HardwareAddress dstMac() { return HardwareAddress(m_header.dstMac); }

  /**
   * @brief Returns the source hardware address.
   * @return source hardware address
   */
  HardwareAddress srcMac() { return HardwareAddress(m_header.srcMac); }

  /**
   * @brief Returns the next protocol type.
   * @return protocol type
   */
  uint16_t protoType() { return BYTE_SWAP_16(m_header.protoType); }

  /**
   * @brief Sets the destination hardware address.
   * @param dst destination hardware address
   */
  void dstMac(const HardwareAddress &dst);

  /**
   * @brief Sets the source hardware address.
   * @param src source hardware address
   */
  void srcMac(const HardwareAddress &src);

  /**
   * @brief Sets the protocol type.
   * @param protoType protocol type
   */
  void protoType(const uint16_t protoType);

  /**
   * @brief Returns the layer's type.
   * @return LayerType::ETHERNET
   * @sa Layer::type()
   */
  virtual const LayerType type() const override { return typeFlag; }

  /**
   * @brief Returns the header size of the layer.
   * @return sizeof(EthernetHeader)
   * @sa Layer::headerSize()
   */
  virtual const SizeType headerSize() const override { return sizeof(EthernetHeader); }

  /**
   * @brief Returns if a buffer contains the reply to this layer.
   * 
   * True if reply's dst mac equals the src mac and vice
   * versa for the reply's src mac and the dst mac. Type must
   * also be the same.
   * 
   * Size must be at least sizeof(EthernetHeader)
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
  struct EthernetHeader
  {
    uint8_t dstMac[6];
    uint8_t srcMac[6];
    uint16_t protoType;
  };

  EthernetHeader m_header;
};

}// namespace PacketHacker
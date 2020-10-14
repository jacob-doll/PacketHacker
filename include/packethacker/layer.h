#pragma once

#include "common.h"

namespace PacketHacker {

/**
 * @brief Container object for a layer inside a packet.
 * 
 * Layers are linked together to form the entirety of a
 * packet. Each layer has a reference to the inner layer
 * and outer layer. If the top-most layer then outer layer
 * is null, if bottom-most then the inner layer is null.  
 * 
 * The supported layers are as follows:
 *   - EthernetII (Layer 2)
 *   - ARP (Layer 2/3)
 *   - IP (Layer 3)
 *   - ICMP (Layer 3/4)
 *   - UDP (Layer 4)
 * 
 * Layers are added to packet objects which are the base
 * container for all packet manipulation. 
 */
class Layer
{
public:
  /**
   * @brief Available packet types.
   */
  enum class LayerType {
    ETHERNET,
    ARP,
    IP,
    ICMP,
    UDP,
    DATA
  };

  /**
   * @brief Default constructor.
   */
  Layer();

  /**
   * @brief Default destructor, deletes the inner layer.
   */
  virtual ~Layer();

  /**
   * @brief Pointer to the layer before this one.
   * 
   * If top-most layer then outerLayer returns nullptr.
   * @return Pointer of outer layer
   */
  Layer *outerLayer() const { return m_outerLayer; }

  /**
   * @brief Pointer to the layer after this one.
   * 
   * If bottom-most layer then innerLayer returns nullptr.
   * @return Pointer of inner layer
   */
  Layer *innerLayer() const { return m_innerLayer; }

  /**
   * @brief Sets the inner layer.
   * @param layer Pointer to layer object
   */
  void innerLayer(Layer *layer);

  /**
   * @brief Returns the layer's type.
   * @return Layer type of this packet
   */
  virtual const LayerType type() const = 0;

  /**
   * @brief Returns the header size of the layer.
   * @return size of header in bytes
   */
  virtual const SizeType headerSize() const = 0;

  /**
   * @brief Returns if a buffer contains the reply to this layer.
   * 
   * If the buffer has enough capacity and represents
   * the reply to this layer then true is returned.
   * @param data data of the reply
   * @param size size of the reply buffer
   * @return true if the buffer contains a reply, false otherwise
   */
  virtual bool isReply(const DataType *data, const SizeType size) = 0;

  /**
   * @brief Writes this layer to a buffer.
   * @param buffer buffer to write to
   */
  virtual void write(DataType *buffer) = 0;

private:
  /**
   * @brief Sets the outer layer of this layer.
   * @param layer outer layer
   */
  void outerLayer(Layer *layer);

private:
  Layer *m_outerLayer;
  Layer *m_innerLayer;
};

#define DECL_LAYER(type) static const Layer::LayerType typeFlag = type;

}// namespace PacketHacker
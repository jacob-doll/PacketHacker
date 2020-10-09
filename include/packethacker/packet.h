#pragma once

#include "layer.h"

namespace PacketHacker {

/**
 * @brief Packet container class representing all data for a given packet.
 * 
 * Packets encompass layers which hold the data for the packet. The packet
 * is a container object that manipulates layers.
 * 
 * The supported layers are as follows:
 *   - EthernetII (Layer 2)
 *   - ARP (Layer 2/3)
 *   - IP (Layer 3)
 *   - ICMP (Layer 3/4)
 *   - UDP (Layer 4)
 * 
 * Packets can be initialized from a buffer of specific size or by manually
 * adding layers to the packet.
 * 
 * e.g. \n Packet packet; \n packet.insertLayer(new EthernetLayer);
 * 
 */
class Packet
{
public:
  /**
   * @brief Default constructor.
   */
  Packet();

  /**
   * @brief Initializes a packet from buffer and size.
   * 
   * Currently the base layer is EthernetLayer but soon the
   * base layer type will be determined from the interface.
   * 
   * @param data data buffer to read from
   * @param size size of data buffer
   */
  Packet(const uint8_t *data, uint32_t size);

  /**
   * @brief Virtual destructor that defaults to nothing.
   * 
   * If a sub-class needs to destroy data then it
   * overrides this function.
   */
  ~Packet();

  /**
   * @brief Returns first layer.
   * @return first layer
   */
  Layer *firstLayer() const { return m_firstLayer; }

  /**
   * @brief Returns last layer.
   * If only one layer exists both first and last layer
   * will point to the same layer.
   * @return last layer
   */
  Layer *lastLayer() const { return m_lastLayer; }

  /**
   * @brief Returns a layer of the given type supplied.
   * @param type layer type to be returned
   * @return layer of the type specified or null if not found
   */
  template<typename TLayer>
  TLayer *getLayer(Layer::LayerType type = TLayer::typeFlag) const
  {
    Layer *curr = m_firstLayer;
    while (curr) {
      if (curr->type() == type) {
        return static_cast<TLayer *>(curr);
      }
      curr = curr->innerLayer();
    }
    return nullptr;
  }

  /**
   * @brief Inserts a new layer at the end.
   * @param layer layer to be inserted
   */
  void insertLayer(Layer *layer);

  /**
   * @brief Removes the layer of the specified type and all
   * layers below it.
   * @param type type of layer to remove
   */
  void RemoveLayer(Layer::LayerType type);

  /**
   * @brief Returns the total size of the packet in bytes.
   * @return Size of packet
   */
  const uint32_t size() const;

  /**
   * @brief Returns if a buffer contains the reply to this packet.
   * 
   * If the buffer has enough capacity and represents
   * the reply to this packet then true is returned.
   * @param data data of the reply
   * @param size size of the reply buffer
   * @return true if the buffer contains a reply, false otherwise
   */
  bool isReply(const uint8_t *data, uint32_t size);

  /**
   * @brief Writes the entire packet and children to a supplied buffer.
   * 
   * Will overwrite and data in the buffer of the given size.
   * @param buffer buffer to write into.
   * @param size size in bytes to write.
   */
  void writeToBuf(uint8_t *buffer, const uint32_t size);

private:
  Layer *m_firstLayer;
  Layer *m_lastLayer;
};

}// namespace PacketHacker

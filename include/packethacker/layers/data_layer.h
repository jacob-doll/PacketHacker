#pragma once

#include <vector>

#include "layer.h"

namespace PacketHacker {

/**
 * @brief Class representation of raw data.
 * 
 * This is the default layer type if no other is specified.
 */
class DataLayer : public Layer
{
public:
  /**
   * Declares the layer type of this layer.
   */
  DECL_LAYER(LayerType::DATA);

  /**
   * @brief Default constructor.
   */
  DataLayer();

  /**
   * @brief Inititializes data from a buffer.
   * 
   * This will instanstiate a vector with the size of the buffer
   * and its contents.
   * 
   * @param data buffer to read from
   * @param size size of buffer
   */
  DataLayer(const uint8_t *data, uint32_t size);

  /**
   * @brief Returns the data in a byte vector.
   * @return vector of data 
   */
  std::vector<uint8_t> &data() { return m_data; }


  /**
   * @brief Returns the layer's type.
   * @return LayerType::DATA
   * @sa Layer::type()
   */
  virtual const LayerType type() const override { return typeFlag; }

  /**
   * @brief Returns the header size of the layer.
   * Returns 0 as there is no header.
   * @return 0
   * @sa Layer::headerSize()
   */
  virtual const uint32_t headerSize() const override { return 0; }

  /**
   * @brief Returns if a buffer contains the reply to this layer.
   * 
   * Will always return true as there is no default reply.
   * @param buffer data of the reply
   * @param size size of the reply buffer
   * @return true
   * @sa Layer::isReply()
   */
  virtual bool isReply(const uint8_t *buffer, uint32_t size) override { return true; }

  /**
   * @brief Writes the contents of this layer to the buffer.
   * @param buffer data buffer to write to
   * @sa Layer::write()
   */
  virtual void write(uint8_t *buffer) override;

private:
  std::vector<uint8_t> m_data;
  uint32_t m_size;
};

}// namespace PacketHacker
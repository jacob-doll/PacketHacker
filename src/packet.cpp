#include "packet.h"

#include "layers/eth_layer.h"

namespace PacketHacker {

Packet::Packet()
  : m_firstLayer(), m_lastLayer()
{
}

Packet::Packet(const uint8_t *data, uint32_t size)
  : Packet()
{
  m_firstLayer = new EthernetLayer(data, size);
  m_lastLayer = m_firstLayer;
  Layer *curr = m_firstLayer;
  while (curr->innerLayer()) {
    curr = curr->innerLayer();
  }
  m_lastLayer = curr;
}


Packet::~Packet()
{
  delete m_firstLayer;
  m_firstLayer = 0;
}

void Packet::insertLayer(Layer *layer)
{
  if (!m_firstLayer) {
    m_firstLayer = layer;
    m_lastLayer = layer;
  } else {
    m_lastLayer->innerLayer(layer);
    m_lastLayer = layer;
  }
}

void Packet::RemoveLayer(Layer::LayerType type)
{
}

const uint32_t Packet::size() const
{
  uint32_t size = 0;
  Layer *curr = m_firstLayer;
  while (curr) {
    size += curr->headerSize();
    curr = curr->innerLayer();
  }
  return size;
}

bool Packet::isReply(const uint8_t *data, uint32_t size)
{
  Layer *curr = m_firstLayer;
  uint32_t offset = 0;
  while (curr) {
    if (!curr->isReply(data + offset, size)) {
      return false;
    }
    offset += curr->headerSize();
    size -= curr->headerSize();
    curr = curr->innerLayer();
  }
  return true;
}

void Packet::writeToBuf(uint8_t *buffer, const uint32_t size)
{
  uint32_t packetSize = this->size();
  if (size < packetSize)
    return;

  Layer *curr = m_lastLayer;
  uint32_t offset = size;
  while (curr) {
    offset -= curr->headerSize();
    curr->write(buffer + offset);
    curr = curr->outerLayer();
  }
}

}// namespace PacketHacker
#include "packet.h"

#include "layers/eth_layer.h"

namespace PacketHacker {

Packet::Packet()
  : m_firstLayer(), m_lastLayer(), m_payload()
{
}

Packet::Packet(const DataType *data, SizeType size)
  : Packet()
{
  m_firstLayer = new EthernetLayer(data, size);
  m_lastLayer = m_firstLayer;
  Layer *curr = m_firstLayer;
  while (curr->innerLayer()) {
    curr = curr->innerLayer();
  }
  m_lastLayer = curr;
  SizeType offset = this->size();
  size = size - offset;
  if (size > 0) {
    m_payload = new RawPayload(data + offset, size);
  }
}


Packet::~Packet()
{
  delete m_firstLayer;
  m_firstLayer = 0;
  delete m_payload;
  m_payload = 0;
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

void Packet::payload(Payload *payload)
{
  delete m_payload;
  m_payload = payload;
}

const SizeType Packet::size() const
{
  SizeType size = 0;
  Layer *curr = m_firstLayer;
  while (curr) {
    size += curr->headerSize();
    curr = curr->innerLayer();
  }
  if (m_payload) {
    size += m_payload->payloadSize();
  }
  return size;
}

bool Packet::isReply(const DataType *data, SizeType size)
{
  Layer *curr = m_firstLayer;
  SizeType offset = 0;
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

void Packet::writeToBuf(DataType *buffer, const SizeType size)
{
  SizeType packetSize = this->size();
  if (size < packetSize)
    return;

  Layer *curr = m_lastLayer;
  SizeType offset = size;

  if (m_payload) {
    offset -= m_payload->payloadSize();
    m_payload->write(buffer + offset);
  }

  while (curr) {
    offset -= curr->headerSize();
    curr->write(buffer + offset);
    curr = curr->outerLayer();
  }
}

}// namespace PacketHacker
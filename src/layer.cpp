#include "layer.h"
#include "constants.h"

namespace PacketHacker {

Layer::Layer()
  : m_innerLayer(), m_outerLayer()
{}

Layer::~Layer()
{
  delete m_innerLayer;
  m_innerLayer = 0;
}

void Layer::innerLayer(Layer *layer)
{
  delete m_innerLayer;
  m_innerLayer = layer;
  if (m_innerLayer) {
    m_innerLayer->outerLayer(this);
  }
}

void Layer::outerLayer(Layer *layer)
{
  m_outerLayer = layer;
}

}// namespace PacketHacker
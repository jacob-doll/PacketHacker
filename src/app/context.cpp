#include "context.h"
#include <pcap.h>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


namespace PacketHacker {

Packet *GetPacketFromId(int packetId)
{
  switch (packetId) {
  case PacketTypes::ARP:
    return new ArpPacket();
    break;
  case PacketTypes::ETHERNET:
    return new EthernetPacket();
    break;
  default:
    return nullptr;
    break;
  }
}

Context::Context(MainWindow *window)
  : m_CurrentAdapter{},
    m_AdapterSet(false),
    m_pBasePacket(),
    m_MainWindow(window)
{
  m_adapters = Utils::GetAdapters();
}

Context::~Context()
{
  delete m_pBasePacket;
}

void Context::SetAdapter(int index)
{
  m_AdapterSet = true;
  m_CurrentAdapter = m_adapters[index];
}

void Context::SetBasePacket(int packetId)
{
  delete m_pBasePacket;
  m_pBasePacket = GetPacketFromId(packetId);
  if (!m_pBasePacket) wxLogMessage("Illegal packetId: %d", packetId);
}

void Context::AddPacket(int packetId)
{
  if (!m_pBasePacket) {
    SetBasePacket(packetId);
  } else {
    Packet *currentPacket = m_pBasePacket;
    while (currentPacket->GetInnerPacket()) {
      currentPacket = currentPacket->GetInnerPacket();
    }
    Packet *packet = GetPacketFromId(packetId);
    if (!packet) wxLogMessage("Illegal packetId: %d", packetId);

    currentPacket->SetInnerPacket(packet);
  }
}

void Context::RemovePacket(std::string name)
{
  if (!m_pBasePacket) return;
  if (m_pBasePacket->GetName() == name) {
    delete m_pBasePacket;
    m_pBasePacket = nullptr;
  }

  Packet *currentPacket = m_pBasePacket;
  while (currentPacket) {
    if (currentPacket->GetInnerPacket()->GetName() == name) {
      currentPacket->RemoveInnerPacket();
    }
    currentPacket = currentPacket->GetInnerPacket();
  }
}

bool Context::SendPacket()
{
  if (!IsAdapterSet()) {
    wxLogMessage("Adapter not set!");
    return false;
  }

  char errbuf[256];
  const uint32_t packetSize = m_pBasePacket->Size();
  uint8_t *packetBuffer = new uint8_t[packetSize];
  m_pBasePacket->WriteToBuf(packetBuffer, packetSize);

  if (!Utils::SendPacket(m_CurrentAdapter, packetBuffer, packetSize, errbuf)) {
    wxLogMessage("%s", errbuf);
    delete[] packetBuffer;
    return false;
  }

  delete[] packetBuffer;

  return true;
}

}// namespace PacketHacker
#include "context.h"
#include "packet/adapter.h"

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
  : m_CurrentAdapter(),
    m_AdapterSet(false),
    m_pBasePacket(),
    m_MainWindow(window)
{
  m_adapters = Utils::GetAdapters();
}

Context::~Context()
{
  delete m_CurrentAdapter;
  delete m_pBasePacket;
}

void Context::SetAdapter(std::string name)
{
  m_AdapterSet = true;
  if (m_CurrentAdapter) {
    delete m_CurrentAdapter;
  }
  m_CurrentAdapter = new Adapter(name);
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
  if (!m_CurrentAdapter->OpenPacketStream(errbuf)) {
    wxLogMessage("%s", errbuf);
    return false;
  }

  if (!m_CurrentAdapter->SendPacket(m_pBasePacket, errbuf)) {
    wxLogMessage("%s", errbuf);
    return false;
  }

  m_CurrentAdapter->ClosePacketStream();

  return true;
}

bool Context::BeginStream()
{
  if (!IsAdapterSet()) {
    wxLogMessage("Adapter not set!");
    return false;
  }

  char errbuf[256];
  if (!m_CurrentAdapter->OpenPacketStream(errbuf)) {
    wxLogMessage("%s", errbuf);
    return false;
  }

  return true;
}

void Context::EndStream()
{
  m_CurrentAdapter->ClosePacketStream();
}

const uint8_t *Context::ReadNextPacket(uint32_t *size)
{
  char errbuf[256];
  const uint8_t *data;
  if ((data = m_CurrentAdapter->GetNextPacket(size, errbuf)) == nullptr) {
    wxLogMessage("%s", errbuf);
    return nullptr;
  }
  return data;
}


}// namespace PacketHacker
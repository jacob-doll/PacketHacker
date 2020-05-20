#include "context.h"
#include <pcap.h>

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif


namespace PacketHacker
{

Context::Context()
    : m_CurrentAdapter{}, m_AdapterSet(false), m_pBasePacket()
{
    m_packetTypes = Utils::GetAvailablePackets();
    m_adapters = Utils::GetAdapters();
}

Context::~Context() {
    delete m_pBasePacket;
}

void Context::SetAdapter(int index)
{
    m_AdapterSet = true;
    m_CurrentAdapter = m_adapters[index];
}
    
void Context::SetBasePacket(int index)
{
    delete m_pBasePacket;
    PacketInfo info = m_packetTypes[index];
    m_pBasePacket = new Packet();
    m_pBasePacket->LoadPacket(info.path.c_str());
}

void Context::AddPacket(int index)
{
    if (!m_pBasePacket)
    {
        SetBasePacket(index);
    }
    else
    {
        Packet *currentPacket = m_pBasePacket;
        while (currentPacket->GetInnerPacket())
        {
            currentPacket = currentPacket->GetInnerPacket();
        }
        PacketInfo info = m_packetTypes[index];
        Packet *packet = new Packet();
        currentPacket->SetInnerPacket(packet);
        packet->LoadPacket(info.path.c_str());
    }
}

void Context::RemovePacket(std::string name)
{
    if (!m_pBasePacket) return;
    if (m_pBasePacket->GetName() == name)
    {
        delete m_pBasePacket;
        m_pBasePacket = nullptr;
    }

    Packet *currentPacket = m_pBasePacket;
    while (currentPacket)
    {
        if (currentPacket->GetInnerPacket()->GetName() == name) {
            currentPacket->RemoveInnerPacket();
        }
        currentPacket = currentPacket->GetInnerPacket();
    }
}

bool Context::SendPacket(uint8_t *data, int size)
{
    if (!IsAdapterSet())
    {
        wxLogMessage("Adapter not set!");
        return false;
    }

    pcap_t *fp;
    char errbuf[PCAP_ERRBUF_SIZE];

    if ((fp = pcap_create(m_CurrentAdapter.name.c_str(), errbuf)) == NULL)
    {
        wxLogMessage("Unable to open the adapter. %s is not supported by Npcap", m_CurrentAdapter.name.c_str());
        return false;
    }
    pcap_set_promisc(fp, 1);

    if (pcap_activate(fp) != 0)
    {
        wxLogMessage("Error activating handle: %s", pcap_geterr(fp));
        return false;
    }

    if (pcap_sendpacket(fp, data, size) != 0)
    {
        wxLogMessage("Error sending the packet: %s", pcap_geterr(fp));
        return false;
    }

    pcap_close(fp);

    return true;
}

} // namespace PacketHacker
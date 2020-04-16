#include <pcap.h>
#include "context.h"

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

namespace PacketHacker
{

Context::Context()
{
}

void Context::SetAdapter(AdapterInfo &info)
{
    m_AdapterSet = true;
    m_CurrentAdapter = info;
}

bool Context::SendPacket(u_char *data, int size)
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
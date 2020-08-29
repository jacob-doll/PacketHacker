#include "interface.h"
#include "packets.h"

#include <pcap.h>

namespace PacketHacker {

Interface::Interface()
  : m_info(), m_arpTable(0), m_streamOpen(false)
{}

Interface::Interface(const InterfaceInfo &info)
  : m_info(info), m_arpTable(info.index), m_streamOpen(false)
{
  m_arpTable.RefreshTable();
}

}// namespace PacketHacker
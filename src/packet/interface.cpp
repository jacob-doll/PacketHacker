#include "interface.h"
#include "packets.h"

#include <pcap.h>

namespace PacketHacker {

Interface::Interface()
  : m_info(), m_streamOpen(false)
{}

Interface::Interface(const InterfaceInfo &info)
  : m_info(info), m_streamOpen(false)
{
}

}// namespace PacketHacker
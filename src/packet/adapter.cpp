#include "adapter.h"
#include "packets.h"

#include <pcap.h>

namespace PacketHacker {

Adapter::Adapter(const AdapterInfo &info)
  : m_name(info.name), m_index(info.index), m_streamOpen(false)
{
}

}// namespace PacketHacker
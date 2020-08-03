#include "adapter.h"
#include "packets.h"

#include <pcap.h>

namespace PacketHacker {

Adapter::Adapter(std::string name)
  : m_name(std::move(name))
{
}

}// namespace PacketHacker